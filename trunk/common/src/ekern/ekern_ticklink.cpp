/*
    ekern_ticklink.cpp
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

EXPORT_C TTickLink::TTickLink() //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void TTickLink::PeriodicInMicroSeconds(TTimeIntervalMicroSeconds32 aPeriod,TTickCallBack aCallBack,TAny* aPtr,TBool aRegulated) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void TTickLink::OneShotInMicroSeconds(TTimeIntervalMicroSeconds32 aTime,TTickCallBack aCallBack,TAny* aPtr) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void TTickLink::Lock(TInt aTicks,TTickCallBack aCallBack,TAny* aPtr) //FIXME: NOT IMPLEMENTED
{
}

TInt TTickLink::GetNextLock(TTimerLockSpec aMark, TInt &aTickCount) const //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C void TTickLink::Cancel(void) //FIXME: NOT IMPLEMENTED
{
}

TTickQ::TTickQ(): iSecondQueue(NULL)
{
}

void TTickQ::StartSecondQueue(TSecondQ* aQueue)
{
  iLastTimeInSecs=Plat::SystemTimeInSecondsFrom2000().Int();
  TInt curr=Kern::HomeTimeOffset().Int()+iLastTimeInSecs;
  TInt diff=curr/86400*86400;
  diff=curr-diff;
  iTomorrowStarts=curr-diff;
  if(diff>=0) iTomorrowStarts+=86400;
  iSecondQueue=aQueue;
}

TBool TTickQ::CheckMidnightCrossover(TInt aSecs)
{
  TInt curr=aSecs+Kern::HomeTimeOffset().Int();
  if(curr>=iTomorrowStarts||curr<(iTomorrowStarts-86400))
  {
    TInt diff=curr/86400*86400;
    diff=curr-diff;
    iTomorrowStarts=curr-diff;
    if(diff>=0) iTomorrowStarts+=86400;
    return ETrue;
  }
  return EFalse;
}

void TTickQ::WakeUp(void) //FIXME: NOT IMPLEMENTED
{
}

void TTickQ::Tick(TInt aBatch) //FIXME: NOT IMPLEMENTED
{
}

void TTickQ::SystemTimeChanged(void) //FIXME: NOT IMPLEMENTED
{
}

TInt TTickQ::GetIdleTime(TBool aIgnoreLocked)
{
  TInt res=0;
  TDblQueIter<TTickLink> iter(*this);
  for(;iter;iter++)
  {
    res+=(*iter).iDelta;
    if((*iter).iPeriod!=-1||!aIgnoreLocked) break;
  }
  if(!iter) return KErrNotFound;
  else return res;
}

EXPORT_C TSecondLink::TSecondLink()
{
}

EXPORT_C TInt TSecondLink::At(const TTime& aHomeTime,TSecondCallBack aCallBack,TAny* aPtr) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C void TSecondLink::Cancel(void) //FIXME: NOT IMPLEMENTED
{
}

void TSecondQ::Tick(void) //FIXME: NOT IMPLEMENTED
{
}

void TSecondQ::SystemTimeChanged(void) //FIXME: NOT IMPLEMENTED
{
}

void TSecondQ::SystemTimeChanged(TUint& aChanges) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C TInactivityLink::TInactivityLink(): iTime(0)
{
}

EXPORT_C TInt TInactivityLink::Start(TInt aSeconds,TInactivityCallBack aCallBack,TAny* aPtr) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C void TInactivityLink::Cancel(void) //FIXME: NOT IMPLEMENTED
{
}

TInactivityQ::TInactivityQ(): TDblQue<TInactivityLink>(0),iPending(0)
{
}

void TInactivityQ::Tick(void) //FIXME: NOT IMPLEMENTED
{
}

void TInactivityQ::Reset(void) //FIXME: NOT IMPLEMENTED
{
}

TInt TInactivityQ::InactiveTime(void) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

TBool TInactivityQ::FirstDelta(TInt& aDelta) //FIXME: NOT IMPLEMENTED
{
  return EFalse;
}
