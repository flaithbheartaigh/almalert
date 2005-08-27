/*
    SharedData.hpp
    Copyright (C) 2005 zg

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

#ifndef __SHAREDDATA_HPP__
#define __SHAREDDATA_HPP__

//uids
const TUid KSysAppUid={0x100058F3};
const TUid KSysUtilUid={0x10005943};
const TUid KAlmAlertUid={0x1000599E};

//KAlmAlertUid
_LIT(KKeySnoozeMsg,"SnoozeMsg");
_LIT(KKeyAlarmTime,"AlarmTime");
_LIT(KKeySnooze,"Snooze");

//KSysAppUid
_LIT(KKeyHide,"Hide");
_LIT(KKeyLight,"Light");
_LIT(KKeyKeyGuard,"KeyGuard");
_LIT(KKeyToneQuit,"ToneQuit");
_LIT(KKeyGameMode,"GameMode"); //only on qd

//KSysUtilUid
_LIT(KKeyStateVal,"state.val");

#endif
