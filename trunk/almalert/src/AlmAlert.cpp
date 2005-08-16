/*
    AlmAlert.cpp
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

#include "AlmAlert.hpp"
#include <bautils.h> //BaflUtils
#include <coemain.h> //CCoeEnv
#include <coeaui.h> //ECoeStackPriorityDialog...
#include <t32alm.h> //TAlarmInfo
#include <eikimage.h> //CEikImage
#include <aknconsts.h> //KAvkonBitmapFile

#include <almalert.rsg>
#include <avkon.rsg>

#include <avkon.mbg>

#include <eiksrv.h> //FIXME

_LIT(KAlarmFormat,"%04d%02d%02d:%02d%02d%02d.000000");
_LIT(KKeySnoozeMsg,"SnoozeMsg");
_LIT(KKeyAlarmTime,"AlarmTime");
_LIT(KKeySnooze,"Snooze");
_LIT(KLabelSnooze,"Snooze");
_LIT(KResourceFile,"z:\\System\\data\\AlmAlert.rsc");
_LIT(KKeyHide,"Hide");
_LIT(KKeyStateVal,"state.val");
_LIT(KEmpty,"");
_LIT(KKeyLight,"Light");
_LIT(KKeyKeyGuard,"KeyGuard");

const TUid KSysAppUid={0x100058F3};
const TUid KSysUtilUid={0x10005943};
const TUid KAlmAlertUid={0x1000599E};

CAlm::CAlm(): iAlmFlags(0)
{
  iBorder=TGulBorder(0x800110);
}

CAlm::~CAlm()
{
  delete iBeeper;
  delete iBeepAudio;
  delete iNoteContainer;
  delete iAudio;
  ((CEikonEnv*)ControlEnv())->RemoveFromStack(this);
  if(iButtonsCurrent) ((CEikonEnv*)ControlEnv())->RemoveFromStack(iButtonsCurrent->ButtonGroup()->AsControl());
  delete iButtonsStopSnooze;
  delete iButtonsYesNo;
  ControlEnv()->DeleteResourceFile(iResourceOffset);
  delete iWatchdogTimer;
  delete iKeyGuardTimer;
  delete iIdle;
  delete iCreateAudioTimer;
  delete iBlinkOffTimer;
  RemoveDevStateNotification();
  if(iKeyHideNotify)
  {
    iKeyHideNotify->Close();
    delete iKeyHideNotify;
  }
}

const TDesC8& CAlm::Copyright(void) const
{
  _LIT8(KAppCopyright,"(c) 2005 by zg. version 0.04");
  return KAppCopyright;
}

void CAlm::ShowRestartNoteL(void) //checked
{
  iAlmFlags&=~EFlagAlarmKeyAllowed;
  iCreateAudioTimer->Cancel();
  delete iAudio;
  iAudio=NULL;
  iAlmFlags|=EFlagAlarmRestartNoteActive;
  HideCurrentButtons();
  FadeBehindPopup(EFalse);
  SetCurrentButtonsL(iButtonsYesNo);
  FadeBehindPopup(ETrue);
  iNoteContainer->CreateRestartNoteL();
  UpdateNoteLayout(TSize(176,78));
  iNoteContainer->UpdateRedraw();
  iButtonsYesNo->DrawNow();
  if(iKeyGuardTimer->IsActive()) iKeyGuardTimer->Cancel();
  iAlmFlags&=~EFlagAlarmAcknowledged;
  TCallBack callback(KeyGuardTimeout,this);
  iKeyGuardTimer->Start(500000,500000,callback);
}

void CAlm::FadeBehindPopup(TBool aFade) //checked
{
  iFader.FadeBehindPopup(this,this,aFade);
}

void CAlm::Blink(TBool aState) //checked
{
  iBlinkOffTimer->Cancel();
  RSharedDataClient blink;
  TInt err=KErrNone;
  if((err=blink.Connect(0))==KErrNone)
  {
    if(blink.AssignToTemporaryFile(KSysAppUid)==KErrNone)
    {
      blink.SetInt(KKeyLight,aState);
      blink.SetInt(KKeyKeyGuard,aState);
    }
    blink.Close();
  }
}

void CAlm::SetSnoozeInfo(void) //checked
{
  if(iAlmFlags&EFlagAlarmSnoozeAllowed)
  {
    RSharedDataClient info;
    if(info.Connect(0)==KErrNone)
    {
      TInt flag=iAlmFlags&EFlagAlarmSnoozed;
      if(info.Assign(KAlmAlarmUid)==KErrNone)
      {
        TTime time;
        time.HomeTime();
        time+=iSettings->SnoozeTime();
        TDateTime dtime=time.DateTime();
        TBuf16<30> buf;
        buf.Format(KAlarmFormat,dtime.Year(),dtime.Month(),dtime.Day(),dtime.Hour(),dtime.Minute(),0);
        if(flag)
        {
          info.SetString(KKeySnoozeMsg,KLabelSnooze);
          info.SetString(KKeyAlarmTime,buf);
          TInt snooze=0;
          info.GetInt(KKeySnooze,snooze);
          info.SetInt(KKeySnooze,--snooze);
        }
        else
        {
          info.SetString(KKeySnoozeMsg,KLabelSnooze);
          info.SetString(KKeyAlarmTime,buf);
          info.SetInt(KKeySnooze,iSettings->SnoozeCount());
        }
      }
      info.Close();
    }
  }
}

TBool CAlm::UpdateStartupState(void) //checked
{
  TSWState state=SysStartup::State();
  TBool result=EFalse;
  if(state!=ESWState203&&state!=ESWState205&&state!=ESWState206) result=ETrue;
  if(result) iAlmFlags|=EFlagAlarmDeviceInInternalState;
  return result;
}

void CAlm::UpdateStartupReason(void)
{
  TWD2StartupReason reason1; TSWStartupReason reason2;
  SysStartup::GetStartupReasons(reason1,reason2);
  if(reason1==EWD2StartupAlarm||reason1==EWD2StartupCharger) iAlmFlags|=EFlagAlarmStartPhone;
  else iAlmFlags&=~EFlagAlarmStartPhone;
}

CEikButtonGroupContainer* CAlm::LoadButtonsL(TInt aResourceId) //checked
{
  CEikButtonGroupContainer* self=CEikButtonGroupContainer::NewL(CEikButtonGroupContainer::ECba,CEikButtonGroupContainer::EHorizontal,this,aResourceId,iServerAppUi->AlertGroupWin(),CEikButtonGroupContainer::EUseMaxSize);
  self->MakeVisible(EFalse);
  return self;
}

void CAlm::SetCurrentButtonsL(CEikButtonGroupContainer* aButtons) //checked
{
  if(iButtonsCurrent) ((CEikonEnv*)ControlEnv())->RemoveFromStack(iButtonsCurrent->ButtonGroup()->AsControl());
  CCoeControl* control=aButtons->ButtonGroup()->AsControl();
  iButtonsCurrent=aButtons;
  ((CCoeAppUi*)ControlEnv()->AppUi())->AddToStackL(control,ECoeStackPriorityCba,ECoeStackFlagStandard);
  control->DrawableWindow()->SetOrdinalPosition(0);
  control->MakeVisible(ETrue);
  control->DrawNow();
}

void CAlm::HideCurrentButtons(void) //checked
{
  iButtonsCurrent->MakeVisible(EFalse);
  CCoeControl* control=iButtonsCurrent->ButtonGroup()->AsControl();
  ((CCoeAppUi*)ControlEnv()->AppUi())->UpdateStackedControlFlags(control,3,3); //FIXME: разобраться с флагами
}

TBool CAlm::IsSnoozeAlarm(void) //checked
{
  TBool res=EFalse;
  if(((const TDesC&)KLabelSnooze)==iAlarmMessage&&iAlarmType==EAlarmTypeClock) res=ETrue;
  return res;
}

TBool CAlm::SnoozeAllowed(void) //not checked
{
  RSharedDataClient info;
  if(info.Connect(0)!=KErrNone) return EFalse;
  TBool result=ETrue;
  if(info.Assign(KAlmAlarmUid)==KErrNone&&(iAlmFlags&EFlagAlarmSnoozed))
  {
    TInt snooze=0;
    info.GetInt(KKeySnooze,snooze);
    snooze--;
    if(!snooze) result=EFalse;
  }
  info.Close();
  return result;
}

TInt CAlm::WatchdogTimeout(TAny* anAlm) //checked
{
  return ((CAlm*)anAlm)->DoWatchdogTimeout();
}

TInt CAlm::DoWatchdogTimeout(void) //checked
{
  if((iAlmFlags&EFlagAlarmAcknowledged)==0)
  {
    AcknowledgeAlarm();
  }
  iWatchdogTimer->Cancel();
  return 0;
}

void CAlm::OrderAlertGroupWin(TBool aState) //checked
{
  RWindowGroup& alert=iServerAppUi->AlertGroupWin();
  if(aState)
  {
    TInt local=iAlertGroupOrder++;
    if(!local)
    {
      alert.SetOrdinalPosition(0,1000);
    }
  }
  else
  {
    iAlertGroupOrder--;
    if(!iAlertGroupOrder)
    {
      alert.SetOrdinalPosition(0,-1000);
    }
  }
}

void CAlm::AcknowledgeAlarm(void) //checked
{
  iAlmFlags|=EFlagAlarmAcknowledged;
  if(!(iAlmFlags&EFlagAlarmRestartNoteActive))
  {
    if(iAlarmType==EAlarmTypeClock)
    {
      SetSnoozeInfo();
      iSupervisor->CmdAcknowledgeAlarm();
      if(iAlmFlags&EFlagAlarmStartPhone) SysStartup::Shutdown(KAlmAlarmUid);
    }
    else
    {
      iSupervisor->CmdTaskAwayFromAlarm();
    }
  }
  else
  {
    iAlmFlags=iAlmFlags&~EFlagAlarmRestartNoteActive;
    iSupervisor->CmdAcknowledgeAlarm();
    SysStartup::Shutdown(KAlmAlarmUid);
  }
}

TInt CAlm::KeyGuardTimeout(TAny* anAlm)
{
  return ((CAlm*)anAlm)->DoKeyGuardTimeout();
}

TInt CAlm::DoKeyGuardTimeout(void)
{
  iAlmFlags|=EFlagAlarmKeyAllowed;
  iKeyGuardTimer->Cancel();
  if(iWatchdogTimer->IsActive()) iWatchdogTimer->Cancel();
  TCallBack callback(WatchdogTimeout,this);
  iWatchdogTimer->Start(59000000,59000000,callback);
  return 0;
}

TInt CAlm::Idle(TAny* anAlm)
{
  return ((CAlm*)anAlm)->DoIdle();
}

TInt CAlm::DoIdle(void)
{
  if(!(iAlmFlags&EFlagAlarmAcknowledged))
    if(iAlmFlags&EFlagAlarmKeyAllowed) AcknowledgeAlarm();
  iIdle->Cancel();
  return 0;
}

void CAlm::ConstructAlarmL(CEikAlmControlSupervisor* aSupervisor,CEikServAppUi* aAppUi)
{
  iSupervisor=aSupervisor;
  iServerAppUi=aAppUi;
  TFileName resourceFile(KResourceFile);
  BaflUtils::NearestLanguageFile(iCoeEnv->FsSession(),resourceFile);
  iResourceOffset=iCoeEnv->AddResourceFileL(resourceFile);
  SetMopParent(iServerAppUi);
  CreateWindowL(iServerAppUi->AlertGroupWin());
  EnableDragEvents();
  SetPointerCapture();
  ((CCoeAppUi*)ControlEnv()->AppUi())->AddToStackL(this,ECoeStackPriorityDialog,ECoeStackFlagRefusesAllKeys|ECoeStackFlagRefusesFocus|ECoeStackFlagSharable);
  Window().SetPointerGrab(ETrue);
  iContext=this;
  Window().SetBackgroundColor(0xffffff);
  iNoteContainer=new(ELeave)CNoteContainer();
  iNoteContainer->ConstructL(this);
  iWatchdogTimer=CPeriodic::NewL(CActive::EPriorityStandard);
  iKeyGuardTimer=CPeriodic::NewL(CActive::EPriorityStandard);
  iIdle=CIdle::NewL(CActive::EPriorityIdle);
  iCreateAudioTimer=CPeriodic::NewL(CActive::EPriorityIdle);
  iBlinkOffTimer=CPeriodic::NewL(CActive::EPriorityStandard);
  iSettings=new(ELeave)CSettings;
  iKeyHideNotify=new(ELeave)RSharedDataClient(this);
  User::LeaveIfError(iKeyHideNotify->Connect(0));
  User::LeaveIfError(iKeyHideNotify->NotifySet(KSysAppUid,&KKeyHide));
  iButtonsStopSnooze=LoadButtonsL(R_ALERT_SOFTKEYS_STOP_SNOOZE);
  iButtonsYesNo=LoadButtonsL(R_AVKON_SOFTKEYS_YES_NO);
  iButtonsCurrent=iButtonsStopSnooze;
  CCoeControl* buttons=iButtonsCurrent->ButtonGroup()->AsControl();
  ((CCoeAppUi*)ControlEnv()->AppUi())->AddToStackL(buttons,ECoeStackPriorityCba,ECoeStackFlagRefusesAllKeys|ECoeStackFlagRefusesFocus);
  SetComponentsToInheritVisibility(ETrue);
  MakeVisible(EFalse);
  iAlmFlags=0;
  iNoteController=iServerAppUi->iNoteController;
  iNoteController->SetNoteObserver(this);
  UpdateStartupReason();
  SetDevStateNotification();
}

void CAlm::ActivateNoteL(void) //checked
{
  Blink(ETrue);
  FadeBehindPopup(ETrue);
  RWindowGroup& alert=iServerAppUi->AlertGroupWin();
  iCaptureHandle=alert.CaptureKeyUpAndDowns(EStdKeyDevice6,EModifierFunc|EModifierShift|EModifierCtrl,0);
  OrderAlertGroupWin(ETrue);
  DrawableWindow()->SetOrdinalPosition(0);
  alert.SetOrdinalPosition(0);
  ClaimPointerGrab();
  MakeVisible(ETrue);
  ((CEikonEnv*)ControlEnv())->RouseSleepingDialog(this,ETrue);
  TRAPD(err,ActivateL());
  if(SnoozeAllowed())
  {
    iAlmFlags|=EFlagAlarmSnoozeAllowed;
  }
  else
  {
    iAlmFlags&=~EFlagAlarmSnoozeAllowed;
  }
  iButtonsStopSnooze->MakeCommandVisible(0x6002,iAlmFlags&EFlagAlarmSnoozeAllowed);
  SetCurrentButtonsL(iButtonsStopSnooze);
  if(iKeyGuardTimer->IsActive()) iKeyGuardTimer->Cancel();
  TCallBack callback(KeyGuardTimeout,this);
  iKeyGuardTimer->Start(500000,500000,callback);
  InitializeAudio();
}

void CAlm::DeactivateNote(void) //checked
{
  iCreateAudioTimer->Cancel();
  delete iAudio;
  iAudio=NULL;
  iAlmFlags&=~EFlagAlarmKeyAllowed;
  FadeBehindPopup(EFalse);
  iKeyGuardTimer->Cancel();
  iWatchdogTimer->Cancel();
  if(iAlmFlags&EFlagAlarmStartPhone)
  {
    Blink(EFalse);
  }
  else
  {
    iBlinkOffTimer->Cancel();
    TCallBack callback(BlinkOffTimeout,this);
    iBlinkOffTimer->Start(500000,0,callback);
  }
  iServerAppUi->AlertGroupWin().CancelCaptureKeyUpAndDowns(iCaptureHandle);
  OrderAlertGroupWin(EFalse);
  MakeVisible(EFalse);
  HideCurrentButtons();
  ((CEikonEnv*)ControlEnv())->RouseSleepingDialog(this,EFalse);
}

void CAlm::SetDevStateNotification(void) //checked
{
  if(!iDevStateNotify)
  {
    iDevStateNotify=new(ELeave)RSharedDataClient(this);
    User::LeaveIfError(iDevStateNotify->Connect(0));
    User::LeaveIfError(iDevStateNotify->NotifySet(KSysUtilUid,&KKeyStateVal));
  }
}

void CAlm::RemoveDevStateNotification(void) //checked
{
  if(iDevStateNotify) iDevStateNotify->Close();
  delete iDevStateNotify;
  iDevStateNotify=NULL;
}

void CAlm::DoCreateAudioTimeout(void) //checked
{
  iCreateAudioTimer->Cancel();
  TRAPD(err,iAudio=CAlmAudio::NewL((CEikonEnv*)ControlEnv(),iSettings,iAlarmType));
}

void CAlm::InitializeAudio(void) //checked
{
  iCreateAudioTimer->Cancel();
  if(iAudio)
  {
    delete iAudio;
    iAudio=NULL;
  }
  iCreateAudioTimer->Start(100000,100000,TCallBack(CreateAudioTimeout,this));
}

TInt CAlm::CreateAudioTimeout(TAny* anAlm)
{
  ((CAlm*)anAlm)->DoCreateAudioTimeout();
  return KErrNone;
}

TInt CAlm::BlinkOffTimeout(TAny* anAlm) //checked
{
  ((CAlm*)anAlm)->Blink(EFalse);
  return KErrNone;
}

void CAlm::Release(void) //checked
{
  delete this;
}

void CAlm::ShowAlarm(void)
{
  if(!(iAlmFlags&EFlagAlarmActive))
  {
    UpdateStartupState();
    iAlmFlags|=EFlagAlarmActive;
    iNoteId=iNoteController->LaunchNoteL(0,0,3095);
  }
}

void CAlm::HideAlarm(void) //checked
{
  if(iAlmFlags&EFlagAlarmActive)
  {
    iAlmFlags&=~EFlagAlarmActive;
    iNoteController->CancelNote(iNoteId);
  }
}

TInt CAlm::CurrentServerState() const //checked
{
  return iServerState;
}

void CAlm::UpdateSoundPauseTimeInterval(TInt aMinutes) //checked
{
}

void CAlm::UpdateForAlarmServerState(TInt aNewAlarmServerState) //checked
{
  if(aNewAlarmServerState!=iServerState)
  {
    iNoteContainer->UpdateRedraw();
    iServerState=aNewAlarmServerState;
  }
}

void CAlm::StartPlayAlarmL(const TDesC& aAlarmName) //checked
{
}

void CAlm::StopPlayAlarm(void) //checked
{
}

void CAlm::UpdateAlarmInfo(const TAlarmInfo& aAlarm,const TFullName& aOwner)
{
  User::ResetInactivityTime();
  iBlinkOffTimer->Cancel();
  TAlarmInfo alarm=aAlarm; //???
  iAlarmType=alarm.iType;
  iAlarmMessage=alarm.iMessage;
  iAlarmTime=alarm.iAlarmTime;
  if(IsSnoozeAlarm())
  {
    iAlmFlags|=EFlagAlarmSnoozed;
  }
  else
  {
    iAlmFlags&=~EFlagAlarmSnoozed;
  }
  if(iAlmFlags&EFlagAlarmSnoozed)
  {
    iAlarmMessage.Zero();
    iCoeEnv->ReadResourceAsDes16(alarm.iMessage,R_EIKALARM_SNOOZEMSG);
  }
  TRAPD(err,iNoteContainer->ConstructAlarmNoteL(alarm));;
  if(err!=KErrNone) iSupervisor->CmdAcknowledgeAlarm();
  UpdateNoteLayout(TSize(176,78));
  iNoteContainer->ActiveNote()->Layout();
  iNoteContainer->UpdateRedraw();
  DrawDeferred();
  iAlmFlags&=~EFlagAlarmAcknowledged;
  if((iAlmFlags&EFlagAlarmActive)&&!(iAlmFlags&EFlagAlarmRestartNoteActive)&&!(iAlmFlags&EFlagAlarmStartPhone))
  {
    iKeyGuardTimer->Cancel();
    iKeyGuardTimer->Start(500000,500000,TCallBack(KeyGuardTimeout,this));
    InitializeAudio();
  }
}

TKeyResponse CAlm::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType) //checked
{
  if(aKeyEvent.iScanCode<EStdKeyDevice6&&aKeyEvent.iScanCode>EStdKeyDevice3) return EKeyWasNotConsumed;
  if(aType==EEventKeyDown)
  {
    iIdle->Cancel();
    iIdle->Start(TCallBack(Idle,this));
  }
  if(aType==EEventKey)
  {
    iIdle->Cancel();
    if(!(iAlmFlags&EFlagAlarmAcknowledged))
    {
      if(iAlmFlags&EFlagAlarmKeyAllowed) AcknowledgeAlarm();
      iAlmFlags|=EFlagAlarmAcknowledged;
    }
  }
  return EKeyWasConsumed;
}

TInt CAlm::CountComponentControls() const
{
  return 1;
}

CCoeControl* CAlm::ComponentControl(TInt aIndex) const //checked
{
  if(!aIndex) return iNoteContainer->ActiveNote();
  return NULL;
}

TSize CAlm::MinimumSize() //checked
{
  return TSize(176,78);
}

void CAlm::SizeChanged() //checked
{
  TRect rect=Rect();
  rect=iBorder.InnerRect(rect);
  iNoteContainer->ActiveNote()->SetRect(rect);
  iButtonsStopSnooze->SetBoundingRect(TRect(TPoint(0,0),ControlEnv()->ScreenDevice()->SizeInPixels()));
  iButtonsYesNo->SetBoundingRect(TRect(TPoint(0,0),ControlEnv()->ScreenDevice()->SizeInPixels()));
}

void CAlm::UpdateNoteLayout(const TSize& aSize) //checked
{
  CAknNoteControl* control=iNoteContainer->ActiveNote();
  if(control)
  {
    TRect rect=iServerAppUi->ApplicationRect();
    TAknWindowLineLayout lineLayout(3,rect,0,0,1,0,1,0,0);
    lineLayout.iH=rect.iBr.iY+lineLayout.ib-lineLayout.it;
    AknLayoutUtils::LayoutControl(this,TRect(TPoint(lineLayout.il,lineLayout.it),TSize(lineLayout.iW,lineLayout.iH)),control->WindowLayout());
  }
}

void CAlm::PrepareContext(CWindowGc& aGc) const //checked
{
  aGc.SetBrushStyle(CGraphicsContext::ESolidBrush);
  aGc.SetBrushColor(TRgb(0xffffff));
}

void CAlm::Draw(const TRect& aRect) const //checked
{
  CWindowGc& gc=SystemGc();
  gc.Clear();
  TRect rect=Rect();
  iBorder.Draw(gc,rect);
}

void CAlm::ProcessCommandL(TInt aCommandId)
{
  if(iAlmFlags&EFlagAlarmKeyAllowed)
  {
    iAlmFlags|=EFlagAlarmAcknowledged;
    switch(aCommandId)
    {
      case 3005: //startup
        iAlmFlags&=~EFlagAlarmRestartNoteActive;
        iNoteContainer->DestroyRestartNote();
        iSupervisor->CmdAcknowledgeAlarm();
        SysStartup::SetState(KAlmAlertUid,ESWState207);
        break;
      case 3006: //off
        iAlmFlags&=~EFlagAlarmRestartNoteActive;
        iNoteContainer->DestroyRestartNote();
        iSupervisor->CmdAcknowledgeAlarm();
        SysStartup::Shutdown(KAlmAlertUid);
        break;
      case 0x6001: //stop
        if(iAlmFlags&EFlagAlarmStartPhone)
        {
          ShowRestartNoteL();
        }
        else
        {
          iSupervisor->CmdAcknowledgeAlarm();
          iAlmFlags&=~EFlagAlarmKeyAllowed;
          if(iKeyGuardTimer->IsActive()) iKeyGuardTimer->Cancel();
          iKeyGuardTimer->Start(500000,500000,TCallBack(KeyGuardTimeout,this));
        }
        break;
      case 0x6002: //snooze
        AcknowledgeAlarm();
        break;
    }
  }
}

void CAlm::SharedDataNotify(TUid anUid,const TDesC16& aKey,const TDesC16& aValue)
{
  if(anUid==KSysAppUid&&aKey==KKeyHide)
  {
    if(!(iAlmFlags&EFlagAlarmAcknowledged)&&iAlmFlags&EFlagAlarmActive) AcknowledgeAlarm();
  }
  else if(anUid==KSysUtilUid&&aKey==KKeyStateVal)
  {
    TSWState state=SysStartup::State();
    if((iAlmFlags&EFlagAlarmDeviceInInternalState)&&(state==ESWState203||state==ESWState205||state==ESWState206))
    {
      iAlmFlags&=~EFlagAlarmDeviceInInternalState;
      ActivateNoteL();
    }
    if(state==ESWState203)
    {
      iAlmFlags&=~EFlagAlarmStartPhone;
      RemoveDevStateNotification();
      OnGui();
    }
  }
}

void CAlm::OnGui(void)
{
  InitBeeperL();
}

void CAlm::DialogNotify1(void) //checked
{
}

TBool CAlm::DialogNotify2(TInt aParam) //checked
{
  if(aParam!=0xC17) return EFalse;
  TBool res=ETrue;
  if(IsVisible()) return ETrue;
  if(!UpdateStartupState()) //показываем ноут, если устройство в окончательном состоянии.
  {
    ActivateNoteL();
    return res;
  }
  else return res;
}

TBool CAlm::DialogNotify3(TInt aParam) //checked
{
  iCreateAudioTimer->Cancel();
  iKeyGuardTimer->Cancel();
  iWatchdogTimer->Cancel();
  if(aParam!=0xC17) return EFalse;
  if(IsVisible()) DeactivateNote();
  return ETrue;
}

TInt CAlm::CountFadedComponents() //checked
{
  return 2;
}

CCoeControl* CAlm::FadedComponent(TInt aIndex) //checked
{
  if(!aIndex) return this;
  if(aIndex!=1) return NULL;
  return iButtonsCurrent;
}

void CAlm::SetBeeperL(void) //устанавливает таймер до следующего часа
{
  TTime next,curr;
  TCallBack callback(BeeperTimeout,this);
  TTimeIntervalMicroSeconds32 diff;
  next.HomeTime();
  if(next>TTime(0))
  {
    next+=TTimeIntervalMicroSeconds(1799999999);
  }
  next-=next.Int64()%TInt64(1800000000);
  curr.HomeTime();
  diff=next.MicroSecondsFrom(curr).Int64().GetTInt();
  if(diff>TTimeIntervalMicroSeconds32(0))
  {
    iBeeper->Start(diff,0,callback);
  }
  else
  {
    User::After(100000);
    SetBeeperL();
  }
}

void CAlm::InitBeeperL(void)
{
  if(iSettings->Beeper())
  {
    iBeeper=CPeriodic::NewL(CActive::EPriorityIdle);
    SetBeeperL();
  }
}

void CAlm::DoBeeperTimeout(void)
{
  iBeeper->Cancel();
  if(iBeepAudio)
  {
    delete iBeepAudio;
    iBeepAudio=NULL;
    SetBeeperL();
  }
  else
  {
    TTime time;
    time.HomeTime();
    time.RoundUpToNextMinute();
    TDateTime dtime=time.DateTime();
    TInt hour=dtime.Hour(),minute=dtime.Minute();
    if(!(iAlmFlags&EFlagAlarmActive)&&minute<2&&hour>=iSettings->BeepStart()&&hour<=iSettings->BeepFinish())
    {
      TRAPD(err,iBeepAudio=CAlmAudioBeep::NewL((CEikonEnv*)(ControlEnv()),iSettings));
    }
    if(iBeepAudio)
    {
      TCallBack callback(BeeperTimeout,this);
      iBeeper->Start(5000000,0,callback);
    }
    else SetBeeperL();
  }
}

TInt CAlm::BeeperTimeout(TAny* aSettings)
{
  ((CAlm*)aSettings)->DoBeeperTimeout();
  return 0;
}

EXPORT_C MAlm* NewAlarm(void) //checked
{
  return new CAlm;
}

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
{
  return KErrNone;
}

CNoteContainer::CNoteContainer() //checked
{
}

CNoteContainer::~CNoteContainer() //checked
{
  delete iNote;
}

void CNoteContainer::ConstructL(CAlm* anAlm) //checked
{
  iAlm=anAlm;
  iEnv=(CEikonEnv*)anAlm->ControlEnv();
  iNote=new(ELeave)CAknNoteControl;
  TResourceReader reader;
  iEnv->CreateResourceReaderLC(reader,R_EIKALARM_NOTE);
  iNote->SetContainerWindowL(*iAlm);
  iNote->ConstructFromResourceL(reader);
  CleanupStack::PopAndDestroy();
}

void CNoteContainer::ConstructAlarmNoteL(const TAlarmInfo& aInfo)
{
  TInt resourceId=R_EIKALARM_OTHER_ALARM_TYPE;
  CEikImage* image=new(ELeave)CEikImage;
  CleanupStack::PushL(image);
  CFbsBitmap* bitmap_mask=new(ELeave)CFbsBitmap;
  CleanupStack::PushL(bitmap_mask);
  CFbsBitmap* bitmap=new(ELeave)CFbsBitmap;
  CleanupStack::PushL(bitmap);
  if(aInfo.iType==EAlarmTypeClock)
  {
    resourceId=R_EIKALARM_CLOCK_ALARM_TYPE;
    bitmap->Load(KAvkonBitmapFile,EMbmAvkonQgn_note_alarm_clock);
    bitmap_mask->Load(KAvkonBitmapFile,EMbmAvkonQgn_note_alarm_clock_mask);
  }
  else
  {
    bitmap->Load(KAvkonBitmapFile,EMbmAvkonQgn_note_alarm_calendar);
    bitmap_mask->Load(KAvkonBitmapFile,EMbmAvkonQgn_note_alarm_calendar_mask);
  }
  image->SetBitmap(bitmap);
  CleanupStack::Pop();
  image->SetMask(bitmap_mask);
  CleanupStack::Pop();
  iNote->SetImageL(image);
  CleanupStack::Pop();
  iNote->ResetText();
  TBuf<256> label;
  iEnv->ReadResourceAsDes16(label,resourceId);
  iNote->SetTextL(label,0);
  iNote->SetTextL(KEmpty,2);
  TInt length=aInfo.iMessage.Length();
  if(length)
  {
    label.Zero();
    label.Copy(aInfo.iMessage.Ptr(),Min(length,256));
    iNote->SetTextL(label,1);
  }
  TBuf<256> timeFormat;
  iEnv->ReadResourceAsDes16(timeFormat,R_QTN_TIME_USUAL_WITH_ZERO);
  aInfo.iDueTime.FormatL(label,timeFormat);
  TInt item=1;
  if(length||aInfo.iType!=EAlarmTypeClock)
    item=2;
  iNote->SetTextL(label,item);
  if(aInfo.iType!=EAlarmTypeClock)
  {
    iEnv->ReadResourceAsDes16(timeFormat,R_QTN_DATE_USUAL_WITH_ZERO);
    aInfo.iDueTime.FormatL(label,timeFormat);
    iNote->SetTextL(label,3);
  }
}

void CNoteContainer::UpdateRedraw(void)
{
  iNote->DrawDeferred();
}

void CNoteContainer::CreateRestartNoteL(void)
{
  iRestartNoteActive=ETrue;
  iNote->MakeVisible(EFalse);
  iRestartNote=new(ELeave)CAknNoteControl;
  TResourceReader reader;
  iEnv->CreateResourceReaderLC(reader,R_EIKALARM_NOTE);
  iRestartNote->SetContainerWindowL(*iAlm);
  iRestartNote->ConstructFromResourceL(reader);
  CleanupStack::PopAndDestroy();
  TBuf<128> label;
  iEnv->ReadResourceAsDes16(label,R_EIKALARM_CONFIRMATION);
  iRestartNote->SetTextL(label);
  iRestartNote->Layout();
}

void CNoteContainer::DestroyRestartNote(void) //checked
{
  iRestartNoteActive=EFalse;
  delete iRestartNote;
  iRestartNote=NULL;
}

CAknNoteControl* CNoteContainer::ActiveNote(void) //checked
{
  if(!iRestartNoteActive) return iNote;
  else return iRestartNote;
}
