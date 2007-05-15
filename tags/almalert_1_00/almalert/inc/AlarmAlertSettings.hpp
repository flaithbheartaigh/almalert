/*
    AlarmAlertSettings.hpp
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

#ifndef __ALARMALERTSETTINGS_HPP__
#define __ALARMALERTSETTINGS_HPP__

#include <e32base.h>
#include "AlmSettingsClient.hpp"

class CSettings: public CBase
{
  public:
    CSettings();
    ~CSettings();
    TBool Connected(void);
  public:
    const TDesC& Alarm(void);
    const TDesC& Calendar(void);
    const TDesC& Beep(void);
    const TDesC& Birthday(void);
    TBool IsBeep(void);
    TUint8 BeepStart(void);
    TUint8 BeepFinish(void);
    const TTimeIntervalMinutes SnoozeTime(void);
    TInt SnoozeCount(void);
    TBool IsBirthday(void);
    const TTimeIntervalDays BirthdayStart(void);
    TUint8 BirthdayHour(void);
    void Notify(TRequestStatus& aStatus);
    void NotifyCancel(void);
  private:
    TBool FileExist(const TDesC& aFileName);
    const TDesC& Load(const TDesC& aCategory,const TDesC& aName,TFileName& aValue);
    TInt Load(const TDesC& aCategory,const TDesC& aName,TInt aLow,TInt aHigh,TInt aDefault);
  private:
    RAlmSettings iSettings;
    TBool iConnected;
    TFileName iAlarm;
    TFileName iCalendar;
    TFileName iBeep;
    TFileName iBirthday;
    TBool iIsSx1;
};

#endif
