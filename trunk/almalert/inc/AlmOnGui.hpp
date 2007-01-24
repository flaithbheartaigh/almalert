/*
    AlmOnGui.hpp
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

#ifndef __ALMONGUI_HPP__
#define __ALMONGUI_HPP__

#include <ActivityManager.hpp> //CUserActivityManager
#include "AlarmAlertSettings.hpp"
#include "AlmAudioBeep.hpp"
#include "AlmBirthdayTimer.hpp"
#include "AlmAudioSms.hpp"
#include "AlmInterface.hpp"

class CAlmOnGui: public CActive
{
  public:
    static CAlmOnGui* NewL(CSettings& aSettings,MAlmInterface& anAlmInterface,CEikonEnv* anEnv);
    ~CAlmOnGui();
    void CancelBirthdayAudio(void);
  protected:
    void RunL(void);
    void DoCancel(void);
  private:
    CAlmOnGui(CSettings& aSettings,MAlmInterface& anAlmInterface,CEikonEnv* anEnv);
    void ConstructL(void);
    void NotifyL(void);
  private:
    CSettings& iSettings;
    MAlmInterface& iAlmInterface;
    CEikonEnv* iEnv;
  private: //Beeper
    TBool iBeepActive;
    CPeriodic* iBeeper;
    CAlmAudioBeep* iBeepAudio;
    static TInt BeeperTimeout(TAny* anAlm);
    void DoBeeperTimeout(void);
    void InitBeeperL(void);
    void SetBeeper(void);
    void CancelBeeper(void);
  private: //Birthday
    TBool iBirthdayActive;
    TInt iBirthdayHour;
    CBirthdayTimer* iBirthday;
    CAlmAudioSms* iBirthdayAudio;
    void InitBirthdayL(void);
    static TInt BirthdayTimeout(TAny* anAlm);
    void DoBirthdayTimeoutL(void);
    void CancelBirthday(void);
  private: //Activity
    TBool iUserActive;
    CUserActivityManager* iActivity;
    static TInt OnActivity(TAny* anAlm);
    static TInt OnInactivity(TAny* anAlm);

};

#endif
