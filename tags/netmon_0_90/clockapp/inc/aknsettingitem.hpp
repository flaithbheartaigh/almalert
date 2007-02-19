/*
    aknsettingitem.hpp
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

#ifndef __AKNSETTINGITEM_HPP__
#define __AKNSETTINGITEM_HPP__

#include "aknsettingitemlistex.hpp"
#include <akncheckboxsettingpage.h>

class CAknFileSettingItem: public CAknSettingItem
{
  public:
    CAknFileSettingItem(TInt aIdentifier,TFileName& aText);
    ~CAknFileSettingItem();
    void StoreL(void);
    void LoadL(void);
    const TDesC& SettingTextL(void);
    void EditItemL(TBool aCalledFromMenu);
  private:
    TFileName& iExternalText;
    TFileName iInternalText;
};

class CSettingsControl;

class CAknVisibilitySettingItem: public CAknBinaryPopupSettingItem
{
  public:
    CAknVisibilitySettingItem(TInt aIdentifier,TBool& aBinaryValue,CAknSettingItemListEx& aList,RArray<TInt>& anItems);
    void EditItemL(TBool aCalledFromMenu);
    void UpdateVisibilityL(void);
  private:
    CAknSettingItemListEx& iList;
    RArray<TInt>& iItems;
};

class CAknTimeOffsetSettingItem: public CAknSettingItem
{
  public:
    CAknTimeOffsetSettingItem(TInt aIdentifier,TTimeIntervalSeconds& aInterval);
    ~CAknTimeOffsetSettingItem();
    void CompleteConstructionL(void);
    void StoreL(void);
    void LoadL(void);
    const TDesC& SettingTextL(void);
    void EditItemL(TBool aCalledFromMenu);
    void SetTimeFormatStringL(const TDesC& aTimeFormat);
    const TPtrC TimeFormatString(void);
  private:
    TTime Convert(TInt aInterval);
  private:
    TTimeIntervalSeconds& iExternalInterval;
    TTimeIntervalSeconds iInternalInterval;
    HBufC* iInternalText; // Owned
    TPtr iInternalTextPtr;
    HBufC* iTimeFormat;
    TPtr iTimeFormatPtr;
};

class CAknWorkDaysSettingItem: public CAknSettingItem
{
  public:
    CAknWorkDaysSettingItem(TInt aIdentifier,TUint& aWorkDays);
    ~CAknWorkDaysSettingItem();
    void CompleteConstructionL(void);
    void StoreL(void);
    void LoadL(void);
    const TDesC& SettingTextL(void);
    void EditItemL(TBool aCalledFromMenu);
  private:
    TUint& iExternalWorkDays;
    TBuf<64> iInternalText;
    CSelectionItemList* iList;
    CCoeEnv* iCoeEnv;
};

#endif
