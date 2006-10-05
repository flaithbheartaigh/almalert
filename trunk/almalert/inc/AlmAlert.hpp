/*
    AlmAlert.hpp
    Copyright (C) 2005-2006 zg

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

#ifndef __ALMALERT_HPP__
#define __ALMALERT_HPP__

#include "eikbctrl.h" //CEikBorderedControl
#include "coeccntx.h" //MCoeControlContext
#include "eikcmobs.h" //MEikCommandObserver
#include <aknpopupfader.h> //MAknFadedComponent
#include <uikon/eiksvfty.h> //eikalsup.h bug
#include <uikon/eikalsup.h> //CEikAlmControlSupervisor
#include <SysUtilEx.hpp> //SysStartup
#include <CommonEngine.hpp> //RSharedDataClient
#include <eikbtgpc.h> //CEikButtonGroupContainer
#include <aknnotecontrol.h> //CAknNoteControl
#include <EikSrvUi.hpp> //CEikServAppUi
#include <ActivityManager.hpp> //CUserActivityManager

#include "AlmAudioAlert.hpp"
#include "AlmAudioBeep.hpp"
#include "AlmAudioSms.hpp"
#include "AlarmAlertSettings.hpp"
#include "AlmBirthdayTimer.hpp"

const TInt KAlmAlarmUidValue=0x1000599E;
const TUid KAlmAlarmUid={KAlmAlarmUidValue};

class CAlm;

class CNoteContainer: public CBase
{
  private:
    CAknNoteControl* iNote; //0x04
    CAknNoteControl* iRestartNote; //0x08
    TInt iRestartNoteActive; //0x0c
    CEikonEnv* iEnv; //0x10
    CAlm* iAlm; //0x14
  public:
    CNoteContainer();
    ~CNoteContainer();
    void ConstructL(CAlm* anAlm); //FIXME
    void Redraw(void) const;
    void SetAlarmL(const TAlarmInfo& aInfo);
    void SetWakeupLabelL(void);
    CAknNoteControl* NoteControl(void);
    void ClearWakeupLabel(void);
};

class TUnknownRecord
{
  public:
    TInt iUnk01;
    TInt iUnk02;
    TInt iUnk03;
};

class MAlm
{
  public:
    virtual void Release(void)=0;
    virtual void ShowAlarm(void)=0;
    virtual void HideAlarm(void)=0;
    virtual TInt CurrentServerState() const=0;
    virtual void UpdateSoundPauseTimeInterval(TInt aMinutes)=0;
    virtual void UpdateForAlarmServerState(TInt aNewAlarmServerState)=0;
    virtual void UpdateAlarmInfo(const TAlarmInfo& aAlarm,const TFullName& aOwner)=0;
    virtual void StartPlayAlarmL(const TDesC& aAlarmName)=0;
    virtual void StopPlayAlarm(void)=0;
    virtual void ConstructAlarmL(CEikAlmControlSupervisor* aSupervisor,CEikServAppUi* aAppUi)=0;
};

class CAlm: public CEikBorderedControl,public MCoeControlContext,public MAlm,public MEikCommandObserver,public MSharedDataNotifyHandler,public MNotifierDialogObserver,public MAknFadedComponent
{
  public:
    enum TAlarmFlags
    {
      EFlagAlarmActive=0x001,
      EFlagAlarmStartPhone=0x002,
      EFlagAlarmAcknowledged=0x004,
      EFlagAlarmKeyAllowed=0x008,
      EFlagAlarmSnoozed=0x010,
      EFlagAlarmSnoozeAllowed=0x020,
      EFlagAlarmDeviceInInternalState=0x040,
      EFlagAlarmRestartNoteActive=0x080
    };
  public:
    CAlm();
    ~CAlm();
    const TDesC8& Copyright(void) const;
  public: //CEikBorderedControl
    TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
    TSize MinimumSize();
  protected:
    void SizeChanged();
  public:
    TInt CountComponentControls() const;
    CCoeControl* ComponentControl(TInt aIndex) const;
  private:
    void Draw(const TRect& aRect) const;
  public: //MEikCommandObserver
    void ProcessCommandL(TInt aCommandId);
  public: //MNotifierDialogObserver
    void NoteCompleted(TInt aNoteId,TInt aCommand);
    TBool DisplayDialogL(TInt aPriority);
    TBool CancelDialog(TInt aPriority);
  public: //MSharedDataNotifyHandler
    void HandleNotifyL(TUid anUid,const TDesC16& aKey,const TDesC16& aValue);
  public: //MAlm
    void UpdateSoundPauseTimeInterval(TInt aMinutes);
    void Release(void);
    void ShowAlarm(void);
    void HideAlarm(void);
    TInt CurrentServerState() const;
    void UpdateForAlarmServerState(TInt aNewAlarmServerState);
    void UpdateAlarmInfo(const TAlarmInfo& aAlarm,const TFullName& aOwner);
    void StartPlayAlarmL(const TDesC& aAlarmName);
    void StopPlayAlarm(void);
    void ConstructAlarmL(CEikAlmControlSupervisor* aSupervisor,CEikServAppUi* aAppUi);
  public: //MCoeControlContext
    void PrepareContext(CWindowGc& aGc) const;
  public: //MAknFadedComponent
    TInt CountFadedComponents();
    CCoeControl* FadedComponent(TInt aIndex);
  private:
    CNoteContainer* iNoteContainer; //0x4c
    TInt iServerState; //0x50
    CEikAlmControlSupervisor* iSupervisor; //0x54
    TInt iCaptureHandle; //0x6c
    CEikServAppUi* iServerAppUi; //0x70
    CAlmAudio* iAudio; //0x74
    CEikButtonGroupContainer* iButtonsStopSnooze; //0x78
    CEikButtonGroupContainer* iButtonsYesNo; //0x7c
    CEikButtonGroupContainer* iButtonsCurrent; //0x80
    TInt iResourceOffset; //0x84
    TInt iAlmFlags; //0x88
    TInt iAlertGroupOrder; //0x8c
    TInt iAlarmType; //0x90
    TBuf16<128> iAlarmMessage; //0x94
    TTime iAlarmTime; //0x19c
    // сторожевой таймер
    // запускается из обработчика iDisplayedTimer
    // если через 59 секунд будильник не проинициализируется
    // все закочится
    CPeriodic* iAutoHideTimer; //0x1a4
    // запускается при старте ноутов
    // предотвращает от случайного закрытия пользователем
    // т.е. в первые пол секунды ввод с клавиатуры не воспринимается
    CPeriodic* iDisplayedTimer; //0x1b0
    // делает CreateSnooze
    CIdle* iScancodeIdle; //0x1b4
    RSharedDataClient* iDevStateNotify; //0x1b8
    RSharedDataClient* iSysApNotify; //0x1bc
    CNotifierDialogController* iNoteController; //0x1c0
    TInt iNoteId; //0x1c4
    TAknPopupFader iFader; //0x1c8
    //запускает проигрывание мелодии
    CPeriodic* iPlayStartTimer; //0x1cc
    //BlinkOff
    CPeriodic* iKeyguardTimer; //0x1d0
    //настройки
    CSettings* iSettings;
  private:
    void OnGuiL(void);
    static TInt ScancodeCallback(TAny* anAlm);
    TInt DoScancodeCallback(void);
    void CreateSnooze(void);
    void NotifyStateBySnooze(void);
    CEikButtonGroupContainer* CreateCbaL(TInt aResourceId);
    void GetStartupReason(void);
    // процедура взвращает тру, если устройство находится в переходном состоянии
    // и фальс - если в окончательном
    TBool CheckStartupReason(void);
    void CreateStateHandlerL(void);
    TBool IsSnoozeAlarm(void);
    void SetSizeAndPosition(const TSize& aSize);
    static TInt DisplayedCallBack(TAny* anAlm);
    TInt DoUpdateFlag(void);
    void PlayStart(void);
    void AskWakupPhoneL(void);
    void DoShowAlarm(void);
    void DoCancelAlarm(void);
    void RemoveStateHandler(void);
    static TInt PlayStartCallback(TAny* anAlm);
    static TInt KeyguardCallBack(TAny* anAlm);
    void PlayAlarm(void);
    void FadeBehindPopup(TBool aFade);
    void NotifyStateToSysApp(TBool aState);
    void BringAlertGroupWinForwards(TBool aState);
    void DeactivateCba(void);
    TBool CanSnooze(void);
    void ActivateCba(CEikButtonGroupContainer* aButtons);
    static TInt AutoHideCallBack(TAny* anAlm);
    TInt DoAutoHide(void);
  private: //Beeper
    CPeriodic* iBeeper;
    CAlmAudioBeep* iBeepAudio;
    static TInt BeeperTimeout(TAny* anAlm);
    void DoBeeperTimeout(void);
    void InitBeeperL(void);
    void SetBeeper(void);
  private: //Birthday
    CBirthdayTimer* iBirthday;
    CAlmAudioSms* iBirthdayAudio;
    void InitBirthdayL(void);
    static TInt BirthdayTimeout(TAny* anAlm);
    void DoBirthdayTimeoutL(void);
  private: //Activity
    TBool iUserActive;
    CUserActivityManager* iActivity;
    static TInt OnActivity(TAny* anAlm);
    static TInt OnInactivity(TAny* anAlm);
};

#endif
