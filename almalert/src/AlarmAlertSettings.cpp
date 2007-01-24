/*
    AlarmAlertSettings.cpp
    Copyright (C) 2005-2007 zg

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

#include "AlarmAlertSettings.hpp"
#include "AlmSettingsNames.hpp"
#include <f32file.h>

_LIT(KDefault,"z:\\Nokia\\Sounds\\Digital\\Nokia tune.mid");

CSettings::~CSettings()
{
  if(iConnected) iSettings.Close();
}

CSettings::CSettings()
{
  if(iSettings.Connect()==KErrNone) iConnected=ETrue;
}

const TDesC& CSettings::Alarm(void)
{
  return Load(KCategoryAlarm,KTone,iAlarm);
}

const TDesC& CSettings::Calendar(void)
{
  return Load(KCategoryCalendar,KTone,iCalendar);
}

const TDesC& CSettings::Beep(void)
{
  return Load(KCategoryBeep,KTone,iBeep);
}

const TDesC& CSettings::Birthday(void)
{
  return Load(KCategoryBirthday,KTone,iBirthday);
}

TBool CSettings::IsBeep(void)
{
  return Load(KCategoryBeep,KEnabled,EFalse,ETrue,EFalse);
}

TUint8 CSettings::BeepStart(void)
{
  return Load(KCategoryBeep,KStart,0,23,0);
}

TUint8 CSettings::BeepFinish(void)
{
  return Load(KCategoryBeep,KFinish,0,23,0);
}

const TTimeIntervalMinutes CSettings::SnoozeTime(void)
{
  return TTimeIntervalMinutes(Load(KCategoryAlarm,KSnoozeTime,5,255,5)-5);
}

TInt CSettings::SnoozeCount(void)
{
  return Load(KCategoryAlarm,KSnoozeCount,1,255,6);
}

TBool CSettings::IsBirthday(void)
{
  return Load(KCategoryBirthday,KEnabled,EFalse,ETrue,EFalse);
}

const TTimeIntervalDays CSettings::BirthdayStart(void)
{
  return TTimeIntervalDays(Load(KCategoryBirthday,KStart,0,10,0));
}

TUint8 CSettings::BirthdayHour(void)
{
  return Load(KCategoryBirthday,KHour,0,23,12);
}

void CSettings::Notify(TRequestStatus& aStatus)
{
  iSettings.Notify(aStatus);
}

void CSettings::NotifyCancel(void)
{
  iSettings.NotifyCancel();
}

TBool CSettings::FileExist(const TDesC& aFileName)
{
  TBool res=EFalse;
  RFs fs;
  if(fs.Connect()==KErrNone)
  {
    TUint att;
    if(fs.Att(aFileName,att)==KErrNone) res=ETrue;
    fs.Close();
  }
  return res;
}

const TDesC& CSettings::Load(const TDesC& aCategory,const TDesC& aName,TFileName& aValue)
{
  if(iConnected&&iSettings.Get(aCategory,aName,aValue)==KErrNone&&FileExist(aValue)) return aValue;
  return KDefault;
}

TInt CSettings::Load(const TDesC& aCategory,const TDesC& aName,TInt aLow,TInt aHigh,TInt aDefault)
{
  TInt value=0;
  if(iSettings.Get(aCategory,aName,value)==KErrNone)
  {
    if(value<aLow||value>aHigh) value=aDefault;
  }
  else value=aDefault;
  return value;
}
