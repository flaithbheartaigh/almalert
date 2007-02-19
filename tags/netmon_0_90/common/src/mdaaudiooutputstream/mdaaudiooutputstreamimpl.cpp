/*
    mdaaudiooutputstreamimpl.cpp
    Copyright (C) 2005 zg

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include <mdaaudiooutputstreamimpl.hpp>

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
{
  return KErrNone;
}

static void StreamPanic(TInt aPanic)
{
  User::Panic(_L("CMdaAudioOutputStream"),aPanic);
}

CSendBufferActive::CSendBufferActive(): CActive(EPriorityHigh)
{
  CActiveScheduler::Add(this);
}

void CMdaAudioOutputStreamImpl::Start()
{
  if(!iSendBufferActive.IsActive())
  {
    if(!iPlayActive.IsActive())
    {
      iDevice.Play(iPlayActive.iStatus);
      iPlayActive.SetActive();
      iState=EMdaStreamPlayerPlaying;
    }
    iDevice.SendBuffer(iQueue->SafeFirst()->Buffer(),RThread().Id(),iSendBufferActive.iStatus);
    iSendBufferActive.SetActive();
  }
}

void CSendBufferActive::RunL()
{
  CSoundBuffer* buffer=iStream->iQueue->SafeFirst();
  iStream->iCallBack->MaoscBufferCopied(iStatus.Int(),*buffer->Buffer());
  buffer=iStream->iQueue->RemoveFirst();
  delete buffer;
  if(iStream->iQueue->SafeFirst()) iStream->Start();
}

void CSendBufferActive::DoCancel()
{
  iStream->iDevice.CancelSendBuffer(iStatus);
}

COpenActive::COpenActive(): CActive(EPriorityStandard)
{
  CActiveScheduler::Add(this);
}

void COpenActive::DoCancel()
{
  iStream->iDevice.CancelOpen(iStatus);
}

void COpenActive::RunL()
{
  if(iStatus.Int()==KErrNone)
  {
    if(!iSettings&&iSettings->Uid()==KUidMdaMediaTypeAudio)
    { //iSettings is TMdaAudioDataSettings<-TMdaDatatypeSettings<-TMdaPackage
      if(!iSettings->iVolume)
      {
        iStream->iConfig.iVolume=iSettings->iVolume;
        iStream->iConfig.iMask|=TMdaAudioOutputConfig::EVolumeChange;
      }
      if(!iSettings->iChannels)
      {
        iStream->iConfig.iNumChannels=iSettings->iChannels;
        iStream->iConfig.iMask|=TMdaAudioOutputConfig::EChannelsChange;
      }
      if(!iSettings->iSampleRate)
      {
        iStream->iConfig.iSampleRate=iSettings->iSampleRate;
        iStream->iConfig.iMask|=TMdaAudioOutputConfig::ESampleRateChange;
      }
    }
    else
    {
      iStream->iConfig.iMask=TMdaAudioOutputConfig::ENull;
    }
    TInt err;
    {
      TTrap trap;
      if(trap.Trap(err)==0)
      {
        iStream->iDevice.InitL(iStream->iConfig);
        TTrap::UnTrap();
      }
    }
    if(err!=KErrNone)
    {
      iStream->iCallBack->MaoscOpenComplete(err);
    }
    else
    {
      iStream->iConfig.iMask=TMdaAudioOutputConfig::EQuery;
      iStream->iDevice.Config(iStream->iConfig);
      iStream->iState=EMdaStreamPlayerOpen;
      iStream->iCallBack->MaoscOpenComplete(iStatus.Int());
    }
  }
  else iStream->iCallBack->MaoscOpenComplete(iStatus.Int());
}

CPlayActive::CPlayActive(): CActive(EPriorityHigh-1)
{
  CActiveScheduler::Add(this);
}

void CPlayActive::DoCancel()
{
  iStream->iDevice.CancelPlay(iStatus);
}

void CPlayActive::RunL()
{
  if(!iStream->iQueue->SafeFirst())
  {
    iStream->iSendBufferActive.Cancel();
    iStream->iState=EMdaStreamPlayerOpen;
  }
  else
  {
    if(iStatus.Int()==KErrUnderflow)
    {
      iStream->iSendBufferActive.Cancel();
      iStream->Start();
      return;
    }
    iStream->Stop();
  }
  iStream->iCallBack->MaoscPlayComplete(iStatus.Int());
}

const TInt CSoundBuffer::iOffset=_FOFF(CSoundBuffer,iSlink);

CSoundBuffer::CSoundBuffer(const TDesC8* aBuffer): CBase(),iBuffer(aBuffer)
{
}

CSoundBuffer* CSoundBuffer::NewL(const TDesC8* aBuffer)
{
  return new(ELeave)CSoundBuffer(aBuffer);
}

CSoundBuffer::~CSoundBuffer()
{

}

const TDesC8* CSoundBuffer::Buffer(void) const
{
  return iBuffer;
}

CSoundBufferQueue* CSoundBufferQueue::NewL(void)
{
  return new(ELeave)CSoundBufferQueue;
}

CSoundBufferQueue::CSoundBufferQueue(): iBuffers(CSoundBuffer::iOffset),iBuffersIter(iBuffers)
{
}

CSoundBufferQueue::~CSoundBufferQueue()
{
  CSoundBuffer* item;
  iBuffersIter.SetToFirst();
  while((item=iBuffersIter++))
  {
    iBuffers.Remove(*item);
    delete item;
  }
}

CSoundBuffer* CSoundBufferQueue::SafeFirst(void)
{
  CSoundBuffer* res;
  if(!iBuffers.IsEmpty()) res=iBuffers.First();
  else res=NULL;
  return res;
}

CSoundBuffer* CSoundBufferQueue::RemoveFirst(void)
{
  CSoundBuffer* res;
  if(!iBuffers.IsEmpty())
  {
    res=iBuffers.First();
    iBuffers.Remove(*res);
  }
  else res=NULL;
  return res;
}

CMdaAudioOutputStream::CMdaAudioOutputStream(void)
{
}

EXPORT_C CMdaAudioOutputStream* CMdaAudioOutputStreamImpl::NewL(MMdaAudioOutputStreamCallback& aCallBack,CMdaServer* aServer)
{
  CMdaAudioOutputStreamImpl* self=new(ELeave)CMdaAudioOutputStreamImpl(aCallBack,aServer);
  CleanupStack::PushL(self);
  self->ConstructL();
  CleanupStack::Pop(self);
  return self;
}

CMdaAudioOutputStreamImpl::CMdaAudioOutputStreamImpl(MMdaAudioOutputStreamCallback& aCallBack,CMdaServer* aServer): CMdaAudioOutputStream(),iCallBack(&aCallBack),iServer(aServer)
{
  iSendBufferActive.iStream=this;
  iPlayActive.iStream=this;
  iOpenActive.iStream=this;
  iConfig.iMaxVolume=-1;
}

void CMdaAudioOutputStreamImpl::ConstructL()
{
  if(!iServer)
  {
    iServer=CMdaServer::NewL();
    iOwnServer=ETrue;
  }
  iQueue=CSoundBufferQueue::NewL();
}

CMdaAudioOutputStreamImpl::~CMdaAudioOutputStreamImpl()
{
  if(iState!=EMdaStreamPlayerNotOpen) iDevice.Stop();
  iSendBufferActive.Cancel();
  iPlayActive.Cancel();
  iOpenActive.Cancel();
  iDevice.Close();
  if(iOwnServer) delete iServer;
  delete iQueue;
}

void CMdaAudioOutputStreamImpl::SetAudioPropertiesL(TInt aSampleRate,TInt aChannels)
{
  if(iState==EMdaStreamPlayerNotOpen) StreamPanic(EMdaStreamPlayerPanicNotOpen);
  if(iState==EMdaStreamPlayerPlaying) StreamPanic(EMdaStreamPlayerPanicBusyPlaying);
  iConfig.iMask=TMdaAudioOutputConfig::ESampleRateChange|TMdaAudioOutputConfig::EChannelsChange;
  iConfig.iSampleRate=aSampleRate;
  iConfig.iNumChannels=aChannels;
  iDevice.ConfigL(iConfig);
}

void CMdaAudioOutputStreamImpl::Open(TMdaPackage* aSettings)
{
  if(iState!=EMdaStreamPlayerNotOpen) StreamPanic(EMdaStreamPlayerPanicAlreadyOpen);
  if(iOpenActive.IsActive()) StreamPanic(EMdaStreamPlayerPanicAlreadyOpen);
  iOpenActive.iSettings=(TMdaAudioDataSettings*)aSettings;
  iDevice.Open(iServer->Registry(),iOpenActive.iStatus);
  iOpenActive.SetActive();
}

TInt CMdaAudioOutputStreamImpl::MaxVolume()
{
  return iConfig.iMaxVolume;
}

TInt CMdaAudioOutputStreamImpl::Volume()
{
  if(iState==EMdaStreamPlayerNotOpen) StreamPanic(EMdaStreamPlayerPanicNotOpen);
  iConfig.iMask=TMdaAudioOutputConfig::EQuery;
  iDevice.Config(iConfig);
  return iConfig.iVolume;
}

void CMdaAudioOutputStreamImpl::SetVolume(const TInt aNewVolume)
{
  if(iState==EMdaStreamPlayerNotOpen) StreamPanic(EMdaStreamPlayerPanicNotOpen);
  if(aNewVolume>MaxVolume()||aNewVolume<0) StreamPanic(EMdaStreamPlayerPanicVolumeOutOfRange);
  iConfig.iMask=TMdaAudioOutputConfig::EVolumeChange;
  iConfig.iVolume=aNewVolume;
  iDevice.Config(iConfig);
}

void CMdaAudioOutputStreamImpl::SetPriority(TInt aPriority,TMdaPriorityPreference aPref)
{
  if(iState==EMdaStreamPlayerNotOpen) StreamPanic(EMdaStreamPlayerPanicNotOpen);
  if(aPriority>100) StreamPanic(EMdaStreamPlayerPanicPriorityOutOfRange);
  iConfig.iMask=TMdaAudioOutputConfig::EPriorityChange;
  iConfig.iPriority=aPriority;
  iConfig.iPref=aPref;
  iDevice.Config(iConfig);
}

void CMdaAudioOutputStreamImpl::WriteL(const TDesC8& aData)
{
  if(iState==EMdaStreamPlayerNotOpen) StreamPanic(EMdaStreamPlayerPanicNotOpen);
  CSoundBuffer* newItem=CSoundBuffer::NewL(&aData);
  iQueue->iBuffers.AddLast(*newItem);
  Start();
}

void CMdaAudioOutputStreamImpl::Stop()
{
  if(iState==EMdaStreamPlayerNotOpen) StreamPanic(EMdaStreamPlayerPanicNotOpen);
  if(iState!=EMdaStreamPlayerOpen)
  {
    if(iSendBufferActive.IsActive()) iSendBufferActive.Cancel();
    iDevice.Stop();
    for(CSoundBuffer* buffer;(buffer=iQueue->SafeFirst());)
    {
      iCallBack->MaoscBufferCopied(KErrAbort,*buffer->Buffer());
      delete iQueue->RemoveFirst();
    }
    iState=EMdaStreamPlayerOpen;
  }
}

const TTimeIntervalMicroSeconds& CMdaAudioOutputStreamImpl::Position()
{
  if(iState==EMdaStreamPlayerNotOpen) StreamPanic(EMdaStreamPlayerPanicNotOpen);
  iConfig.iMask=TMdaAudioOutputConfig::EQuery;
  iDevice.Config(iConfig);
  return iConfig.iPosition;
}
