/*
    AlmBirthdayTimer.cpp
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

#include "AlmBirthdayTimer.hpp"
#include "AlmUtils.hpp"

CBirthdayTimer::~CBirthdayTimer()
{
}

CBirthdayTimer* CBirthdayTimer::NewL(TUint8 aHour,TCallBack aCallback)
{
  CBirthdayTimer* self=new(ELeave)CBirthdayTimer(aHour,aCallback);
  CleanupStack::PushL(self);
  self->ConstructL();
  CleanupStack::Pop(self);
  return self;
}

CBirthdayTimer::CBirthdayTimer(TUint8 aHour,TCallBack aCallback): CTimer(EPriorityIdle),iCallback(aCallback),iHour(aHour)
{
}

void CBirthdayTimer::NearTime(void)
{
  TTime htime;
  htime.HomeTime();
  TDateTime time=AlmUtils::StripTime(htime.DateTime());
  User::LeaveIfError(time.SetHour(iHour));
  iTime=time;
  if(iTime<htime) iTime+=TTimeIntervalDays(1);
}

void CBirthdayTimer::ConstructL(void)
{
  CTimer::ConstructL();
  CActiveScheduler::Add(this);
  NearTime();
  At(iTime);
}

void CBirthdayTimer::RunL(void)
{
  if(iStatus==KErrNone||iStatus==KErrAbort)
  {
    if(iStatus==KErrNone)
    {
      iCallback.iFunction(iCallback.iPtr);
      iTime+=TTimeIntervalDays(1);
    }
    else NearTime();
    At(iTime);
  }
}
