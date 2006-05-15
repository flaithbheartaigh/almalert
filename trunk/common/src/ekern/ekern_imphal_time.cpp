/*
    ekern_imphal_time.cpp
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
#include <k32std61.hpp>

EXPORT_C void ImpHal::Idle(void)
{
}

//*****

EXPORT_C TTimerLockSpec Plat::LockPeriod(void)
{
  TUint lock=(K::TickCounter&0x3f)*3/16;
  return lock?(TTimerLockSpec)(lock-1):ETwelveOClock;
}

TInt P::MicroSecondsToTicks(TInt aMicroSeconds)
{
  return aMicroSeconds/15625;
}

TUint P::RoundTicksToSecond(TUint aTicks)
{
  return aTicks&~0x3f;
}

const TUint LockTicks[]={5,11,16,21,27,32,37,43,48,53,59,64};

TUint P::TimerLockSpecToTicks(TTimerLockSpec aLock)
{
  return LockTicks[aLock];
}

EXPORT_C void Plat61::Beep(TInt aValue)
{
}

//*****

void P61::Wait(TInt aValue)
{
  __asm("mov\tr1, #0");
  __asm("L_wait:");
  __asm("add\tr1, r1, #1");
  __asm("nop");
  __asm("nop");
  __asm("nop");
  __asm("cmp\tr0, r1");
  __asm("nop");
  __asm("nop");
  __asm("nop");
  __asm("bne\tL_wait");
  __asm("mov\tpc, lr");
}

EXPORT_C void Plat::WaitAtLeastAMicroSecond(void)
{
  P61::Wait(8);
}

void P61::KeyClick(void)
{
}

void P61::PointerClick(void)
{
}
