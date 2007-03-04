/*
    clockappview.cpp
    Copyright (C) 2006-2007 zg

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

#include <aknquerydialog.h>
#include <stringloader.h> //StringLoader
#include <aknnotewrappers.h> //CAknConfirmationNote
#include <clockapp.rsg>
#include "clockapp.hpp"
#include "clockapp.hrh"
#include <hal.h>

_LIT(KAlarmSound,"a");

CClkAlmView* CClkAlmView::NewLC(CClkAlmModel* anAlm,CClkNitzModel* aNitz)
{
  CClkAlmView* self=new(ELeave)CClkAlmView;
  CleanupStack::PushL(self);
  self->ConstructL(anAlm,aNitz);
  return self;
}

CClkAlmView::~CClkAlmView()
{
  delete iControl;
  delete iAlmObserver;
  delete iNitzObserver;
}

TBool CClkAlmView::OpenedByOk(void) const
{
  return iOpenedByOk;
}

void CClkAlmView::SetOpenedByOk(TBool aOpenedByOk)
{
  iOpenedByOk=aOpenedByOk;
}

void CClkAlmView::HandleSettingsChangeL(TClkEnvChanged anEvent)
{
  if(iControl)
  {
    UpdateAlarmStateL(1);
    UpdateNitzStateL(1);
    iControl->DrawNow();
  }
}

TBool CClkAlmView::UpdateNitzStateL(TInt aParam)
{
  TInt state;
  User::LeaveIfError(iNitz->NitzState(state));
  if(!aParam)
  {
    if(iNitzSettings==state) return EFalse;
  }
  iNitzSettings=state;
  iControl->SetNitzInfoL();
  return EFalse;
}

CClkNitzModel* CClkAlmView::NitzMdl(void)
{
  return iNitz;
}

TInt CClkAlmView::NitzState(void)
{
  return iNitzSettings;
}

TUid CClkAlmView::Id(void) const
{
  return KClkAlmViewId;
}

void CClkAlmView::HandleCommandL(TInt aCommand)
{
  switch(aCommand)
  {
    case EClockAppSet:
    case EClockAppReset:
      QueryAlarmTimeL();
      break;
    case EClockAppRemove:
      HandleDeleteCmdL(0,0);
      break;
    case EClockAppSettings:
      iClkAppUi->CmdSettingsL();
      break;
    case EClockAppHelp:
      if(iIsSx1) iClkAppUi->CmdExit();
      else iClkAppUi->CmdHelpL();
      break;
    case EAknSoftkeyExit:
    case EClockAppExit:
      iClkAppUi->CmdExit();
      break;
    case EClockAppExtraStopWatch:
      iClkAppUi->CmdStopWatchL();
      break;
    case EClockAppExtraSettings:
      iClkAppUi->CmdSettingsExL();
      break;
    case EClockAppExtraLocale:
      iClkAppUi->CmdLocaleL();
      break;
    case EClockAppExtraCompactDB:
      iClkAppUi->CmdCompactDBL();
      break;
  }
}

void CClkAlmView::DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane)
{
  if(aResourceId==0x10931009)
  {
    aMenuPane->AddMenuItemsL(R_CLOCKAPP_EXTRA,EClockAppSettings);
    if(IsSnoozedAlarm()||iAlm->ClockAlarmState(0)==1)
    {
      aMenuPane->DeleteMenuItem(EClockAppSet);
    }
    else
    {
      aMenuPane->DeleteMenuItem(EClockAppReset);
      aMenuPane->DeleteMenuItem(EClockAppRemove);
    }
    if(OpenedByOk())
    {
      aMenuPane->DeleteMenuItem(EClockAppHelp);
      if(!iIsSx1) aMenuPane->DeleteMenuItem(EClockAppExit);
      SetOpenedByOk(EFalse);
    }
  }
}

void CClkAlmView::HandleUpdateL(TInt aNotification)
{
  if(aNotification!=KErrServerTerminated)
  {
    TBool res1=UpdateAlarmStateL(0),res2=UpdateNitzStateL(0);
    if(res1||res2)
    {
      if(iControl) iControl->DrawNow();
    }
  }
}

void CClkAlmView::DoActivateL(const TVwsViewId& aPrevViewId,TUid aCustomMessageId,const TDesC8& aCustomMessage)
{
  User::LeaveIfError(iAlm->ClockAlarmInfo(0,iAlarmInfo));
  iSnoozed=IsSnoozedAlarm();
  User::LeaveIfError(iNitz->NitzState(iNitzSettings));
  if(!iAlmObserver)
  {
    iAlmObserver=CClkAlmIdleObserver::NewL(this,-16);
    iAlm->SetClkModelObserver(iAlmObserver);
  }
  if(!iNitzObserver)
  {
    iNitzObserver=CClkNitzIdleObserver::NewL(this,-5);
    iNitz->SetClkModelObserver(iNitzObserver);
  }
  if(!iControl)
  {
    iControl=CClkAlmViewContainer::NewL(ClientRect(),*this);
    iControl->SetMopParent(this);
    iClkAppUi->AddToStackL(*this,iControl);
  }
  ShowTitlePaneL();
  UpdateAlarmStateL(1);
  UpdateNitzStateL(1);
  iControl->ActivateL();
}

void CClkAlmView::DoDeactivate(void)
{
  if(iAlmObserver)
  {
    iAlm->SetClkModelObserver(NULL);
    delete iAlmObserver;
    iAlmObserver=NULL;
  }
  if(iNitzObserver)
  {
    iNitz->SetClkModelObserver(NULL);
    delete iNitzObserver;
    iNitzObserver=NULL;
  }
  if(iControl)
  {
    iClkAppUi->RemoveFromStack(iControl);
    delete iControl;
  }
  iControl=NULL;
}

void CClkAlmView::ConstructL(CClkAlmModel* anAlm,CClkNitzModel* aNitz)
{
  BaseConstructL(0x10931007);
  iAlm=anAlm;
  iNitz=aNitz;
  TInt machine;
  if(HAL::Get(HALData::EModel,machine)==KErrNone&&machine==0x101f9071) iIsSx1=ETrue;
}

void CClkAlmView::ShowTitlePaneL(void)
{
  iClkAppUi->ShowTitlePaneL(0x10931004); //FIXME
}

TBool CClkAlmView::UpdateAlarmStateL(TInt aParam)
{
  SClockAlarmInfo info;
  iSnoozed=IsSnoozedAlarm();
  User::LeaveIfError(iAlm->ClockAlarmInfo(iSnoozed?1:0,info));
  if(!aParam&&iAlarmInfo.iState==info.iState)
  {
    if(!(iAlarmInfo.iAlarmTime!=info.iAlarmTime)) return EFalse;
  }
  iAlarmInfo=info;
  if(iSnoozed)
  {
    iControl->ShowSnoozedAlarmL(iAlarmInfo.iAlarmTime);
  }
  else
  {
    if(iAlarmInfo.iState==EAlarmSet)
    {
      iControl->ShowActiveAlarmL(iAlarmInfo.iAlarmTime);
    }
    else
    {
      iControl->ShowInactiveAlarm();
    }
  }
  return ETrue;
}

void CClkAlmView::QueryAlarmTimeL(void) //ok
{
  CClkDocument* document=(CClkDocument*)(AppUi()->Document());
  TTime alarm,alarmOld;
  if(iAlarmInfo.iState==EAlarmSet)
  {
    alarm=iAlarmInfo.iAlarmTime;
  }
  else
  {
    alarm=document->GetLastAlarmTime();
  }
  alarmOld=alarm;
  CAknTimeQueryDialog* dialog=new(ELeave)CAknTimeQueryDialog(alarm/*,tone*/);
  if(dialog->ExecuteLD(0x10931010))
  {
    if(iSnoozed)
    {
      TInt changed=EFalse;
      if(alarmOld!=alarm) changed=ETrue;
      HandleDeleteCmdL(1,changed);
    }
    SClockAlarmInfo info;
    info.iAlarmTime=alarm;
    info.iRepeat=EAlarmClockNext24hours;
    info.iSound.Copy(KAlarmSound);
    iAlm->ClockAlarmSet(0,info);
    User::LeaveIfError(iAlm->ClockAlarmEnable(0,EAlarmSet));
    document->SetLastAlarmTime(alarm);
    TRAPD(err,document->StoreDataL());
    ShowRemainTimeL(info.iAlarmTime);
  }
}

