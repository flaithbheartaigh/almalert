/*
    ekern_imppsu.cpp
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

#include <m32std61.hpp>

void ImpPsu::Init1(void) //FIXME: NOT IMPLEMENTED
{
}

void ImpPsu::Init3(void)
{
}

void ImpPsu61::BacklightOn(void) //FIXME: NOT IMPLEMENTED
{
  //Variant::???
}

void ImpPsu61::BacklightOff(void) //FIXME: NOT IMPLEMENTED
{
  //Variant::???
}

void ImpPsu61::DisplayOn(void) //FIXME: NOT IMPLEMENTED
{
  //Variant::???
}

TBool ImpPsu::ExternalPowerPresent(void) //FIXME: NOT IMPLEMENTED
{
  return ETrue;
}

TInt ImpPsu::MainBatteryMaxMilliVolts(void)
{
  return 0;
}

TInt ImpPsu::BackupBatteryMaxMilliVolts(void)
{
  return 0;
}

TSupplyStatus ImpPsu::MainBatteryStatus(void) //FIXME: NOT IMPLEMENTED
{
  return EGood;
}

TInt ImpPsu::MainBatteryMilliVolts(void) //FIXME: NOT IMPLEMENTED
{
  return 0;
}

TSupplyStatus ImpPsu::BackupBatteryStatus(void) //FIXME: NOT IMPLEMENTED
{
  return EGood;
}

TInt ImpPsu::BackupBatteryMilliVolts(void) //FIXME: NOT IMPLEMENTED
{
  return 0;
}

void ImpPsu::CheckPowerSupplies(void) //FIXME: NOT IMPLEMENTED
{
}
