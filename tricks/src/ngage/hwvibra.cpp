/*
    hwvibra.cpp
    Copyright (C) 2005-2006 zg

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
#include <zg_vibra.hpp>
#include <isi_units.hpp>

EXPORT_C void HWVibra::SwitchL(TBool aState)
{
  CVibraReq* send=CVibraReq::NewL(0,aState?1:0);
  CHWServer::SendL(send);
}

EXPORT_C void HWVibra::Reserved_1(void)
{
  User::Leave(KErrNotSupported);
}

EXPORT_C void HWVibra::SetIntensityL(TUint8 anIntensity)
{
  if(anIntensity>KMaxIntensity) User::Leave(KErrTooBig);
  CVibraIntensityReq* send=CVibraIntensityReq::NewL(0,anIntensity);
  CHWServer::SendL(send);
}

EXPORT_C void HWVibra::IntensityL(TUint8& anIntensity)
{
  TBuf<1> buffer;
  TRAPD(err,HWNetmon::ValueL(KPhoneAccessoryUnit,0x1c,buffer,HWNetmon::ERaw|HWNetmon::EExt));
  if(err==KErrNone&&buffer.Length()) anIntensity=buffer[0];
  else anIntensity=KDefaultIntensity;
}
