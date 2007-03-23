/*
    settingsclient.hpp
    Copyright (C) 2007 zg

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

#ifndef __SETTINGSCLIENT_HPP__
#define __SETTINGSCLIENT_HPP__

#include <AlmSettingsClient.hpp>
#include <e32base.h>

class CSettingsClient: public CBase
{
  public:
    static CSettingsClient* NewL(void);
    ~CSettingsClient();
  public:
    void CompactL(void);
    void LoadSettingL(const TDesC& aCategory,const TDesC& aName,TFileName& aValue);
    void LoadSettingL(const TDesC& aCategory,const TDesC& aName,TInt& aValue,TInt aLow,TInt aHigh,TInt aDefault);
    void LoadSettingL(const TDesC& aCategory,const TDesC& aNameHigh,const TDesC& aNameLow,TInt64& aValue);
    void StoreSettingL(const TDesC& aCategory,const TDesC& aName,const TFileName& aValue);
    void StoreSettingL(const TDesC& aCategory,const TDesC& aName,const TInt& aValue);
    void StoreSettingL(const TDesC& aCategory,const TDesC& aNameHigh,const TDesC& aNameLow,const TInt64& aValue);
  private:
    CSettingsClient();
    void ConstructL(void);
  private:
    RAlmSettings iSettings;
};

#endif
