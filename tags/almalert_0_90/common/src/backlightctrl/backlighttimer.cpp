/*
    backlighttimer.cpp
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

#include "backlighttimer.hpp"

CBackLightTimer* CBackLightTimer::NewL(MTimeOutNotify* aTimeOutNotify,TUint aParam)
{
  CBackLightTimer* self=new(ELeave)CBackLightTimer(aTimeOutNotify,aParam);
  CleanupStack::PushL(self);
  self->ConstructL();
  CleanupStack::Pop();
  return self;
}

CBackLightTimer::CBackLightTimer(MTimeOutNotify* aTimeOutNotify,TUint aParam): CTimer(EPriorityHigh),iNotify(aTimeOutNotify),iParam(aParam)
{
  if(!aTimeOutNotify) User::Invariant();
}

CBackLightTimer::~CBackLightTimer()
{
  iNotify=NULL;
}

TInt CBackLightTimer::Start(TInt anInterval)
{
  if(!CActiveScheduler::Current()) User::Invariant();
  if(!IsAdded()) CActiveScheduler::Add(this);
  if(IsActive()) Cancel();
  if(anInterval) After(anInterval*1000);
  return KErrNone;
}

void CBackLightTimer::RunL(void)
{
  if(!iNotify) User::Invariant();
  iNotify->TimerExpired(iParam);
}
