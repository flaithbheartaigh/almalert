/*
    shutdown.cpp
    Copyright (C) 2008 zg

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

#include "shutdown.hpp"
#include <SysUtilEx.hpp>
#include <SharedData.hpp>

CShutdown* CShutdown::NewLC(void)
{
  CShutdown* self=new(ELeave)CShutdown;
  CleanupStack::PushL(self);
  self->ConstructL();
  return self;
}

CShutdown::~CShutdown()
{
  iSysAp.Close();
}

void CShutdown::HandleNotifyL(TUid anUid,const TDesC16& aKey,const TDesC16& aValue)
{
  if(SysStartup::State()==ESWState204)
  {
    CActiveScheduler::Stop();
  }
  _LIT(KVoiceRecorderRecording,"1");
  if(anUid==KSDUidVoiceRecorder&&aKey==KVoiceRecorderMode&&aValue==KVoiceRecorderRecording)
  {
    TFullName result;
    TFindChunk chunks(_L("*Voicerecorder::$STK"));
    if(chunks.Next(result)==KErrNone)
    {
      RChunk chunk;
      chunk.Open(chunks);
      TUint32* data=(TUint32*)(chunk.Base()+chunk.Bottom());
      TInt length=(chunk.Top()-chunk.Bottom())/sizeof(TUint32);
      for(TInt ii=0;ii<length;++ii)
      {
        if(data[ii]==60000000)
        {
          data[ii]=480000000; //8 minutes
          break;
        }
        if(data[ii]==480000000) break;
      }
    }
  }
}

CShutdown::CShutdown(): CBase(),iSysAp(this)
{
}

void CShutdown::Wait(void)
{
  CActiveScheduler::Start();
}

void CShutdown::ConstructL(void)
{
  User::LeaveIfError(iSysAp.Connect(0));
  User::LeaveIfError(iSysAp.NotifySet(KSysUtilUid,&KKeyStateVal));
  User::LeaveIfError(iSysAp.NotifySet(KSDUidVoiceRecorder,&KVoiceRecorderMode));
}
