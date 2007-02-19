/*
    ekern_hal.cpp
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

TInt P::HalFunction(THalFunction aFunction,TAny* aData) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal::MachineInfo(TMachineInfoV1& aMachineInfo) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal::MachineInfo(TMachineInfoV2& aMachineInfo)
{
  MachineInfo(static_cast<TMachineInfoV1&>(aMachineInfo));
  aMachineInfo.iLanguageIndex=ImpHal61::LanguageIndex();
  aMachineInfo.iKeyboardIndex=ImpHal61::KeyboardIndex();
  return KErrNone;
}

EXPORT_C TInt Hal::MemoryInfo(TMemoryInfoV1& aMemoryInfo) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal::SupplyInfo(TSupplyInfoV1& aSupplyInfo) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal61::SoundInfo(TSoundInfoV1& aSoundInfo) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal61::OnOffInfo(TOnOffInfoV1& aOnOffInfo) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal::DriveInfo(TDriveInfoV1& aDriveInfo) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal61::SetKeyClickEnabled(TBool aValue) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal61::SetKeyClickLoud(TBool aValue) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal61::KeyClickEnabled(TBool& aValue) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal61::KeyClickLoud(TBool& aValue) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal61::SetPointerClickEnabled(TBool aValue) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal61::SetPointerClickLoud(TBool aValue) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal61::PointerClickEnabled(TBool& aValue) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal61::PointerClickLoud(TBool& aValue) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal61::SetBeepEnabled(TBool aValue) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal61::SetBeepLoud(TBool aValue) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal61::SetSoundDriverEnabled(TBool aValue) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal61::SetSoundDriverLoud(TBool aValue) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal61::SetSoundEnabled(TBool aValue) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal::CalibrationPoints(TDigitizerCalibration& aCalibration) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal::SetXYInputCalibration(const TDigitizerCalibration& aCalibration) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal61::ModifyLedMask(TUint aSetMask,TUint aClearMask) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal61::SetBacklightOn(TBool aValue) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal61::BacklightOn(TBool& aValue) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal61::BacklightBehavior(TBacklightBehavior& aValue) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal61::SetBacklightBehavior(TBacklightBehavior aValue) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal61::BacklightOnTime(TTimeIntervalSeconds& aValue) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal61::SetBacklightOnTime(TTimeIntervalSeconds aValue) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal61::AutoSwitchOffBehavior(TAutoSwitchOffBehavior& aValue) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal61::SetAutoSwitchOffBehavior(TAutoSwitchOffBehavior aValue) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal61::AutoSwitchOffTime(TTimeIntervalSeconds& aValue) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal61::SetAutoSwitchOffTime(TTimeIntervalSeconds aValue) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal61::SetPointerSwitchesOn(TBool aValue) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal61::PointerSwitchesOn(TBool& aValue) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal61::SetCaseOpenSwitchesOn(TBool aValue) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal61::CaseOpenSwitchesOn(TBool& aValue) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal61::SetCaseCloseSwitchesOff(TBool aValue) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal61::CaseCloseSwitchesOff(TBool& aValue) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal::FaultReason(TInt& aReason) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal::ExceptionId(TInt &anId) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Hal::ExceptionInfo(TExcInfo &aReason) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInfoName Hal61::XYInputName(void)
{
  return ImpHal61::XYInputName();
}

EXPORT_C TInfoName Hal61::KeyboardName(void)
{
  return ImpHal61::KeyboardName();
}

EXPORT_C TInfoName Hal61::PsuName(void)
{
  return ImpHal61::PsuName();
}

EXPORT_C TInfoName Hal61::DisplayName(void)
{
  return ImpHal61::DisplayName();
}

EXPORT_C TInfoName Hal61::MachineName(void)
{
  return ImpHal61::MachineName();
}

EXPORT_C TBool Hal61::BacklightPresent(void)
{
  return ImpHal61::IsBacklightPresent();
}

EXPORT_C TInt Hal::TotalRamInBytes(void)
{
  return TheSuperPage().iTotalRamSize;
}

EXPORT_C TInt Hal::TotalRomInBytes(void)
{
  return TheSuperPage().iTotalRomSize;
}

EXPORT_C TInt Hal::MaxFreeRamInBytes(void)
{
  return TheSuperPage().iMaxFreeRam;
}

EXPORT_C TInt Hal::FreeRamInBytes(void) //FIXME: NOT IMPLEMENTED
{
  return 0;
}

EXPORT_C TInt Hal::InternalDiskRamInBytes(void)
{
  return TheSuperPage().iRamDriveSize;
}

EXPORT_C TBool Hal::RomIsReprogrammable(void)
{
  return ETrue;
}

EXPORT_C TVersion Hal::RomVersion(void)
{
  return TheRomHeader().iVersion;
}

EXPORT_C TXYInputType Hal::XYInputType(void)
{
  return ImpHal::XYInputType();
}

EXPORT_C TBool Hal61::KeyboardPresent(void)
{
  return ETrue;
}

EXPORT_C TSize Hal61::DisplaySizeInPixels(void)
{
  return ImpHal61::DisplaySizeInPixels();
}

EXPORT_C TSize Hal::XYInputSizeInPixels(void)
{
  return ImpHal::XYInputSizeInPixels();
}

EXPORT_C TSize Hal61::PhysicalScreenSize(void)
{
  return ImpHal61::PhysicalScreenSize();
}

EXPORT_C TPoint Hal::OffsetToDisplayInPixels(void)
{
  return ImpHal::OffsetToDisplayInPixels();
}

EXPORT_C TInt Hal61::KeyboardId(void)
{
  return ImpHal61::KeyboardId();
}

EXPORT_C TInt Hal61::DisplayId(void)
{
  return ImpHal61::DisplayId();
}

EXPORT_C TInt64 Hal61::MachineUniqueId(void) //FIXME: NOT IMPLEMENTED
{
  return TInt64(0);
}

EXPORT_C TUint Hal61::LedCapabilities(void)
{
  return 0;
}

EXPORT_C TSupplyStatus Hal::MainBatteryStatus(void) //FIXME: NOT IMPLEMENTED
{
  return EZero;
}

EXPORT_C TInt Hal::MainBatteryMilliVolts(void) //FIXME: NOT IMPLEMENTED
{
  return 0;
}

EXPORT_C TInt Hal::MainBatteryMaxMilliVolts(void) //FIXME: NOT IMPLEMENTED
{
  return 0;
}

EXPORT_C TInt Hal::BackupBatteryMaxMilliVolts(void) //FIXME: NOT IMPLEMENTED
{
  return 0;
}

EXPORT_C TSupplyStatus Hal::BackupBatteryStatus(void) //FIXME: NOT IMPLEMENTED
{
  return EZero;
}

EXPORT_C TInt Hal::BackupBatteryMilliVolts(void) //FIXME: NOT IMPLEMENTED
{
  return 0;
}

EXPORT_C TBool Hal::ExternalPowerPresent(void) //FIXME: NOT IMPLEMENTED
{
  return EFalse;
}

EXPORT_C TUint Hal::Flags(void)
{
  return 0;
}

EXPORT_C TInt Hal::TickPeriod(TTimeIntervalMicroSeconds32& aPeriod)
{
  return ImpHal::TickPeriod(aPeriod);
}

EXPORT_C void Hal61::ResetAutoSwitchOffTimer(void) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void Hal61::ResetBacklightTimer(void) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void Hal::MapRom(TLinAddr anAddrL,TPhyAddr anAddrP,TInt aMapSize,TInt aRealSize)
{
}

EXPORT_C TCpuTime Hal::DfcRescheduleCpuTime() //FIXME: NOT IMPLEMENTED
{
  return 0;
}

void Hal::Init1(void) //FIXME: NOT IMPLEMENTED
{
}

void Hal::Init3(void) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C TBool Hal::ColdStart(void) //FIXME: NOT IMPLEMENTED
{
  return EFalse;
}


EXPORT_C TInt Hal::AddProtected(TInt &aCounter,TInt aValue)
{
  TInt level=ImpHal::DisableIrqsToLevel2();
  TInt res=aCounter+aValue;
  aCounter=res;
  ImpHal::RestoreIrqs(level);
  return res;
}

static TInt WaitCallBack(TAny* aPtr)
{
  *(TBool*)aPtr=ETrue;
  return KErrNone;
}

EXPORT_C void Hal::WaitMilliSeconds(TInt aMilliSeconds) //FIXME: NOT IMPLEMENTED
{
  (void)&WaitCallBack;
}
