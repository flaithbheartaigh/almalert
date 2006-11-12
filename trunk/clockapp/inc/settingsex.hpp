/*
    settingsex.hpp
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

#ifndef __SETTINGSEX_HPP__
#define __SETTINGSEX_HPP__

#include <aknview.h>
#include <AlmSettingsClient.hpp>
#include "aknsettingitemlistex.hpp"

class CSettingsControl;
class CSettingsView: public CAknView
{
  public:
    static CSettingsView* NewLC(void);
    ~CSettingsView();
  private:
    void ConstructL(void);
  public: //CAknView
    TUid Id(void) const;
    void HandleCommandL(TInt aCommand);
    void HandleClientRectChange(void);
  private: //CAknView
    void DoActivateL(const TVwsViewId& aPrevViewId,TUid aCustomMessageId,const TDesC8& aCustomMessage);
    void DoDeactivate(void);
  private:
    CSettingsControl* iControl;
};

class CSettingsControl: public CAknSettingItemListEx
{
  public:
    static CSettingsControl* NewL(const TRect& aRect);
    ~CSettingsControl();
  protected: //CAknSettingItemList
    CAknSettingItem* CreateSettingItemL(TInt aSettingId);
  public: //CAknSettingItemList
    void StoreSettingsL(void);
  private:
    CSettingsControl(void);
    void ConstructL(const TRect& aRect);
    void LoadSettingL(const TDesC& aCategory,const TDesC& aName,TFileName& aValue);
    void LoadSettingL(const TDesC& aCategory,const TDesC& aName,TInt& aValue,TInt aLow,TInt aHigh,TInt aDefault);
    void StoreSettingL(const TDesC& aCategory,const TDesC& aName,const TFileName& aValue);
    void StoreSettingL(const TDesC& aCategory,const TDesC& aName,const TInt& aValue);
    void UpdateVisibility(void);
  private:
    RAlmSettings iSettings;
    TFileName iAlarmTone;
    TInt iSnoozeTime;
    TInt iSnoozeCount;
    TFileName iCalendarTone;
    RArray<TInt> iBeepArray;
    TInt iBeep;
    TFileName iBeepTone;
    TInt iBeepStart;
    TInt iBeepFinish;
    RArray<TInt> iBirthdayArray;
    TInt iBirthday;
    TFileName iBirthdayTone;
    TInt iBirthdayStart;
    TInt iBirthdayHour;
};

#endif
