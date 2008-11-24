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
const TUid KSettingsUid={0x100058ec};
const TUid KSysAppUid={0x100058f3};
const TUid KSysUtilUid={0x10005943};
const TUid KAlmAlertUid={0x1000599e};
const TUid KSDUidVoiceRecorder={0x100058ca};

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

//KSettingsUid
_LIT(KKeyAutoRedial,"AutoRedial");
_LIT(KKeySumAftCall,"SumAftCall");
_LIT(KKeyOneKeyDial,"OneKeyDial");
_LIT(KKeyAnyKeyAns,"AnyKeyAns");
_LIT(KKeyMcnDisplay,"McnDisplay");
_LIT(KKeyWNoteType,"WNoteType");
_LIT(KKeyWNoteText,"WNoteText");
_LIT(KKeyWNoteImg,"WNoteImg");
_LIT(KKeyBGImg,"BGImg");
_LIT(KKeyBGImgPath,"BGImgPath");
_LIT(KKeyLeftIdle,"LeftIdle");
_LIT(KKeyRightIdle,"RightIdle");
_LIT(KKeyColPal,"ColPal");
_LIT(KKeyColPals,"ColPals");
_LIT(KKeyScrSvrObj,"ScrSvrObj");
_LIT(KKeyScrSvrText,"ScrSvrText");
_LIT(KKeyScrSvrPrd,"ScrSvrPrd");
_LIT(KKeyAutoLock,"AutoLock");
_LIT(KKeyLockTime,"LockTime");
_LIT(KKeyConfirmSat,"ConfirmSat");
_LIT(KKeyHsProfile,"HsProfile");
_LIT(KKeyLsProfile,"LsProfile");
_LIT(KKeyLoopSet,"LoopSet");
_LIT(KKeyHsAutoAns,"HsAutoAns");
_LIT(KKeyLsAutoAns,"LsAutoAns");
_LIT(KKeyInputLang,"InputLang");
_LIT(KKeyDispLang,"DispLang");
_LIT(KKeyPredTxt,"PredTxt");
_LIT(KKeyInputMode,"InputMode");
_LIT(KKeyTolerance,"Tolerance");
_LIT(KKeyNrGrouping,"NrGrouping");
_LIT(KKeyTTYProfile,"TTYProfile");
_LIT(KKeyAccessory,"Accessory");
_LIT(KKeyBTProfile,"BTProfile");
_LIT(KKeyBtAutoAns,"BtAutoAns");
_LIT(KKeySScrBObj,"SScrBObj");
_LIT(KKeySScrBTextLength,"SScrBText#len");
_LIT(KKeySScrBText,"SScrBText");
_LIT(KKeyScrSvrTextLength,"ScrSvrText#len");
//r/o keys
_LIT(KKeyROHSCSD,"R-HSCSD");
_LIT(KKeyROWNote,"R-WNote");
_LIT(KKeyROConfirmSat,"R-ConfirmSat");
_LIT(KKeyROOffline,"R-Offline");

//KSDUidVoiceRecorder
_LIT(KVoiceRecorderMode,"VRMode");

#endif
