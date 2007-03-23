/*
    clockapp.hpp
    Copyright (C) 2005-2007 zg

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

#ifndef __CLOCKAPP_HPP__
#define __CLOCKAPP_HPP__

#include <aknapp.h>
#include <akndoc.h>
#include <aknviewappui.h>
#include <aknview.h>
#include <ClkWldAlmMdl.hpp>
#include <aknnavi.h>
#include <akntitle.h>
#include <clkdatetimeview.h>
#include <eikclock.h>
#include <eikimage.h>

class CClkApplication: public CAknApplication
{
  public: //CAknApplication
    TUid AppDllUid() const;
  protected: //CAknApplication
    CApaDocument* CreateDocumentL();
};

class CClkDocument: public CAknDocument
{
  public:
    static CClkDocument* NewL(CEikApplication& aApp);
    ~CClkDocument();
  public: //from CAknDocument
    CEikAppUi* CreateAppUiL();
  public:
    inline CClkAlmModel* Alm(void) {return iAlm;};
    inline CClkNitzModel* Nitz(void) {return iNitz;};
  private:
    CClkDocument(CEikApplication& aApp);
    void ConstructL(void);
  private:
    CClkAlmModel* iAlm; //0x24
    CClkNitzModel* iNitz; //0x28
};

enum TClkEnvChanged
{
  EClkEnvChanged3=3
};

class CSettingsClient;

class CClkAppUi: public CAknViewAppUi
{
  public:
    ~CClkAppUi();
  public:
    void ShowTitlePaneL(TInt aResId);
    CSettingsClient& Settings(void);
    TBool IsSettingsOk(void) {return (iSettings!=NULL);};
    void CmdSettingsL(void);
    void CmdHelpL(void);
    void CmdExit(void);
    void CmdBackL(void);
    void CmdSettingsExL(void);
    void CmdStopWatchL(void);
    void CmdLocaleL(void);
    void CmdCompactDBL(void);
    void CmdInternetTimeL(void);
  public:
    TTime GetLastAlarmTime(void);
    void SetLastAlarmTime(TTime& anAlarm);
    void RestoreDataL(void);
    void StoreDataL(void);
  private: //CAknAppUi
    void HandleCommandL(TInt aCommand);
    void ConstructL(void);
    void CheckAlmAlertL(void);
  private:
    static TInt DoNotiferCallbackL(TAny* anAppUi);
    void NotifyEnvChangedL(TClkEnvChanged anEvent);
  private:
    CEnvironmentChangeNotifier* iEnvNotifier; //0x7c
    CAknNavigationControlContainer* iNaviPane; //0x80
    CAknTitlePane* iTitlePane; //0x84
    CSettingsClient* iSettings;
    TInt64 iDate;
};

const TUid KViewId={1};
const TUid KClkAlmViewId={3};
const TUid KSettingsExViewId={4};
const TUid KStopWatchViewId={5};
const TUid KLocaleViewId={6};
const TUid KSettingsId={10};

class CClkAlmViewContainer;

class CClkAlmView: public CAknView,public MClkModelObserver
{
  public:
    static CClkAlmView* NewLC(CClkAlmModel* anAlm,CClkNitzModel* aNitz);
    ~CClkAlmView();
  public:
    void SetOpenedByOk(TBool aOpenedByOk);
    TInt NitzState(void);
    void HandleSettingsChangeL(TClkEnvChanged anEvent);
  private:
    void ConstructL(CClkAlmModel* anAlm,CClkNitzModel* aNitz);
  public: //CAknView
    TUid Id(void) const;
    void HandleCommandL(TInt aCommand);
  public: //MEikMenuObserver
    void DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane);
  private: //CAknView
    void DoActivateL(const TVwsViewId& aPrevViewId,TUid aCustomMessageId,const TDesC8& aCustomMessage);
    void DoDeactivate(void);
  private: //MClkModelObserver
    void HandleUpdateL(TInt aNotification);
  private:
    TBool OpenedByOk(void) const;
    CClkNitzModel* NitzMdl(void);
    TBool IsSnoozedAlarm(void);
    void ShowTitlePaneL(void);
    TBool UpdateAlarmStateL(TInt aParam);
    TBool UpdateNitzStateL(TInt aParam);
    void QueryAlarmTimeL(void);
    void HandleDeleteCmdL(TInt aParam1,TInt aParam2);
    void ShowRemainTimeL(const TTime& anAlarmTime);
  private:
    CClkAlmViewContainer* iControl; //0x38
    CClkAlmModel* iAlm; //0x3c
    CClkAlmIdleObserver* iAlmObserver; //0x40
    CClkNitzModel* iNitz; //0x44
    CClkNitzIdleObserver* iNitzObserver; //0x48
    SClockAlarmInfo iAlarmInfo; //0x4c
    TBool iOpenedByOk; //0x1ac
    TInt iNitzSettings; //0x1b0
    TInt iSnoozed; //0x1b4
    TBool iIsSx1;
};

class CClkAlmViewContainer: public CCoeControl
{
  public:
    static CClkAlmViewContainer* NewL(const TRect& aRect,CClkAlmView& aView);
    ~CClkAlmViewContainer();
    void ShowSnoozedAlarmL(const TTime& anAlarm);
    void ShowActiveAlarmL(const TTime& anAlarm);
    void ShowInactiveAlarm(void);
    void SetNitzInfoL(void);
  public: //CCoeControl
    TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
    void GetHelpContext(TCoeHelpContext& aContext) const;
    void Draw(const TRect& aRect) const;
    TInt CountComponentControls() const;
    CCoeControl* ComponentControl(TInt aIndex) const;
    void SizeChanged(void);
  private:
    void ConstructL(const TRect& aRect,CClkAlmView& aView);
    void SetAlarmTimeL(const TTime& anAlarm);
  private:
    enum TViewType
    {
      ETypeActive,
      ETypeInactive,
      ETypeSnoozed
    };
  private:
    CEikClock* iClock; //0x30
    CClkAlmView* iView; //0x34
    TAknLayoutRect iRect1; //0x38
    TBuf<5> iAlarmTimeStr; //0x4c
    TBuf<5> iString2; //0x60
    TBuf<32> iAlarmWeekStr; //0x74
    TBuf<12> iAlarmDateStr; //0xbc
    CEikImage* iImage; //0xdc
    TViewType iViewType; //0xe0
    HBufC16* iCurrentTimeStr; //0xe4
    HBufC16* iLocalTimeStr; //0xe8
    HBufC16* iNoAlarmsSetStr; //0xec
    HBufC16* iAlarmStr; //0xf0
    HBufC16* iSnoozeActiveStr; //0xf4
    TAknLayoutRect iRect2; //0xf8
    TBuf<12> iLocalDateStr; //0x10c
    TBuf<64> iString6; //0x12c
    TBuf<32> iLocalWeekStr; //0x1b4
};

#define iClockDocument ((CClkDocument*)Document())
#define iClkAppUi ((CClkAppUi*)AppUi())

#endif
