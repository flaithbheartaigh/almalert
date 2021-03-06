/*
    ekern_logicaldevice.cpp
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

class TProcessMemoryInfo; //don't exists in symbian 6.1
#include <k32std.h>
#include <e32std.h>

EXPORT_C DLogicalDevice::DLogicalDevice()
{
}

EXPORT_C DLogicalDevice::~DLogicalDevice()
{
}

EXPORT_C TInt DLogicalDevice::Remove()
{
  return KErrNone;
}

EXPORT_C TBool DLogicalDevice::QueryVersionSupported(const TVersion& aVer) const
{
  return User::QueryVersionSupported(iVersion,aVer);
}

EXPORT_C TBool DLogicalDevice::IsAvailable(TInt aUnit,const TDesC* aDriver,const TDesC* anInfo) const
{
  return ETrue;
}
