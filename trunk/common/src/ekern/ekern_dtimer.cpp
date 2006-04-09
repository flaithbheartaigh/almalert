/*
    ekern_dthread.cpp
    Copyright (C) 2006 zg

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

class TProcessMemoryInfo; //don't exists in symbian 6.1
#include <k32std.h>
#include <e32std.h>
#include <m32std.h>

void DTimer::TimerComplete(TAny* aPtr,TInt aDelay)
{
  DTimer* self=(DTimer*)aPtr;
  self->iOwningThread->RequestComplete(self->iStatus,KErrNone);
}

void DTimer::SecondComplete(TAny* aPtr)
{
  DTimer* self=(DTimer*)aPtr;
  self->iOwningThread->RequestComplete(self->iStatus,KErrNone);
}

void DTimer::LockComplete(TAny* aPtr,TInt aDelay) //FIXME: NOT IMPLEMENTED
{
}

void DTimer::LockSynchronize(TAny* aPtr,TInt aDelay) //FIXME: NOT IMPLEMENTED
{
}

DTimer::DTimer()
{
}

DTimer::~DTimer()
{
  Cancel();
}

void DTimer::Create(DThread* aThread)
{
  SetOwner(aThread);
  iOwningThread=aThread;
}


EXPORT_C void DTimer::Cancel(void)
{
  if(iStatus)
  {
    if(iType==EAbsolute)
    {
      iSecond.Cancel();
    }
    else if(iType==EInactivity)
    {
      iInactivity.Cancel();
    }
    else
    {
      iTimer.Cancel();
    }
    iOwningThread->RequestComplete(iStatus,KErrCancel);
  }
}

EXPORT_C void DTimer::Abort(void) //yes, this method do nothing
{
}

EXPORT_C void DTimer::AbortAbsolute(void)
{
  if(iStatus&&iType==EAbsolute)
  {
    iSecond.Cancel();
    iOwningThread->RequestComplete(iStatus,KErrAbort);
  }
}

EXPORT_C void DTimer::AbortLocked() //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void DTimer::After(TRequestStatus &aStatus,TTimeIntervalMicroSeconds32 anInterval) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void DTimer::At(TRequestStatus &aStatus,const TTime &aTime) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void DTimer::Lock(TRequestStatus &aStatus, TTimerLockSpec aLock) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void DTimer::Inactivity(TRequestStatus& aStatus, TInt aSeconds) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C DTimer* Kern::TimerFromHandle(TInt aHandle) //FIXME: NOT IMPLEMENTED
{
  return NULL;
}

void S::TimerCreateL(TInt& aHandle,DThread* anOwningThread,DTimer*& aTimer) //FIXME: NOT IMPLEMENTED
{
  aTimer=new(ELeave)DTimer;
  aTimer->Create(anOwningThread);
  K::Timers->AddL(aTimer);
  //FIXME
}

TInt S::TimerCreate(TInt& aHandle,DThread* anOwningThread)
{
  DTimer* timer=NULL;
  TRAPD(err,TimerCreateL(aHandle,anOwningThread,timer));
  if(err!=KErrNone) S::CheckedClose(timer);
  return err;
}

EXPORT_C TTime Kern::HomeTime()
{
  return Plat::SystemTime()+Kern::HomeTimeOffset();
}

EXPORT_C TTimeIntervalSeconds Kern::HomeTimeOffset() //FIXME: NOT IMPLEMENTED
{
  return TTimeIntervalSeconds(0);
}

TUint K::AdjustTickCount(TUint aCount)
{
  TInt level=ImpHal::DisableIrqsToLevel2();
  TUint rtc=RtcTickRef;
  TUint count=TickCountRef;
  ImpHal::RestoreIrqs(level);
  return aCount-count+rtc;
}

TUint K::AdjustedTickCounter(void)
{
  return AdjustTickCount(TickCounter);
}
