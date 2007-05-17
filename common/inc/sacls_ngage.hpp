/*
    sacls_ngage.hpp
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

#ifndef __SACLS_NGAGE_HPP__
#define __SACLS_NGAGE_HPP__

#include <e32std.h>

const TInt KUidUsbStatusValue=0x101F5CE1;
const TUid KUidUsbStatus={KUidUsbStatusValue};
enum TSAUsbStatus
{
  ESAUsbNotConnected,
  ESAUsbConnected
};

const TInt KUidMmcStatusValue={0x101F5CE2};
const TUid KUidMmcStatus={KUidMmcStatusValue};
enum TSAMmcStatus
{
  ESAMmcNone,
  ESAMmcLocked,
  ESAMmcUnlocked
};

const TInt KUidMusicPlayerStatusValue=0x101F5CE3;
const TUid KUidMusicPlayerStatus={KUidMusicPlayerStatusValue};
enum TSAMusicPlayerStatus
{
  ESAMusicPlayerActive,
  ESAMusicPlayerInActive
};

const TInt KUidRadioStatusValue=0x101F5CE4;
const TUid KUidRadioStatus={KUidRadioStatusValue};
enum TSARadioStatus
{
  ESAActive,
  ESAInActive
};

const TInt KUidStereoHeadsetStatusValue=0x101F5CE5;
const TUid KUidStereoHeadsetStatus={KUidStereoHeadsetStatusValue};
enum TSAStereoHeadsetStatus
{
  ESAStereoHeadsetBothConnected,
  ESAStereoHeadsetConnected,
  ESAStereoHeadsetNotConnected
};

#endif
