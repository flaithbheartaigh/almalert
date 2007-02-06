/*
    ActivityManager.hpp
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

#ifndef __ACTIVITYMANAGER_HPP__
#define __ACTIVITYMANAGER_HPP__

#include <e32base.h>

class CUserActivityManager: public CTimer
{
  public:
    IMPORT_C static CUserActivityManager* NewL(TInt aPriority);
    IMPORT_C ~CUserActivityManager(void);
  public:
    IMPORT_C void SetInactivityTimeout(TTimeIntervalSeconds aSeconds);
    IMPORT_C void Start(TTimeIntervalSeconds aSeconds,TCallBack aInactivity,TCallBack aActivity);
  protected:
    IMPORT_C CUserActivityManager(TInt aPriority);
    IMPORT_C void RunL(void);
  private:
    void WaitForInactivity(void);
    void WaitForActivity(void);
    void UpdateTimeout(void);
  private:
    enum TState
    {
      EWaitingForActivity,
      EWaitingForInactivity
    };
  private:
    TTimeIntervalSeconds iSeconds; //0x1c
    TCallBack iInactivity; //0x20
    TCallBack iActivity; //0x28
    TState iState; //0x30
};

#endif
