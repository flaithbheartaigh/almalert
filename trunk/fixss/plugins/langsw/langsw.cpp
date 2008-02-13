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
#include "shutdown.hpp"
#include <coedef.h>

class RLang: public RAnim
{
  public:
    RLang(RAnimDll& aAnimDll): RAnim(aAnimDll) {};
    TInt Construct(const RWindowBase& aDevice) {return RAnim::Construct(aDevice,0,KNullDesC8);};
};

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
{
  return KErrNone;
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
