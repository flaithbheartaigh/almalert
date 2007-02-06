/*
    ActivityManager.cpp
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

#include <ActivityManager.hpp>

EXPORT_C CUserActivityManager* CUserActivityManager::NewL(TInt aPriority)
{
  CUserActivityManager* self=new(ELeave)CUserActivityManager(aPriority);
  CleanupStack::PushL(self);
  self->ConstructL();
  CActiveScheduler::Add(self);
  CleanupStack::Pop(); //self
  return self;
}

EXPORT_C CUserActivityManager::CUserActivityManager(TInt aPriority): CTimer(aPriority),iInactivity(),iActivity(),iState(EWaitingForInactivity)
{
}

EXPORT_C CUserActivityManager::~CUserActivityManager(void)
{
}

EXPORT_C void CUserActivityManager::Start(TTimeIntervalSeconds aSeconds,TCallBack aInactivity,TCallBack aActivity)
{
  iSeconds=aSeconds;
  iInactivity=aInactivity;
  iActivity=aActivity;
  WaitForInactivity();
}

EXPORT_C void CUserActivityManager::SetInactivityTimeout(TTimeIntervalSeconds aSeconds)
{
  iSeconds=aSeconds;
  UpdateTimeout();
}

void CUserActivityManager::WaitForInactivity(void)
{
  if(iState==EWaitingForInactivity) Inactivity(iSeconds);
}

void CUserActivityManager::WaitForActivity(void)
{
  if(iState==EWaitingForActivity)
  {
    Inactivity(0);
    if(User::InactivityTime()<iSeconds)
    {
      Cancel();
      iState=EWaitingForInactivity;
      iActivity.CallBack();
      WaitForInactivity();
    }
  }
}

void CUserActivityManager::UpdateTimeout(void)
{
  if(iState==EWaitingForInactivity)
  {
    Cancel();
    TInt seconds=iSeconds.Int()-User::InactivityTime().Int();
    if(seconds>0)
    {
      iStatus=KErrNone;
      Inactivity(seconds);
    }
    else
    {
      iState=EWaitingForActivity;
      iInactivity.CallBack();
      WaitForActivity();
    }
  }
}

EXPORT_C void CUserActivityManager::RunL(void)
{
  if(iStatus==KErrNone)
  {
    TTimeIntervalSeconds seconds=User::InactivityTime();
    if(iState==EWaitingForInactivity)
    {
      if(seconds>=iSeconds)
      {
        iState=EWaitingForActivity;
        iInactivity.CallBack();
        WaitForActivity();
      }
      else
      {
        WaitForInactivity();
      }
    }
    else
    {
      iState=EWaitingForInactivity;
      iActivity.CallBack();
      WaitForInactivity();
    }
  }
}

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
{
  return KErrNone;
}
