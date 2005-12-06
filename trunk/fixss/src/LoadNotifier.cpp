/*
    LoadNotifier.cpp
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

#include <SysUtilEx.hpp>
#include <SharedData.hpp>
#include <f32file.h>
#include "LoadNotifier.hpp"

CLoadNotifier* CLoadNotifier::NewLC(void)
{
  CLoadNotifier* self=new(ELeave)CLoadNotifier;
  CleanupStack::PushL(self);
  self->ConstructL();
  return self;
}

CLoadNotifier::~CLoadNotifier()
{
  iSysAp.Close();
}


void CLoadNotifier::Patch1stL(void)
{
  _LIT(KPhoneStackMask,"*Phone::$STK");
  _LIT(KGD1Eng,"gd1eng.dll");
  TFullName result;
  TFindChunk chunks(KPhoneStackMask);
  User::LeaveIfError(chunks.Next(result));
  RChunk chunk;
  User::LeaveIfError(chunk.Open(chunks));
  CleanupClosePushL(chunk);
  RLibrary gd1eng;
  User::LeaveIfError(gd1eng.Load(KGD1Eng));
  CleanupClosePushL(gd1eng);
  TUint8* export1=(TUint8*)gd1eng.Lookup(1);
  User::LeaveIfNull(export1);
  export1=export1+19;
  TUint32 vtable=*(TUint32*)export1;
  TUint32 inactive=(TUint32)gd1eng.Lookup(4);
  User::LeaveIfNull((TAny*)inactive);
  inactive+=56;

  TUint32* data=(TUint32*)(chunk.Base()+chunk.Bottom());
  TInt length=(chunk.Top()-chunk.Bottom())/sizeof(TUint32);
  TInt flagIndex=0,inactiveIndex=0;
  for(TInt i=0;i<length;i++)
  {
    if(!flagIndex&&data[i]==vtable)
    {
      flagIndex=i+19;
    }
    if(!inactiveIndex&&data[i]==inactive)
    {
      inactiveIndex=i;
    }
    if(flagIndex&&inactiveIndex) break;
  }
  if(flagIndex&&inactiveIndex)
  {
    data[inactiveIndex+2]=data[inactiveIndex];
    data[flagIndex]=1;
  }
  CleanupStack::PopAndDestroy(2); //gd1eng, chunk
}

void CLoadNotifier::Patch2ndL(void)
{
  _LIT(KMediaStackMask,"*MediaServer::$STK");
  _LIT(KMediaThreadMask,"*::MediaServer");
  TFullName result;
  TFindChunk chunks(KMediaStackMask);
  User::LeaveIfError(chunks.Next(result));
  RChunk chunk;
  User::LeaveIfError(chunk.Open(chunks));
  CleanupClosePushL(chunk);
  TUint32* data=(TUint32*)(chunk.Base()+chunk.Bottom());
  TInt length=(chunk.Top()-chunk.Bottom())/sizeof(TUint32);
  TFindThread threads(KMediaThreadMask);
  User::LeaveIfError(threads.Next(result));
  RThread thread;
  User::LeaveIfError(thread.Open(threads));
  CleanupClosePushL(thread);
  User::LeaveIfError(user::Open(16,16,16,0x10000));
  CleanupStack::PushL(TCleanupItem((TCleanupOperation)user::Close,NULL));
  TUint32 value;
  TPtr8 valueP((TUint8*)&value,sizeof(value));
  for(int i=0;i<(length-6);i++)
  {
    User::LeaveIfError(user::op(Read,thread.Id(),data+i,value));
    if(value==0xfd7f0100)
    {
      User::LeaveIfError(user::op(Read,thread.Id(),data+i+1,value));
      if(value==0xf050010f)
      {
        User::LeaveIfError(user::op(Read,thread.Id(),data+i+6,value));
        if(value==0x84031205)
        {
          value&=0xffff;
          User::LeaveIfError(user::op(Write,thread.Id(),data+i+6,value));
          break;
        }
        else if(value==0x1205) break;
      }
    }
  }
  CleanupStack::PopAndDestroy(3); //user,thread,chunk
}

void CLoadNotifier::OnGuiL(void)
{
  Patch1stL();
  Patch2ndL();
  LoadPluginsL();
}

void CLoadNotifier::SharedDataNotify(TUid anUid,const TDesC16& aKey,const TDesC16& aValue)
{
  if(SysStartup::State()==ESWState203)
  {
    TRAPD(err,OnGuiL());
    CActiveScheduler::Stop();
  }
}

CLoadNotifier::CLoadNotifier(): CBase(),iSysAp(this)
{
}

void CLoadNotifier::Wait(void)
{
  if(SysStartup::State()==ESWState203)
  {
    TRAPD(err,OnGuiL());
  }
  else CActiveScheduler::Start();
}

void CLoadNotifier::ConstructL(void)
{
  User::LeaveIfError(iSysAp.Connect(0));
  User::LeaveIfError(iSysAp.NotifySet(KSysUtilUid,&KKeyStateVal));
}

void CLoadNotifier::LoadPluginsL(void)
{
}
