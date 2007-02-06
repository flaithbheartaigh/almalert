/*
    AlmAlert.cpp
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

#include "AlmAlert.hpp"
#include <SharedData.hpp>

#include <bautils.h> //BaflUtils
#include <coemain.h> //CCoeEnv
#include <coeaui.h> //ECoeStackPriorityDialog...
#include <t32alm.h> //TAlarmInfo
#include <eikimage.h> //CEikImage
#include <aknconsts.h> //KAvkonBitmapFile
#include <hal.h>

#include <almalert.rsg>
#include <avkon.rsg>

#include <avkon.mbg>

#include <eiksrv.h> //FIXME

_LIT(KAlarmFormat,"%04d%02d%02d:%02d%02d%02d.000000");
_LIT(KLabelSnooze,"Snooze");
_LIT(KResourceFile,"z:\\System\\data\\AlmAlert.rsc");

CAlm::CAlm(): iAlmFlags(0)
{
  iBorder=TGulBorder(0x800110);
}

CAlm::~CAlm()
{
  delete iNoteContainer;
  delete iAudio;
  ((CEikonEnv*)ControlEnv())->RemoveFromStack(this);
  if(iButtonsCurrent) ((CEikonEnv*)ControlEnv())->RemoveFromStack(iButtonsCurrent->ButtonGroup()->AsControl());
  delete iButtonsStopSnooze;
  delete iButtonsYesNo;
  ControlEnv()->DeleteResourceFile(iResourceOffset);
  delete iAutoHideTimer;
  delete iDisplayedTimer;
  delete iScancodeIdle;
  delete iPlayStartTimer;
  delete iKeyguardTimer;
  RemoveStateHandler();
  if(iSysApNotify)
  {
    iSysApNotify->Close();
    delete iSysApNotify;
  }
  delete iSettings;
}

const TDesC8& CAlm::Copyright(void) const
{
  _LIT8(KAppCopyright,"(c) 2005-2006 by zg. version 0.13");
  return KAppCopyright;
}

void CAlm::AskWakupPhoneL(void) //checked
{
  iAlmFlags&=~EFlagAlarmKeyAllowed;
  iPlayStartTimer->Cancel();
  delete iAudio;
  iAudio=NULL;
  iAlmFlags|=EFlagAlarmRestartNoteActive;
  DeactivateCba();
  FadeBehindPopup(EFalse);
  ActivateCba(iButtonsYesNo);
  FadeBehindPopup(ETrue);
  iNoteContainer->SetWakeupLabelL();
  SetSizeAndPosition(TSize(176,78));
  iNoteContainer->Redraw();
  iButtonsYesNo->DrawNow();
  if(iDisplayedTimer->IsActive()) iDisplayedTimer->Cancel();
  iAlmFlags&=~EFlagAlarmAcknowledged;
  TCallBack callback(DisplayedCallBack,this);
  iDisplayedTimer->Start(500000,500000,callback);
}

void CAlm::FadeBehindPopup(TBool aFade) //checked
{
  iFader.FadeBehindPopup(this,this,aFade);
}

void CAlm::NotifyStateToSysApp(TBool aState) //checked
{
  iKeyguardTimer->Cancel();
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

void CAlm::NotifyStateBySnooze(void) //checked
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

TBool CAlm::CheckStartupReason(void) //checked
{
  TSWState state=SysStartup::State();
  TBool result=EFalse;
  if(state!=ESWState203&&state!=ESWState205&&state!=ESWState206) result=ETrue;
  if(result) iAlmFlags|=EFlagAlarmDeviceInInternalState;
  return result;
}

void CAlm::GetStartupReason(void)
{
  TWD2StartupReason reason1; TSWStartupReason reason2;
  SysStartup::GetStartupReasons(reason1,reason2);
  if(reason1==EWD2StartupAlarm||reason1==EWD2StartupCharger) iAlmFlags|=EFlagAlarmStartPhone;
  else iAlmFlags&=~EFlagAlarmStartPhone;
}

CEikButtonGroupContainer* CAlm::CreateCbaL(TInt aResourceId) //checked
{
  CEikButtonGroupContainer* self=CEikButtonGroupContainer::NewL(CEikButtonGroupContainer::ECba,CEikButtonGroupContainer::EHorizontal,this,aResourceId,iServerAppUi->AlertGroupWin(),CEikButtonGroupContainer::EUseMaxSize);
  self->MakeVisible(EFalse);
  return self;
}

//FIXME: function can leave
void CAlm::ActivateCba(CEikButtonGroupContainer* aButtons) //checked
{
  if(iButtonsCurrent) ((CEikonEnv*)ControlEnv())->RemoveFromStack(iButtonsCurrent->ButtonGroup()->AsControl());
  CCoeControl* control=aButtons->ButtonGroup()->AsControl();
  iButtonsCurrent=aButtons;
  ((CCoeAppUi*)ControlEnv()->AppUi())->AddToStackL(control,ECoeStackPriorityCba,ECoeStackFlagStandard);
  control->DrawableWindow()->SetOrdinalPosition(0);
  control->MakeVisible(ETrue);
  control->DrawNow();
}

void CAlm::DeactivateCba(void) //checked
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

TBool CAlm::CanSnooze(void) //not checked
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

TInt CAlm::AutoHideCallBack(TAny* anAlm) //checked
{
  return ((CAlm*)anAlm)->DoAutoHide();
}

TInt CAlm::DoAutoHide(void) //checked
{
  if((iAlmFlags&EFlagAlarmAcknowledged)==0)
  {
    CreateSnooze();
  }
  iAutoHideTimer->Cancel();
  return 0;
}

void CAlm::BringAlertGroupWinForwards(TBool aState) //checked
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

void CAlm::CreateSnooze(void) //checked
{
  iAlmFlags|=EFlagAlarmAcknowledged;
  if(!(iAlmFlags&EFlagAlarmRestartNoteActive))
  {
    if(iAlarmType==EAlarmTypeClock)
    {
      NotifyStateBySnooze();
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

TInt CAlm::DisplayedCallBack(TAny* anAlm)
{
  return ((CAlm*)anAlm)->DoUpdateFlag();
}

TInt CAlm::DoUpdateFlag(void)
{
  iAlmFlags|=EFlagAlarmKeyAllowed;
  iDisplayedTimer->Cancel();
  if(iAutoHideTimer->IsActive()) iAutoHideTimer->Cancel();
  TCallBack callback(AutoHideCallBack,this);
  iAutoHideTimer->Start(59000000,59000000,callback);
  return 0;
}

TInt CAlm::ScancodeCallback(TAny* anAlm)
{
  return ((CAlm*)anAlm)->DoScancodeCallback();
}

TInt CAlm::DoScancodeCallback(void)
{
  if(!(iAlmFlags&EFlagAlarmAcknowledged))
    if(iAlmFlags&EFlagAlarmKeyAllowed) CreateSnooze();
  iScancodeIdle->Cancel();
  return 0;
}

void StartAlmSettingsServerL(void);

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
  iAutoHideTimer=CPeriodic::NewL(CActive::EPriorityStandard);
  iDisplayedTimer=CPeriodic::NewL(CActive::EPriorityStandard);
  iScancodeIdle=CIdle::NewL(CActive::EPriorityIdle);
  iPlayStartTimer=CPeriodic::NewL(CActive::EPriorityIdle);
  iKeyguardTimer=CPeriodic::NewL(CActive::EPriorityStandard);

  StartAlmSettingsServerL();
  iSettings=new(ELeave)CSettings;

  iSysApNotify=new(ELeave)RSharedDataClient(this);
  User::LeaveIfError(iSysApNotify->Connect(0));
  User::LeaveIfError(iSysApNotify->NotifySet(KSysAppUid,NULL));
  iButtonsStopSnooze=CreateCbaL(R_ALERT_SOFTKEYS_STOP_SNOOZE);
  iButtonsYesNo=CreateCbaL(R_AVKON_SOFTKEYS_YES_NO);
  iButtonsCurrent=iButtonsStopSnooze;
  CCoeControl* buttons=iButtonsCurrent->ButtonGroup()->AsControl();
  ((CCoeAppUi*)ControlEnv()->AppUi())->AddToStackL(buttons,ECoeStackPriorityCba,ECoeStackFlagRefusesAllKeys|ECoeStackFlagRefusesFocus);
  SetComponentsToInheritVisibility(ETrue);
  MakeVisible(EFalse);
  iAlmFlags=0;
  TInt machine;
  User::LeaveIfError(HAL::Get(HALData::EModel,machine));
  if(machine==0x101F8C19) //n-gage
    iNoteController=iServerAppUi->iNoteController_ngage;
  else
    iNoteController=iServerAppUi->iNoteController;
  iNoteController->SetNoteObserver(this);
  GetStartupReason();
  CreateStateHandlerL();
}

void CAlm::DoShowAlarm(void) //checked
{
  NotifyStateToSysApp(ETrue);
  FadeBehindPopup(ETrue);
  RWindowGroup& alert=iServerAppUi->AlertGroupWin();
  iCaptureHandle=alert.CaptureKeyUpAndDowns(EStdKeyDevice6,EModifierFunc|EModifierShift|EModifierCtrl,0);
  BringAlertGroupWinForwards(ETrue);
  DrawableWindow()->SetOrdinalPosition(0);
  alert.SetOrdinalPosition(0);
  ClaimPointerGrab();
  MakeVisible(ETrue);
  ((CEikonEnv*)ControlEnv())->RouseSleepingDialog(this,ETrue);
  TRAPD(err,ActivateL());
  if(CanSnooze())
  {
    iAlmFlags|=EFlagAlarmSnoozeAllowed;
  }
  else
  {
    iAlmFlags&=~EFlagAlarmSnoozeAllowed;
  }
  iButtonsStopSnooze->MakeCommandVisible(0x6002,iAlmFlags&EFlagAlarmSnoozeAllowed);
  ActivateCba(iButtonsStopSnooze);
  if(iDisplayedTimer->IsActive()) iDisplayedTimer->Cancel();
  TCallBack callback(DisplayedCallBack,this);
  iDisplayedTimer->Start(500000,500000,callback);
  PlayStart();
}

void CAlm::DoCancelAlarm(void) //checked
{
  iPlayStartTimer->Cancel();
  delete iAudio;
  iAudio=NULL;
  iAlmFlags&=~EFlagAlarmKeyAllowed;
  FadeBehindPopup(EFalse);
  iDisplayedTimer->Cancel();
  iAutoHideTimer->Cancel();
  if(iAlmFlags&EFlagAlarmStartPhone)
  {
    NotifyStateToSysApp(EFalse);
  }
  else
  {
    iKeyguardTimer->Cancel();
    TCallBack callback(KeyguardCallBack,this);
    iKeyguardTimer->Start(500000,0,callback);
  }
  iServerAppUi->AlertGroupWin().CancelCaptureKeyUpAndDowns(iCaptureHandle);
  BringAlertGroupWinForwards(EFalse);
  MakeVisible(EFalse);
  DeactivateCba();
  ((CEikonEnv*)ControlEnv())->RouseSleepingDialog(this,EFalse);
}

void CAlm::CreateStateHandlerL(void) //checked
{
  if(!iDevStateNotify)
  {
    iDevStateNotify=new(ELeave)RSharedDataClient(this);
    User::LeaveIfError(iDevStateNotify->Connect(0));
    User::LeaveIfError(iDevStateNotify->NotifySet(KSysUtilUid,&KKeyStateVal));
  }
}

void CAlm::RemoveStateHandler(void) //checked
{
  if(iDevStateNotify) iDevStateNotify->Close();
  delete iDevStateNotify;
  iDevStateNotify=NULL;
}

void CAlm::PlayAlarm(void) //checked
{
  iPlayStartTimer->Cancel();
  TRAPD(err,iAudio=CAlmAudio::NewL((CEikonEnv*)ControlEnv(),iSettings,iAlarmType));
}

void CAlm::PlayStart(void) //checked
{
  iPlayStartTimer->Cancel();
  if(iAudio)
  {
    delete iAudio;
    iAudio=NULL;
  }
  iPlayStartTimer->Start(100000,100000,TCallBack(PlayStartCallback,this));
}

TInt CAlm::PlayStartCallback(TAny* anAlm)
{
  ((CAlm*)anAlm)->PlayAlarm();
  return KErrNone;
}

TInt CAlm::KeyguardCallBack(TAny* anAlm) //checked
{
  ((CAlm*)anAlm)->NotifyStateToSysApp(EFalse);
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
    CheckStartupReason();
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
    iNoteContainer->Redraw();
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
  iKeyguardTimer->Cancel();
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
  TRAPD(err,iNoteContainer->SetAlarmL(alarm));;
  if(err!=KErrNone) iSupervisor->CmdAcknowledgeAlarm();
  SetSizeAndPosition(TSize(176,78));
  iNoteContainer->NoteControl()->Layout();
  iNoteContainer->Redraw();
  DrawDeferred();
  iAlmFlags&=~EFlagAlarmAcknowledged;
  if((iAlmFlags&EFlagAlarmActive)&&!(iAlmFlags&EFlagAlarmRestartNoteActive)&&!(iAlmFlags&EFlagAlarmStartPhone))
  {
    iDisplayedTimer->Cancel();
    iDisplayedTimer->Start(500000,500000,TCallBack(DisplayedCallBack,this));
    PlayStart();
  }
}

TKeyResponse CAlm::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType) //checked
{
  if(aKeyEvent.iScanCode<EStdKeyDevice6&&aKeyEvent.iScanCode>EStdKeyDevice3) return EKeyWasNotConsumed;
  if(aType==EEventKeyDown)
  {
    iScancodeIdle->Cancel();
    iScancodeIdle->Start(TCallBack(ScancodeCallback,this));
  }
  if(aType==EEventKey)
  {
    iScancodeIdle->Cancel();
    if(!(iAlmFlags&EFlagAlarmAcknowledged))
    {
      if(iAlmFlags&EFlagAlarmKeyAllowed) CreateSnooze();
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
  if(!aIndex) return iNoteContainer->NoteControl();
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
  iNoteContainer->NoteControl()->SetRect(rect);
  iButtonsStopSnooze->SetBoundingRect(TRect(TPoint(0,0),ControlEnv()->ScreenDevice()->SizeInPixels()));
  iButtonsYesNo->SetBoundingRect(TRect(TPoint(0,0),ControlEnv()->ScreenDevice()->SizeInPixels()));
}

void CAlm::SetSizeAndPosition(const TSize& aSize) //checked
{
  CAknNoteControl* control=iNoteContainer->NoteControl();
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
        iNoteContainer->ClearWakeupLabel();
        iSupervisor->CmdAcknowledgeAlarm();
        SysStartup::SetState(KAlmAlertUid,ESWState207);
        break;
      case 3006: //off
        iAlmFlags&=~EFlagAlarmRestartNoteActive;
        iNoteContainer->ClearWakeupLabel();
        iSupervisor->CmdAcknowledgeAlarm();
        SysStartup::Shutdown(KAlmAlertUid);
        break;
      case 0x6001: //stop
        if(iAlmFlags&EFlagAlarmStartPhone)
        {
          AskWakupPhoneL();
        }
        else
        {
          iSupervisor->CmdAcknowledgeAlarm();
          iAlmFlags&=~EFlagAlarmKeyAllowed;
          if(iDisplayedTimer->IsActive()) iDisplayedTimer->Cancel();
          iDisplayedTimer->Start(500000,500000,TCallBack(DisplayedCallBack,this));
        }
        break;
      case 0x6002: //snooze
        CreateSnooze();
        break;
    }
  }
}

void CAlm::HandleNotifyL(TUid anUid,const TDesC16& aKey,const TDesC16& aValue)
{
  if(anUid==KSysAppUid)
  {
    if(aKey==KKeyHide)
    {
      if(!(iAlmFlags&EFlagAlarmAcknowledged)&&iAlmFlags&EFlagAlarmActive) CreateSnooze();
    }
    else if(aKey==KKeyToneQuit)
    {
      if(aValue[0]==0x30&&iOnGui)
      {
        iOnGui->CancelBirthdayAudio();
      }
    }
  }
  else if(anUid==KSysUtilUid&&aKey==KKeyStateVal)
  {
    TSWState state=SysStartup::State();
    if((iAlmFlags&EFlagAlarmDeviceInInternalState)&&(state==ESWState203||state==ESWState205||state==ESWState206))
    {
      iAlmFlags&=~EFlagAlarmDeviceInInternalState;
      DoShowAlarm();
    }
    if(state==ESWState203)
    {
      iAlmFlags&=~EFlagAlarmStartPhone;
      RemoveStateHandler();
      TRAPD(err,OnGuiL());
    }
  }
}

void CAlm::OnGuiL(void)
{
  iOnGui=CAlmOnGui::NewL(*iSettings,*this,(CEikonEnv*)ControlEnv());
}

void CAlm::NoteCompleted(TInt aNoteId,TInt aCommand) //checked
{
}

TBool CAlm::DisplayDialogL(TInt aPriority) //checked
{
  if(aPriority!=0xC17) return EFalse;
  TBool res=ETrue;
  if(IsVisible()) return ETrue;
  if(!CheckStartupReason()) //показываем ноут, если устройство в окончательном состоянии.
  {
    DoShowAlarm();
    return res;
  }
  else return res;
}

TBool CAlm::CancelDialog(TInt aPriority) //checked
{
  iPlayStartTimer->Cancel();
  iDisplayedTimer->Cancel();
  iAutoHideTimer->Cancel();
  if(aPriority!=0xC17) return EFalse;
  if(IsVisible()) DoCancelAlarm();
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

TBool CAlm::AlarmActive(void)
{
  return iAlmFlags&EFlagAlarmActive;
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

void CNoteContainer::SetAlarmL(const TAlarmInfo& aInfo)
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
  iNote->SetTextL(KNullDesC,2);
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

void CNoteContainer::Redraw(void) const
{
  iNote->DrawDeferred();
}

void CNoteContainer::SetWakeupLabelL(void)
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

void CNoteContainer::ClearWakeupLabel(void) //checked
{
  iRestartNoteActive=EFalse;
  delete iRestartNote;
  iRestartNote=NULL;
}

CAknNoteControl* CNoteContainer::NoteControl(void) //checked
{
  if(!iRestartNoteActive) return iNote;
  else return iRestartNote;
}
