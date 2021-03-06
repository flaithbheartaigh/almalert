/*
    btin.cpp
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

#include <e32base.h>
#include "btobexserver.hpp"
#include "shutdown.hpp"

_LIT8(KCopyright,"(c) by zg. version 0.444");

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
{
  KCopyright();
  return KErrNone;
}

LOCAL_C void MainL(void)
{
  CActiveScheduler* scheduler=new(ELeave)CActiveScheduler;
  CleanupStack::PushL(scheduler);
  CActiveScheduler::Install(scheduler);
  CBtListenObex::NewLC();
  CBtListenFtp::NewLC();
  CShutdown* shutdown=CShutdown::NewLC();
  shutdown->Wait();
  CleanupStack::PopAndDestroy(4); //shutdown,btftp,btobex,scheduler
}

LOCAL_C TInt ThreadProc(TAny* aParam)
{
  CTrapCleanup* cleanup=CTrapCleanup::New();
  TRAPD(err,MainL());
  delete cleanup;
  return err;
}

_LIT(KThreadName,"zg.btin");

EXPORT_C TInt Start(void)
{
  RThread thread;
  TInt err=thread.Create(KThreadName,ThreadProc,KDefaultStackSize,KMinHeapSize*16*4,KMinHeapSize*16*4,NULL);
  if(err==KErrNone)
  {
    thread.Resume();
    TRequestStatus status;
    thread.Logon(status);
    User::WaitForRequest(status);
    thread.Close();
  }
  return err;
}
