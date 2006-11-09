/*
    AlmSettingsClientImplementation.hpp
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

#ifndef __ALMSETTINGSCLIENTIMPLEMENTATION_HPP__
#define __ALMSETTINGSCLIENTIMPLEMENTATION_HPP__

const TUint KDefaultMessageSlots=4;

RAlmSettings::RAlmSettings()
{
}

TInt RAlmSettings::Connect(void)
{
  return CreateSession(KSettingsServerName,TVersion(KSettingsServerMajorVersionNumber,KSettingsServerMinorVersionNumber,KSettingsServerBuildVersionNumber),KDefaultMessageSlots);
}

TInt RAlmSettings::Size(const TDesC& aCategory,const TDesC& aName,TInt& aSize)
{
  TAny* p[KMaxMessageArguments];
  TPtrC8 category((const TUint8*)aCategory.Ptr(),aCategory.Length()*2);
  TPtrC8 name((const TUint8*)aName.Ptr(),aName.Length()*2);
  TPckg<TInt> size(aSize);
  p[0]=(TAny*)(&category);
  p[1]=(TAny*)(&name);
  p[2]=(TAny*)(&size);
  return SendReceive(ESettingsServerRequestGetLength,&p[0]);
}

TInt RAlmSettings::Get(const TDesC& aCategory,const TDesC& aName,TDes8& aValue)
{
  TAny* p[KMaxMessageArguments];
  TPtrC8 category((const TUint8*)aCategory.Ptr(),aCategory.Length()*2);
  TPtrC8 name((const TUint8*)aName.Ptr(),aName.Length()*2);
  p[0]=(TAny*)(&category);
  p[1]=(TAny*)(&name);
  p[2]=(TAny*)(&aValue);
  return SendReceive(ESettingsServerRequestGetData,&p[0]);
}

TInt RAlmSettings::Get(const TDesC& aCategory,const TDesC& aName,TDes& aValue)
{
  TAny* p[KMaxMessageArguments];
  TPtrC8 category((const TUint8*)aCategory.Ptr(),aCategory.Length()*2);
  TPtrC8 name((const TUint8*)aName.Ptr(),aName.Length()*2);
  TPtr8 value((TUint8*)aValue.Ptr(),aValue.MaxLength()*2);
  p[0]=(TAny*)(&category);
  p[1]=(TAny*)(&name);
  p[2]=(TAny*)(&value);
  TInt res=SendReceive(ESettingsServerRequestGetData,&p[0]);
  if(res==KErrNone) aValue.SetLength(value.Length()/2);
  return res;
}

TInt RAlmSettings::Get(const TDesC& aCategory,const TDesC& aName,TUint32& aValue)
{
  TAny* p[KMaxMessageArguments];
  TPtrC8 category((const TUint8*)aCategory.Ptr(),aCategory.Length()*2);
  TPtrC8 name((const TUint8*)aName.Ptr(),aName.Length()*2);
  TPckg<TUint32> value(aValue);
  p[0]=(TAny*)(&category);
  p[1]=(TAny*)(&name);
  p[2]=(TAny*)(&value);
  return SendReceive(ESettingsServerRequestGetData,&p[0]);
}

TInt RAlmSettings::Set(const TDesC& aCategory,const TDesC& aName,const TDesC8& aValue)
{
  TAny* p[KMaxMessageArguments];
  TPtrC8 category((const TUint8*)aCategory.Ptr(),aCategory.Length()*2);
  TPtrC8 name((const TUint8*)aName.Ptr(),aName.Length()*2);
  p[0]=(TAny*)(&category);
  p[1]=(TAny*)(&name);
  p[2]=(TAny*)(&aValue);
  return SendReceive(ESettingsServerRequestSet,&p[0]);
}

TInt RAlmSettings::Set(const TDesC& aCategory,const TDesC& aName,const TDesC& aValue)
{
  TAny* p[KMaxMessageArguments];
  TPtrC8 category((const TUint8*)aCategory.Ptr(),aCategory.Length()*2);
  TPtrC8 name((const TUint8*)aName.Ptr(),aName.Length()*2);
  TPtrC8 value((const TUint8*)aValue.Ptr(),aValue.Length()*2);
  p[0]=(TAny*)(&category);
  p[1]=(TAny*)(&name);
  p[2]=(TAny*)(&value);
  return SendReceive(ESettingsServerRequestSet,&p[0]);
}

TInt RAlmSettings::Set(const TDesC& aCategory,const TDesC& aName,const TUint32& aValue)
{
  TAny* p[KMaxMessageArguments];
  TPtrC8 category((const TUint8*)aCategory.Ptr(),aCategory.Length()*2);
  TPtrC8 name((const TUint8*)aName.Ptr(),aName.Length()*2);
  TPckgC<TUint32> value(aValue);
  p[0]=(TAny*)(&category);
  p[1]=(TAny*)(&name);
  p[2]=(TAny*)(&value);
  return SendReceive(ESettingsServerRequestSet,&p[0]);
}

#endif
