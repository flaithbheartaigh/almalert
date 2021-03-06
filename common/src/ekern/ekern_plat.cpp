/*
    ekern_plat.cpp
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
#include <m32std.h>
#include <e32std.h>

EXPORT_C void Plat::DebugInit() //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void Plat::ScreenInfo(TDes8 &anInfo) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C TUint Plat::DebugMask() //FIXME: NOT IMPLEMENTED
{
  return 0;
}

EXPORT_C void Plat::SetDebugger(DDebugger *aDebugger) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C DDebugger* Plat::Debugger() //FIXME: NOT IMPLEMENTED
{
  return NULL;
}

EXPORT_C TInt Plat::NullThread(TAny *aPtr) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

//*****

void P::BlockThreads(TBlockType aType) //FIXME: NOT IMPLEMENTED
{
}

void P::WsRegisterThread(TThreadFunction aFunction) //FIXME: NOT IMPLEMENTED
{
}

void P::WsRegisterSwitchOnScreenHandling(TBool aState) //FIXME: NOT IMPLEMENTED
{
}

void P::WsSwitchOnScreen() //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void Plat::OsSwitchOnScreen() //FIXME: NOT IMPLEMENTED
{
}

TInt P::HalPageSize(void)
{
  return ImpMmu::PageSizeInBytes;
}

void PP::Panic(TPlatPanic aPanic)
{
  Plat::Panic(_L("PLAT"),aPanic);
}

void PP::Fault(TPlatFault aFault)
{
  Plat::Fault(_L("PLAT"),aFault);
}

EXPORT_C TUint Plat::TickCount()
{
  return K::TickCounter;
}

EXPORT_C TTime Plat::SystemTime() //FIXME: NOT IMPLEMENTED
{
  TTime time;
  return time;
}

EXPORT_C TTimeIntervalSeconds Plat::SystemTimeInSecondsFrom2000() //FIXME: NOT IMPLEMENTED
{
  return TTimeIntervalSeconds(0);
}

EXPORT_C TInt Plat::SetSystemTime(const TTime& aTime) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C void Plat::EnterCS() //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void Plat::LeaveCS() //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C DThread& Plat::CurrentThread() //FIXME: NOT IMPLEMENTED
{
  return *((DThread*)0);
}

EXPORT_C DProcess& Plat::CurrentProcess() //FIXME: NOT IMPLEMENTED
{
  return *((DProcess*)0);
}

EXPORT_C void Plat::Panic(const TDesC &aName,TInt aPanicNumber)
{
  Fault(aName,aPanicNumber+0x10000);
}

EXPORT_C void Plat::Fault(const TDesC &aName,TInt aFaultNumber) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void Plat::Reset(TMachineStartupType aType)
{
  PP::Fault(PP::ESoftwareWarmReset);
}
