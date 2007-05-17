/*
    sacls_s60.hpp
    Copyright (C) 2007 zg

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

#ifndef __SACLS_S60_HPP__
#define __SACLS_S60_HPP__

#include <e32std.h>

const TInt KUidBatteryBarsValue=0x100052D3;
const TUid KUidBatteryBars={KUidBatteryBarsValue};
enum TSABatteryBars
{
  ESABBars_0,
  ESABBars_1,
  ESABBars_2,
  ESABBars_3,
  ESABBars_4,
  ESABBars_5,
  ESABBars_6,
  ESABBars_7
};

const TInt KUidNetworkBarsValue=0x100052D4;
const TUid KUidNetworkBars={KUidNetworkBarsValue};
enum TSANetworkBars
{
  ESANBars_0,
  ESANBars_1,
  ESANBars_2,
  ESANBars_3,
  ESANBars_4,
  ESANBars_5,
  ESANBars_6,
  ESANBars_7
};

const TInt KUidHeadsetStatusValue=0x100052D5;
const TUid KUidHeadsetStatus={KUidHeadsetStatusValue};
enum TSAHeadsetStatus
{
  ESAHeadsetNotConnected,
  ESAHeadsetConnected,
  ESAUnsupportedAccessoryConnected
};

const TInt KUidBatteryCStatusValue=0x100052D8;
const TUid KUidBatteryCStatus={KUidBatteryCStatusValue};
enum TSABatteryCStatus
{
  ESACBatteryOk,
  ESACBatteryLow,
  ESACBatteryEmpty,
  ESACPowerOff
};

const TInt KUidGprsAvailabilityValue=0x100052DA;
const TUid KUidGprsAvailability={KUidGprsAvailabilityValue};
enum TSAGprsAvailability
{
  ESAGprsAvailable,
  ESAGprsNotAvailable,
  ESAGprsAvailabilityUnknown
};

const TInt KUidGprsStatusValue=0x100052DB;
const TUid KUidGprsStatus={KUidGprsStatusValue};
enum TSAGprsStatus
{
  ESAGprsUnattached,
  ESAGprsAttach,
  ESAGprsContextActive,
  ESAGprsSuspend,
  ESAGprsContextActivating
};

const TInt KUidSimCStatusValue=0x100052E9;
const TUid KUidSimCStatus={KUidSimCStatusValue};
enum TSASimCStatus
{
  ESACSimInitWait,
  ESACSimLockOperative,
  ESACSimPinVerifyRequired,
  ESACSimPermanentlyBlocked,
  ESACSimRemoved,
  ESACSimRejected,
  ESACSimBlocked,
  ESACSimOk
};

#endif
