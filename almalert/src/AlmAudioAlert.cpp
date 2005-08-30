/*
    AlmAudioAlert.cpp
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

#include "AlmAudioAlert.hpp"
#include <t32alm.h> //EAlarmTypeClock
#include <avkon.hrh> //EAknAudioPrefCalendarAlarm
#include <Mda/Common/Resource.h> //KMdaRepeatForever

CAlmAudio* CAlmAudio::NewL(CEikonEnv* anEnv,const TDesC& aFileName,TInt anAlarmType)
{
  CAlmAudio* self=new(ELeave)CAlmAudio(anEnv,anAlarmType);
  CleanupStack::PushL(self);
  self->ConstructL(aFileName);
  CleanupStack::Pop(self);
  return self;
}

CAlmAudio::CAlmAudio(CEikonEnv* anEnv,TInt anAlarmType): CAlmAudioBase(anEnv),iAlarmType(anAlarmType)
{
}

TInt CAlmAudio::Priority(void)
{
  return 99;
}

TMdaPriorityPreference CAlmAudio::PriorityPreference(void)
{
  return (iAlarmType==EAlarmTypeClock)?(TMdaPriorityPreference)EAknAudioPrefAlarmClock:(TMdaPriorityPreference)EAknAudioPrefCalendarAlarm;
}

TBool CAlmAudio::PlayAlways(void)
{
  return iAlarmType==EAlarmTypeClock;
}

void CAlmAudio::PlayInit(void)
{
  iPlayer->SetRepeats(KMdaRepeatForever,TTimeIntervalMicroSeconds(0));
}
