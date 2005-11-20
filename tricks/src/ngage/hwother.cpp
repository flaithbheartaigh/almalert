/*
    hwother.cpp
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

#include "hwtricks.hpp"
#include <hal.h>

EXPORT_C void HWOther::InfoL(TInfoType aType,TInt& aValue)
{
  switch(aType)
  {
    case EInfoBuild:
      aValue=9;
      break;
    case EInfoServerSupported:
      aValue=ETrue;
      break;
    case EInfoVibraSupported:
      aValue=ETrue;
      break;
    case EInfoSwitchLightsSupported:
      aValue=ETrue;
      break;
    case EInfoBrightnessSupported:
      aValue=ETrue;
      break;
    case EInfoContrastSupported:
      {
        TInt machine;
        User::LeaveIfError(HAL::Get(HALData::EModel,machine));
        if(machine==0x101F8C19) aValue=ETrue;
        else aValue=EFalse;
      }
      break;
    case EInfoNetmonSupported:
      aValue=ETrue;
      break;
    default:
      User::Leave(KErrNotSupported);
      break;
  }
}
