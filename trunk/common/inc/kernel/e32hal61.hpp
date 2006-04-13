/*
    e32hal61.hpp
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

#ifndef __E32HAL61_HPP__
#define __E32HAL61_HPP__

#include <e32hal.h>

enum TBacklightBehavior
{
  EBacklightBehaviorTimed,EBacklightBehaviorUntimed
};

enum TAutoSwitchOffBehavior
{
  ESwitchOffDisabled,ESwitchOffEnabledOnBatteries,ESwitchOffEnabledAlways
};

enum TAutoSwitchOffType
{
  ESwitchOffHard,ESwitchOffEvent
};

class TSoundInfoV1
{
  public:
    TBool iKeyClickEnabled;
    TBool iKeyClickLoud;
    TBool iKeyClickOverridden;
    TBool iPointerClickEnabled;
    TBool iPointerClickLoud;
    TBool iBeepEnabled;
    TBool iBeepLoud;
    TBool iSoundDriverEnabled;
    TBool iSoundDriverLoud;
    TBool iSoundEnabled;
};
typedef TPckgBuf<TSoundInfoV1> TSoundInfoV1Buf;

class TOnOffInfoV1
{
  public:
    TAutoSwitchOffBehavior iAutoSwitchOffBehavior;
    TTimeIntervalSeconds iAutoSwitchOffTime;
    TBacklightBehavior iBacklightBehavior;
    TTimeIntervalSeconds iBacklightOnTime;
    TBool iPointerSwitchesOn;
    TBool iCaseOpenSwitchesOn;
    TBool iCaseCloseSwitchesOff;
};
typedef TPckgBuf<TOnOffInfoV1> TOnOffInfoV1Buf;

#endif
