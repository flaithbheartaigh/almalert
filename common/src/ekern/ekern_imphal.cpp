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

#include <m32std61.hpp>
#include <k32std.h>

EXPORT_C void ImpHal61::UpdateLeds(TUint aLedMask)
{
}

EXPORT_C TInt ImpHal::StartupReason(TMachineStartupType& aReason) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt ImpHal::RamSize(void)
{
  return TheSuperPage().iMaxFreeRam;
}

EXPORT_C TUint ImpHal::DebugMask()
{
  return TheSuperPage().iDebugMask;
}

EXPORT_C void ImpHal::SetDebugMask(TUint32 aVal) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C TPtrC ImpHal61::MachineName(void)
{
  return _L("");
}

EXPORT_C TPtrC ImpHal61::DisplayName(void)
{
  return _L("");
}

EXPORT_C TPtrC ImpHal61::KeyboardName(void)
{
  return _L("");
}

EXPORT_C TPtrC ImpHal61::XYInputName(void)
{
  return _L("");
}

EXPORT_C TXYInputType ImpHal::XYInputType(void)
{
  return EXYInputDeltaMouse;
}

EXPORT_C TPtrC ImpHal61::PsuName(void)
{
  return _L("");
}

EXPORT_C TSize ImpHal::XYInputSizeInPixels(void)
{
  return TSize(0,0);
}

EXPORT_C TBool ImpHal61::IsBacklightPresent(void) //FIXME: NOT IMPLEMENTED
{
  return ETrue;
}

EXPORT_C TInt ImpHal61::BacklightCurrentInMilliAmps(void)
{
  return 0;
}

EXPORT_C TInt ImpHal61::KeyboardId(void)
{
  return 0;
}

EXPORT_C TInt ImpHal61::DisplayId(void) //FIXME: NOT IMPLEMENTED
{
  return 0; //0x59e00000[0x40];
}

EXPORT_C TInt ImpHal::PreemptionTicks(void)
{
  return 4;
}

EXPORT_C TSize ImpHal61::DisplaySizeInPixels(void) //FIXME: NOT IMPLEMENTED
{
  return TSize(0,0); //Variant::???
}

EXPORT_C TSize ImpHal61::PhysicalScreenSize(void) //FIXME: NOT IMPLEMENTED
{
  return TSize(0,0); //Variant::???
}

EXPORT_C TPoint ImpHal::OffsetToDisplayInPixels(void) //FIXME: NOT IMPLEMENTED
{
  return TPoint(0,0); //Variant::???
}

EXPORT_C TInt ImpHal::TickPeriod(TTimeIntervalMicroSeconds32& aPeriod) //FIXME: NOT IMPLEMENTED
{
  //Variant::???
  return KErrNone;
}

EXPORT_C TInt ImpHal61::MaxDisplayContrast(TInt& aValue) //FIXME: NOT IMPLEMENTED
{
  //Variant::???
  return KErrNone;
}

EXPORT_C TInt ImpHal61::SetDisplayContrast(TInt aValue) //FIXME: NOT IMPLEMENTED
{
  //Variant::???
  return KErrNone;
}

EXPORT_C TInt ImpHal61::DisplayContrast(TInt& aValue) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt ImpHal::PageSizeInBytes(TInt& aSize)
{
  aSize=0x1000;
  return KErrNone;
}

EXPORT_C TInt ImpHal::IdleCurrentInMilliAmps(void)
{
  return 0;
}

EXPORT_C TInt ImpHal::RunningCurrentInMilliAmps(void)
{
  return 0;
}

EXPORT_C TBool ImpHal::CaseClosed(void)
{
  return EFalse;
}

EXPORT_C TInt ImpHal61::CpuInfo(TDes8& aValue)
{
  return KErrNotSupported;
}

EXPORT_C TPtrC ImpHal61::ProcessorName(void)
{
  return _L("");
}

EXPORT_C TInt ImpHal::ProcessorClockInKHz(void) //FIXME: NOT IMPLEMENTED
{
  //Variant::???
  return 0;
}

EXPORT_C TInt ImpHal::SpeedFactor(void) //FIXME: NOT IMPLEMENTED
{
  //Variant::???
  return 0;
}

EXPORT_C TInt ImpHal61::MaximumDisplayColors(void)
{
  //Variant::???
  return 0;
}

EXPORT_C TInt ImpHal61::LanguageIndex(void)
{
  //Variant::???
  return 0;
}

EXPORT_C TInt ImpHal61::KeyboardIndex(void)
{
  //Variant::???
  return 0;
}

EXPORT_C TInt ImpHal61::AutoSwitchOffType(TAutoSwitchOffType& aValue)
{
  aValue=ECommon;
  return KErrNone;
}

EXPORT_C TBool ImpHal::TestBootSequence() //FIXME: NOT IMPLEMENTED
{
  //Variant::???
  return EFalse;
}

EXPORT_C TInt ImpHal61::SetMouse(TBool aState) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt ImpHal61::SetMouseButtonDown(TBool aState)
{
  return KErrNone;
}

EXPORT_C TInt ImpHal61::SetMouseSpeed(TUint8 aSpeed) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt ImpHal61::SetMouseAcceleration(TUint8 anAcceleration) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt ImpHal61::MouseOn(TBool& aState) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt ImpHal61::MouseProperties(TMouseProperties& aProperties) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

__declspec(naked) void ImpHal::DisableIrqs(void)
{
  __asm("mrs\tr1, cpsr");
  __asm("orr\tr1, r1, #128");
  __asm("msr\tcpsr_all, r1");
  __asm("mov\tpc, lr");
}

__declspec(naked) void ImpHal::EnableIrqs(void)
{
  __asm("mrs\tr1, cpsr");
  __asm("bic\tr1, r1, #128");
  __asm("msr\tcpsr_all, r1");
  __asm("mov\tpc, lr");
}

__declspec(naked) EXPORT_C TInt ImpHal::DisableIrqsToLevel1()
{
  __asm("mrs\tr1, cpsr");
  __asm("and\tr0, r1, #192");
  __asm("orr\tr1, r1, #128");
  __asm("msr\tcpsr_all, r1");
  __asm("mov\tpc, lr");
}

__declspec(naked) EXPORT_C TInt ImpHal::DisableIrqsToLevel2()
{
  __asm("mrs\tr1, cpsr");
  __asm("and\tr0, r1, #192");
  __asm("orr\tr1, r1, #192");
  __asm("msr\tcpsr_all, r1");
  __asm("mov\tpc, lr");
}

__declspec(naked) EXPORT_C void ImpHal::RestoreIrqs(TInt aLevel)
{
  __asm("mrs\tr1, cpsr");
  __asm("and\tr0, r0, #192");
  __asm("bic\tr1, r1, #192");
  __asm("orr\tr1, r1, r0");
  __asm("msr\tcpsr_all, r1");
  __asm("mov\tpc, lr");
}

EXPORT_C void ImpHal::UnlockRamDrive() //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void ImpHal::LockRamDrive() //FIXME: NOT IMPLEMENTED
{
}
