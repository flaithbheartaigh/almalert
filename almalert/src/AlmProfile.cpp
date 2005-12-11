/*
    AlmProfile.cpp
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

#include "AlmProfile.hpp"
#include <ProfileApi.hpp>

void AlmProfile::SoundParamsL(TInt& aRingType,TInt& aRingVolume)
{
  CProfileAPI* profiles=CProfileAPI::NewLC();
  CArrayFixFlat<TContactItemId>* alertFor=new(ELeave)CArrayFixFlat<long>(5);
  CleanupStack::PushL(alertFor);
  TFileName ringingTone,sms;
  TInt ringType,ringVolume,vibra,keypadTones,alertForCount;
  if(profiles->GetProfileMultiData(ringingTone,sms,ringType,ringVolume,vibra,keypadTones,alertFor,alertForCount)!=CProfileAPI::EProSuccess) User::Leave(KErrUnknown);
  aRingType=ringType;
  aRingVolume=ringVolume;
  CleanupStack::PopAndDestroy(2); //alertFor,profiles
}
