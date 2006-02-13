/*
    ekern_imphal.cpp
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
#include <m32std.h>

EXPORT_C TUint ImpHal::DebugMask() //FIXME: NOT IMPLEMENTED
{
  return 0;
}

EXPORT_C void ImpHal::SetDebugMask(TUint32 aVal) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C TBool ImpHal::TestBootSequence() //FIXME: NOT IMPLEMENTED
{
  return EFalse;
}

EXPORT_C TInt ImpHal::DisableIrqsToLevel1() //FIXME: NOT IMPLEMENTED
{
  return 0;
}

EXPORT_C TInt ImpHal::DisableIrqsToLevel2() //FIXME: NOT IMPLEMENTED
{
  return 0;
}

EXPORT_C void ImpHal::RestoreIrqs(TInt aLevel) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void ImpHal::UnlockRamDrive() //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void ImpHal::LockRamDrive() //FIXME: NOT IMPLEMENTED
{
}