void CClkAlmView::HandleDeleteCmdL(TInt aParam1,TInt aParam2)
{
  if(aParam1==0)
  {
    CAknQueryDialog* dialog=CAknQueryDialog::NewL();
    if(!dialog->ExecuteLD(0x1093100F)) return;
  }
  TInt alarmId=iSnoozed;
  if(alarmId) alarmId=1;
  if(iAlm->ClockAlarmState(alarmId)==EAlarmSet)
  {
    User::LeaveIfError(iAlm->ClockAlarmEnable(alarmId,EAlarmDisabled));
  }
  if(iSnoozed)
  {
    iSnoozed=EFalse;
    if(aParam2)
    {
      UpdateAlarmStateL(1);
      if(iControl) iControl->DrawNow();
    }
  }
}

void CClkAlmView::ShowRemainTimeL(const TTime& anAlarmTime)
{
  TTime time;
  time.HomeTime();
  TDateTime timeS=time.DateTime();
  TDateTime alarmTimeS=anAlarmTime.DateTime();
  alarmTimeS.SetYear(timeS.Year());
  alarmTimeS.SetMonth(timeS.Month());
  alarmTimeS.SetDay(timeS.Day());
  TTime time2(alarmTimeS);
  if(time2<time) time2+=TTimeIntervalDays(1);
  TInt minutes=(time2.MicroSecondsFrom(time).Int64()/TInt64(60000000)).GetTInt();
  CArrayFixFlat<TInt>* array=new(ELeave)CArrayFixFlat<TInt>(2);
  CleanupStack::PushL(array);
  TInt hours=minutes/60;
  minutes=minutes-hours*60;
  array->AppendL(hours);
  array->AppendL(minutes);

  //FIXME: fix this stupid code
  HBufC* string;
  if(hours==1&&minutes==1) string=StringLoader::LoadLC(0x10931011,*array,iCoeEnv);
  else if(hours==1&&minutes!=1) string=StringLoader::LoadLC(0x10931013,*array,iCoeEnv);
  else if(hours!=1&&minutes==1) string=StringLoader::LoadLC(0x10931014,*array,iCoeEnv);
  else string=StringLoader::LoadLC(0x10931012,*array,iCoeEnv);
/*  if(hours==1)
  {
    if(minutes==1) string=StringLoader::LoadLC(0x10931011,*array,iCoeEnv); //FIXME
    else string=StringLoader::LoadLC(0x10931013,*array,iCoeEnv); //FIXME
  }
  else
  {
    if(minutes==1) string=StringLoader::LoadLC(0x10931014,*array,iCoeEnv); //FIXME
    else string=StringLoader::LoadLC(0x10931012,*array,iCoeEnv); //FIXME
  }*/
  // string=StringLoader::LoadLC(((hours==1)?((minutes==1)?0x10931011:0x10931013):((minutes==1)?0x10931014:0x10931012)),*array,iCoeEnv); //FIXME
  CAknConfirmationNote* note=new(ELeave)CAknConfirmationNote;
  note->ExecuteLD(*string);
  CleanupStack::PopAndDestroy(2); //string,array
}

TBool CClkAlmView::IsSnoozedAlarm(void)
{
  TAlarmSetState alm0=iAlm->ClockAlarmState(0),alm1=iAlm->ClockAlarmState(1);
  TBool res=EFalse;
  if(alm0!=EAlarmSet&&alm1==EAlarmSet) res=ETrue;
  return res;
}
