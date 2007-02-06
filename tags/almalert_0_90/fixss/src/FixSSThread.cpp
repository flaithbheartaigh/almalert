/*
    FixSSThread.cpp
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

#include "FixSS.hpp"
#include "LoadNotifier.hpp"

LOCAL_C void ThreadProcL(void)
{
  CActiveScheduler* scheduler=new(ELeave)CActiveScheduler;
  CleanupStack::PushL(scheduler);
  CActiveScheduler::Install(scheduler);
  CLoadNotifier* notifier=CLoadNotifier::NewLC();
  notifier->Wait();
  CleanupStack::PopAndDestroy(2); //notifier, scheduler
}

LOCAL_C TInt ThreadProc(TAny* aParam)
{
  TInt err=KErrNoMemory;
  CTrapCleanup* cleanup=CTrapCleanup::New();
  if(cleanup)
  {
    TRAP(err,ThreadProcL());
    delete cleanup;
  }
  return err;
}

_LIT(KThreadName,"zg.fixss");

void CRecog::StartThread(void)
{
  RThread thread;
  if(thread.Create(KThreadName,ThreadProc,KDefaultStackSize,KMinHeapSize,KMinHeapSize,NULL)==KErrNone)
  {
    thread.Resume();
    thread.Close();
  }
}
