/*
    phiobexclient.cpp
    Copyright (C) 2007-2008 zg

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "bt/phiobexclient.hpp"
#include "bt/phiobexservicesearcher.hpp"
#include "phiutils.hpp"
#include <phi.rsg>
#include <eikprogi.h>

/*
#include <f32file.h>
static void Log(const TDesC8& aBuffer,TInt aParam)
{
  RFs fs;
  RFile file;
  fs.Connect();
  TInt err=file.Open(fs,_L("e:\\zg_phi_log.txt"),EFileWrite|EFileShareAny);
  if(err==KErrNotFound) err=file.Create(fs,_L("e:\\zg_phi_log.txt"),EFileWrite|EFileShareAny);
  TInt pos=0;
  err=file.Seek(ESeekEnd,pos);
  TBuf8<1024> logBuf;
  TTime localTime; RThread thread;
  localTime.HomeTime();
  logBuf.Format(_L8("0x%08.8d|%02.2d:%02.2d:%02.2d.%06.6d: %S: 0x%x\n"),thread.Id(),localTime.DateTime().Hour(),localTime.DateTime().Minute(),localTime.DateTime().Second(),localTime.DateTime().MicroSecond(),&aBuffer,aParam);
  file.Write(logBuf);
  file.Close();
  fs.Close();
}

static void Log(const TDesC& aBuffer,TInt aParam)
{
  TBuf8<1024> cnvBuf;
  cnvBuf.Copy(aBuffer);
  Log(cnvBuf,aParam);
}
// */

_LIT(KServerTransportName,"RFCOMM");

void CPhiObexClient::NewLD(const TDesC& aFile)
{
  CPhiObexClient* self=new(ELeave)CPhiObexClient(aFile);
  CleanupStack::PushL(self);
  self->ConstructL(aFile);
  CleanupStack::Pop(); //self
}

CPhiObexClient::CPhiObexClient(const TDesC& aFile): CActive(CActive::EPriorityStandard),iState(EIdle)
{
  CActiveScheduler::Add(this);
}

void CPhiObexClient::ConstructL(const TDesC& aFile)
{
  iServiceSearcher=CPhiObexServiceSearcher::NewL();
  iCurrObject=CObexFileObject::NewL(aFile);
  iCurrObject->InitFromFileL(aFile);
  iProgressDialog=new(ELeave)CAknProgressDialog(reinterpret_cast<CEikDialog**>(&iProgressDialog),ETrue);
  iProgressDialog->SetTone(CAknNoteDialog::ENoTone);
  iProgressDialog->SetCallback(this);
  iProgressDialog->ExecuteLD(R_PHI_OPERATION_PROGRESS_NOTE_SOFTKEY_CANCEL);
  iProgressInfo=iProgressDialog->GetProgressInfoL();
  iProgressInfo->SetFinalValue(iCurrObject->Length());
  iRedrawTimer=CPeriodic::NewL(CActive::EPriorityStandard);
  iRedrawTimer->Start(100000,1000000,TCallBack(RedrawCallBack,this));
  ConnectL();
}

void CPhiObexClient::CleanupL(void)
{
  if(iProgressDialog) iProgressDialog->ProcessFinishedL();
}

CPhiObexClient::~CPhiObexClient()
{
  delete iRedrawTimer;
  TRAPD(err,CleanupL());
  Cancel();
  delete iCurrObject;
  iCurrObject=NULL;
  delete iServiceSearcher;
  iServiceSearcher=NULL;
  delete iClient;
  iClient=NULL;
}

void CPhiObexClient::DialogDismissedL(TInt aButtonId)
{
  if(IsActive())
  {
    iRedrawTimer->Cancel();
    iProgressDialog=NULL; //dont' call ProcessFinishedL in in CleanupL
    Cancel();
    delete this;
  }
}

void CPhiObexClient::DoCancel(void)
{
  switch(iState)
  {
    case EIdle:
      break;
    case EGettingDevice:
      {
        TRAPD(err,iServiceSearcher->CancelSelectDeviceByDiscoveryL());
      }
      break;
    case EGettingService:
      iServiceSearcher->CancelFindService();
      break;
    case EGettingConnection:
    case EWaitingToSend:
    case EDisconnecting:
      {
        delete iClient;
        iClient=NULL;
      }
      break;
    default:
      User::Invariant();
      break;
  }
  iState=EIdle;
}

void CPhiObexClient::RunL(void)
{
  if(iStatus!=KErrNone)
  {
    switch(iState)
    {
      case EGettingDevice:
      case EGettingService:
      case EGettingConnection:
      case EDisconnecting:
      case EWaitingToSend:
        iState=EIdle;
        break;
      default:
        User::Invariant();
        break;
    }
  }
  else
  {
    switch(iState)
    {
      case EGettingDevice:
        iState=EGettingService;
        iStatus=KRequestPending;
        iServiceSearcher->FindServiceL(iStatus);
        SetActive();
        break;
      case EGettingService:
        iState=EGettingConnection;
        ConnectToServerL();
        break;
      case EGettingConnection:
        iState=EWaitingToSend;
        SendObjectL(); //!
        break;
      case EWaitingToSend:
        DisconnectL(); //!
        break;
      case EDisconnecting:
        iState=EIdle;
        break;
      default:
        User::Invariant();
        break;
    }
  }
  if(iState==EIdle) delete this;
}

void CPhiObexClient::ConnectL(void)
{
  if(iState==EIdle&&!IsActive())
  {
    iServiceSearcher->SelectDeviceByDiscoveryL(iStatus);
    iState=EGettingDevice;
    SetActive();
  }
  else
  {
    User::Leave(KErrInUse);
  }
}

void CPhiObexClient::ConnectToServerL(void)
{
  TObexBluetoothProtocolInfo protocolInfo;

  protocolInfo.iTransport.Copy(KServerTransportName);
  protocolInfo.iAddr.SetBTAddr(iServiceSearcher->BTDevAddr());
  protocolInfo.iAddr.SetPort(iServiceSearcher->Port());

  if(iClient)
  {
    delete iClient;
    iClient=NULL;
  }
  iClient=CObexClient::NewL(protocolInfo);

  iClient->Connect(iStatus);
  SetActive();
}

void CPhiObexClient::SendObjectL(void)
{
  if(iState!=EWaitingToSend)
  {
    User::Leave(KErrDisconnected);
  }
  else if(IsActive())
  {
    User::Leave(KErrInUse);
  }
  iClient->Put(*iCurrObject,iStatus);
  SetActive();
}

void CPhiObexClient::DisconnectL(void)
{
  if(iState==EIdle)
  {
    return;
  }
  if(iState==EWaitingToSend)
  {
    iState=EDisconnecting;
    iClient->Disconnect(iStatus);
    SetActive();
  }
  else
  {
    User::Leave(KErrInUse);
  }
}

TInt CPhiObexClient::RedrawCallBack(TAny* aPhiObexClient)
{
  return ((CPhiObexClient*)aPhiObexClient)->DoRedraw();
}

TInt CPhiObexClient::DoRedraw(void)
{
  iProgressInfo->SetAndDraw(iCurrObject->BytesSent());
  TBuf<64> size;
  PhiUtils::FormatSize(iCurrObject->BytesSent(),size);
  TBuf<KMaxFileName+64+2> buffer;
  buffer.Append(iCurrObject->Name());
  buffer.Append(':');
  buffer.Append(' ');
  buffer.Append(size);
  TRAPD(err,iProgressDialog->SetTextL(buffer));
  return 0;
}
