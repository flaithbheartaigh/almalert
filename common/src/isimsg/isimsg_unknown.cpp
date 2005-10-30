/*
    isimsg_unknown.cpp
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

#include <isimsg.hpp>

EXPORT_C TUint8 CIsiMsg976::GetParam10(void)
{
  return iPtr[10];
}

EXPORT_C TUint8 CIsiMsg976::GetParam10A(void)
{
  return iPtr[10];
}

EXPORT_C TUint8 CIsiMsg976::GetParam10B(void)
{
  return iPtr[10];
}

EXPORT_C TUint8 CIsiMsg976::GetParam10C(void)
{
  return iPtr[10];
}

EXPORT_C CIsiMsg802* CIsiMsg802::NewL(TUint8 aParam1,TUint8 aParam2)
{
  return NULL;
}

EXPORT_C CIsiMsg848* CIsiMsg848::NewL(TUint8 aParam1,TUint8 aParam2)
{
  return NULL;
}

EXPORT_C CIsiMsg850* CIsiMsg850::NewL(TUint8 aParam1,TUint8 aParam2,TUint8 aParam3,TUint8 aParam4)
{
  return NULL;
}

EXPORT_C CIsiMsg649* CIsiMsg649::NewL(TUint8 aParam1,TUint8 aParam2,TUint8 aParam3)
{
  return NULL;
}

EXPORT_C CIsiMsg691* CIsiMsg691::NewL(TUint8 aParam1,TUint8 aParam2,TUint8 aParam3,CIsiMsg649* aMsg)
{
  return NULL;
}

EXPORT_C void* IsiMsg900(TUint8 aTransactionId,TUint8 aParam1,TUint8 aParam2,TUint8 aParam3)
{
  return NULL;
}

EXPORT_C void* IsiMsg970(TUint8 aTransactionId,TUint8 aParam1,TUint8 aParam2,TUint8 aParam3,const TDesC16& aParam4)
{
  return NULL;
}

EXPORT_C void* IsiMsg971(TUint8 aTransactionId,TUint8 aParam1,TUint8 aParam2,TUint8 aParam3,const TDesC16& aParam4)
{
  return NULL;
}

EXPORT_C void* IsiMsg972(TUint8 aTransactionId,TUint8 aParam1,TUint8 aParam2,TUint8 aParam3,TUint8 aParam4,TUint8 aParam5,const TDesC8& aParam6)
{
  return NULL;
}
