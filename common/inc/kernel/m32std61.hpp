/*
    m32std61.hpp
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

#ifndef __M32STD61_HPP__
#define __M32STD61_HPP__

class TProcessMemoryInfo; //don't exists in symbian 6.1
#include <m32std.h>
#include <e32hal61.hpp>

class ImpHal61
{
  public:
    class TMouseProperties
    {
      TUint8 iSpeed;
      TUint8 iAcceleration;
    };
  public:
    IMPORT_C static void UpdateLeds(TUint aLedMask);
    IMPORT_C static TPtrC MachineName(void);
    IMPORT_C static TPtrC DisplayName(void);
    IMPORT_C static TPtrC KeyboardName(void);
    IMPORT_C static TPtrC XYInputName(void);
    IMPORT_C static TPtrC PsuName(void);
    IMPORT_C static TBool IsBacklightPresent(void);
    IMPORT_C static TInt BacklightCurrentInMilliAmps(void);
    IMPORT_C static TInt KeyboardId(void);
    IMPORT_C static TInt DisplayId(void);
    IMPORT_C static TSize DisplaySizeInPixels(void);
    IMPORT_C static TSize PhysicalScreenSize(void);
    IMPORT_C static TInt MaxDisplayContrast(TInt& aValue);
    IMPORT_C static TInt SetDisplayContrast(TInt aValue);
    IMPORT_C static TInt DisplayContrast(TInt& aValue);
    IMPORT_C static TInt CpuInfo(TDes8& aValue);
    IMPORT_C static TPtrC ProcessorName(void);
    IMPORT_C static TInt MaximumDisplayColors(void);
    IMPORT_C static TInt LanguageIndex(void);
    IMPORT_C static TInt KeyboardIndex(void);
    IMPORT_C static TInt AutoSwitchOffType(TAutoSwitchOffType& aValue);
    IMPORT_C static TInt SetMouse(TBool aState);
    IMPORT_C static TInt SetMouseButtonDown(TBool aState);
    IMPORT_C static TInt SetMouseSpeed(TUint8 aSpeed);
    IMPORT_C static TInt SetMouseAcceleration(TUint8 anAcceleration);
    IMPORT_C static TInt MouseOn(TBool& aState);
    IMPORT_C static TInt MouseProperties(TMouseProperties& aProperties);
};

class Hal61
{
  public:
    IMPORT_C static TInt SoundInfo(TSoundInfoV1& aSoundInfo);
    IMPORT_C static TInt OnOffInfo(TOnOffInfoV1& aOnOffInfo);
    IMPORT_C static TInt SetKeyClickEnabled(TBool aValue);
    IMPORT_C static TInt SetKeyClickLoud(TBool aValue);
    IMPORT_C static TInt KeyClickEnabled(TBool& aValue);
    IMPORT_C static TInt KeyClickLoud(TBool& aValue);
    IMPORT_C static TInt SetPointerClickEnabled(TBool aValue);
    IMPORT_C static TInt SetPointerClickLoud(TBool aValue);
    IMPORT_C static TInt PointerClickEnabled(TBool& aValue);
    IMPORT_C static TInt PointerClickLoud(TBool& aValue);
    IMPORT_C static TInt SetBeepEnabled(TBool aValue);
    IMPORT_C static TInt SetBeepLoud(TBool aValue);
    IMPORT_C static TInt SetSoundDriverEnabled(TBool aValue);
    IMPORT_C static TInt SetSoundDriverLoud(TBool aValue);
    IMPORT_C static TInt SetSoundEnabled(TBool aValue);
    IMPORT_C static TInt ModifyLedMask(TUint aSetMask,TUint aClearMask);
    IMPORT_C static TInt SetBacklightOn(TBool aValue);
    IMPORT_C static TInt BacklightOn(TBool& aValue);
    IMPORT_C static TInt BacklightBehavior(TBacklightBehavior& aValue);
    IMPORT_C static TInt SetBacklightBehavior(TBacklightBehavior aValue);
    IMPORT_C static TInt BacklightOnTime(TTimeIntervalSeconds& aValue);
    IMPORT_C static TInt SetBacklightOnTime(TTimeIntervalSeconds aValue);
    IMPORT_C static TInt AutoSwitchOffBehavior(TAutoSwitchOffBehavior& aValue);
    IMPORT_C static TInt SetAutoSwitchOffBehavior(TAutoSwitchOffBehavior aValue);
    IMPORT_C static TInt AutoSwitchOffTime(TTimeIntervalSeconds& aValue);
    IMPORT_C static TInt SetAutoSwitchOffTime(TTimeIntervalSeconds aValue);
    IMPORT_C static TInt SetPointerSwitchesOn(TBool aValue);
    IMPORT_C static TInt PointerSwitchesOn(TBool& aValue);
    IMPORT_C static TInt SetCaseOpenSwitchesOn(TBool aValue);
    IMPORT_C static TInt CaseOpenSwitchesOn(TBool& aValue);
    IMPORT_C static TInt SetCaseCloseSwitchesOff(TBool aValue);
    IMPORT_C static TInt CaseCloseSwitchesOff(TBool& aValue);
    IMPORT_C static TInfoName XYInputName(void);
    IMPORT_C static TInfoName KeyboardName(void);
    IMPORT_C static TInfoName PsuName(void);
    IMPORT_C static TInfoName DisplayName(void);
    IMPORT_C static TInfoName MachineName(void);
    IMPORT_C static TBool BacklightPresent(void);
    IMPORT_C static TBool KeyboardPresent(void);
    IMPORT_C static TSize DisplaySizeInPixels(void);
    IMPORT_C static TSize PhysicalScreenSize(void);
    IMPORT_C static TInt KeyboardId(void);
    IMPORT_C static TInt DisplayId(void);
    IMPORT_C static TInt64 MachineUniqueId(void);
    IMPORT_C static TUint LedCapabilities(void);
    IMPORT_C static void ResetAutoSwitchOffTimer(void);
    IMPORT_C static void ResetBacklightTimer(void);
};

#endif
