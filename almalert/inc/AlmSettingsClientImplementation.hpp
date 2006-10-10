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

TInt RAlmSettings::Size(const TDesC& aName,TInt& aSize)
{
  TAny* p[KMaxMessageArguments];
  TPtrC8 name((const TUint8*)aName.Ptr(),aName.Length()*2);
  TPckgC<TInt> size(aSize);
  p[0]=(TAny*)(&name);
  p[1]=(TAny*)(&size);
  return SendReceive(ESettingsServerRequestGetData,&p[0]);
}

TInt RAlmSettings::Get(const TDesC& aName,TDes8& aValue)
{
  TAny* p[KMaxMessageArguments];
  TPtrC8 name((const TUint8*)aName.Ptr(),aName.Length()*2);
  p[0]=(TAny*)(&name);
  p[1]=(TAny*)(&aValue);
  return SendReceive(ESettingsServerRequestGetData,&p[0]);
}

TInt RAlmSettings::Set(const TDesC& aName,const TDesC8& aValue)
{
  TAny* p[KMaxMessageArguments];
  TPtrC8 name((const TUint8*)aName.Ptr(),aName.Length()*2);
  p[0]=(TAny*)(&name);
  p[1]=(TAny*)(&aValue);
  return SendReceive(ESettingsServerRequestSet,&p[0]);
}

#endif
