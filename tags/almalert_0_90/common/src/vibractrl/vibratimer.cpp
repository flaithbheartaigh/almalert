/*
    vibratimer.cpp
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

#include "vibratimer.hpp"

CVibraTimer* CVibraTimer::NewL(MTimeOutNotify* aTimeOutNotify)
{
  CVibraTimer* self=new(ELeave)CVibraTimer(aTimeOutNotify);
  CleanupStack::PushL(self);
  self->ConstructL();
  CleanupStack::Pop();
  return self;
}

CVibraTimer::CVibraTimer(MTimeOutNotify* aTimeOutNotify): CTimer(EPriorityHigh),iNotify(aTimeOutNotify)
{
  if(!aTimeOutNotify) User::Invariant();
}

CVibraTimer::~CVibraTimer()
{
  iNotify=NULL;
}

TInt CVibraTimer::Start(TInt anInterval)
{
  if(!CActiveScheduler::Current()) User::Invariant();
  if(!IsAdded()) CActiveScheduler::Add(this);
  if(IsActive()) Cancel();
  if(anInterval) After(anInterval*1000);
  else After(15000000);
  return KErrNone;
}

void CVibraTimer::RunL(void)
{
  if(!iNotify) User::Invariant();
  iNotify->TimerExpired();
}
