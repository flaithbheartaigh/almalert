/*
    AlarmAlertSettings.hpp
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

#ifndef __ALARMALERTSETTINGS_HPP__
#define __ALARMALERTSETTINGS_HPP__

#include <e32base.h>

class CSettings: public CBase
{
  public:
    CSettings();
    ~CSettings();
  public:
    const TDesC& Alarm(void);
    const TDesC& Calendar(void);
    const TDesC& Beep(void);
    inline TBool Beeper(void) {return iBeep&&iBeepStart<=iBeepFinish;};
    TUint8 BeepStart(void) {return iBeepStart;};
    TUint8 BeepFinish(void) {return iBeepFinish;};
    const TTimeIntervalMinutes SnoozeTime(void) {return TTimeIntervalMinutes(iSnoozeTime);};
    TInt SnoozeCount(void) {return iSnoozeCount;};
  private:
    TBool FileExist(const TDesC& aFileName);
  private:
    HBufC* iAlarm;
    HBufC* iCalendar;
    HBufC* iBeep;
    TUint8 iBeepStart;
    TUint8 iBeepFinish;
    TUint8 iSnoozeTime;
    TUint8 iSnoozeCount;
};

#endif
