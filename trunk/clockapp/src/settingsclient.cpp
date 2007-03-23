/*
    settingsclient.cpp
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

#include "settingsclient.hpp"
#include <AlmSettingsCommon.hpp>
#include <AlmSettingsClientImplementation.hpp>

CSettingsClient* CSettingsClient::NewL(void)
{
  CSettingsClient* self=new(ELeave)CSettingsClient;
  CleanupStack::PushL(self);
  self->ConstructL();
  CleanupStack::Pop(); //self
  return self;
}

CSettingsClient::~CSettingsClient()
{
  iSettings.Close();
}

CSettingsClient::CSettingsClient(): CBase()
{
}

void CSettingsClient::ConstructL(void)
{
  User::LeaveIfError(iSettings.Connect());
}

void CSettingsClient::CompactL(void)
{
  User::LeaveIfError(iSettings.Compact());
}

void CSettingsClient::LoadSettingL(const TDesC& aCategory,const TDesC& aName,TFileName& aValue)
{
  TInt err=iSettings.Get(aCategory,aName,aValue);
  if(err!=KErrNotFound) User::LeaveIfError(err);
}

void CSettingsClient::LoadSettingL(const TDesC& aCategory,const TDesC& aName,TInt& aValue,TInt aLow,TInt aHigh,TInt aDefault)
{
  TInt err=iSettings.Get(aCategory,aName,aValue);
  if(err!=KErrNotFound) User::LeaveIfError(err);
  if(aValue<aLow||aValue>aHigh) aValue=aDefault;
}

void CSettingsClient::LoadSettingL(const TDesC& aCategory,const TDesC& aNameHigh,const TDesC& aNameLow,TInt64& aValue)
{
  TInt high,low;
  TInt err=iSettings.Get(aCategory,aNameHigh,high);
  if(err!=KErrNotFound) User::LeaveIfError(err);
  if(err==KErrNone) err=iSettings.Get(aCategory,aNameLow,low);
  if(err!=KErrNotFound) User::LeaveIfError(err);
  if(err==KErrNone)
    aValue.Set(high,low);
  else
    aValue.Set(6,0xb49d2000);
}

void CSettingsClient::StoreSettingL(const TDesC& aCategory,const TDesC& aName,const TFileName& aValue)
{
  TFileName old;
  TInt err=iSettings.Get(aCategory,aName,old);
  if(err!=KErrNotFound) User::LeaveIfError(err);
  if(err==KErrNotFound||old.CompareF(aValue)) User::LeaveIfError(iSettings.Set(aCategory,aName,aValue));
}

void CSettingsClient::StoreSettingL(const TDesC& aCategory,const TDesC& aName,const TInt& aValue)
{
  TInt old;
  TInt err=iSettings.Get(aCategory,aName,old);
  if(err!=KErrNotFound) User::LeaveIfError(err);
  if(err==KErrNotFound||old!=aValue) User::LeaveIfError(iSettings.Set(aCategory,aName,aValue));
}

void CSettingsClient::StoreSettingL(const TDesC& aCategory,const TDesC& aNameHigh,const TDesC& aNameLow,const TInt64& aValue)
{
  TInt old;
  TInt err=iSettings.Get(aCategory,aNameHigh,old);
  if(err!=KErrNotFound) User::LeaveIfError(err);
  if(err==KErrNotFound||(TUint)old!=aValue.High()) User::LeaveIfError(iSettings.Set(aCategory,aNameHigh,aValue.High()));
  err=iSettings.Get(aCategory,aNameLow,old);
  if(err!=KErrNotFound) User::LeaveIfError(err);
  if(err==KErrNotFound||(TUint)old!=aValue.Low()) User::LeaveIfError(iSettings.Set(aCategory,aNameLow,aValue.Low()));
}
