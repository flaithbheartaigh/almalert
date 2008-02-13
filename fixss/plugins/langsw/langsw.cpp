/*
    langsw.cpp
    Copyright (C) 2007 zg

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

#include <w32adll.h>
#include <apgwgnam.h>
#include <CommonEngine.hpp>
#include <SysUtilEx.hpp>
#include <SharedData.hpp>
#include <coedef.h>

class RLang: public RAnim
{
  public:
    RLang(RAnimDll& aAnimDll): RAnim(aAnimDll) {};
    TInt Construct(const RWindowBase& aDevice) {return RAnim::Construct(aDevice,0,KNullDesC8);};
};

class CShutdown: public CBase,public MSharedDataNotifyHandler
{
  public:
    static CShutdown* NewLC(void);
    ~CShutdown();
    void Wait(void);
  public:
    void HandleNotifyL(TUid anUid,const TDesC16& aKey,const TDesC16& aValue);
  private:
    CShutdown();
    void ConstructL(void);
  private:
    RSharedDataClient iSysAp;
};

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
{
  return KErrNone;
}

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
}

_LIT(KDllName,"langswa.dll");

LOCAL_C void StartL(TUint32 aHandle)
{
  CActiveScheduler* scheduler=new(ELeave)CActiveScheduler;
  CleanupStack::PushL(scheduler);
  CActiveScheduler::Install(scheduler);
  RWsSession ws;
  User::LeaveIfError(ws.Connect());
  CleanupClosePushL(ws);
  RWindowGroup root(ws);
  User::LeaveIfError(root.Construct(aHandle,EFalse));
  CleanupClosePushL(root);
  CApaWindowGroupName* groupName=CApaWindowGroupName::NewLC(ws);
  groupName->SetHidden(ETrue);
  groupName->SetWindowGroupName(root);
  CleanupStack::PopAndDestroy(); //groupName
  root.SetOrdinalPosition(-1,ECoeWinPriorityNeverAtFront);
  RBlankWindow window(ws);
  CleanupClosePushL(window);
  User::LeaveIfError(window.Construct(root,aHandle+1));
  RAnimDll dll(ws);
  User::LeaveIfError(dll.Load(KDllName));
  CleanupClosePushL(dll);
  RLang lang(dll);
  User::LeaveIfError(lang.Construct(window));
  CleanupClosePushL(lang);
  CShutdown* shutdown=CShutdown::NewLC();
  shutdown->Wait();
  CleanupStack::PopAndDestroy(7); //shutdown,lang,dll,window,root,ws,scheduler
}

EXPORT_C TInt Start(void)
{
  TInt err=KErrNoMemory;
  CTrapCleanup *cleanup=CTrapCleanup::New();
  if(cleanup)
  {
    TRAP(err,StartL((TUint32)cleanup));
    delete cleanup;
  }
  return err;
}
