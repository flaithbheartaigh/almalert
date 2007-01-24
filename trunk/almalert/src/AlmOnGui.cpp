/*
    AlmOnGui.cpp
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

#include "AlmOnGui.hpp"
#include "AlmUtils.hpp"
#include <BtEng.hpp> //CBTMCMSettings

CAlmOnGui* CAlmOnGui::NewL(CSettings& aSettings,MAlmInterface& anAlmInterface,CEikonEnv* anEnv)
{
  CAlmOnGui* self=new(ELeave)CAlmOnGui(aSettings,anAlmInterface,anEnv);
  CleanupStack::PushL(self);
  self->ConstructL();
  CleanupStack::Pop(); //self
  return self;
}

CAlmOnGui::~CAlmOnGui()
{
  CancelBeeper();
  CancelBirthday();
}

void CAlmOnGui::RunL(void)
{
  if(iStatus==KErrNone) NotifyL();
}

void CAlmOnGui::DoCancel(void)
{
  iSettings.NotifyCancel();
}

CAlmOnGui::CAlmOnGui(CSettings& aSettings,MAlmInterface& anAlmInterface,CEikonEnv* anEnv): CActive(EPriorityIdle),iSettings(aSettings),iAlmInterface(anAlmInterface),iEnv(anEnv)
{
}

void CAlmOnGui::ConstructL(void)
{
  CActiveScheduler::Add(this);
  NotifyL();
}

void CAlmOnGui::NotifyL(void)
{
  TBool beep=iSettings.IsBeep(),birthday=iSettings.IsBirthday();
  TInt hour=iSettings.BirthdayHour();
  if(beep!=iBeepActive)
  {
    if(beep) InitBeeperL();
    else CancelBeeper();
    iBeepActive=beep;
  }
  if(birthday!=iBirthdayActive)
  {
    if(birthday) InitBirthdayL();
    else CancelBirthday();
    iBirthdayActive=birthday;
  }
  else if(iBirthdayActive&&hour!=iBirthdayHour)
  {
    CancelBirthday();
    InitBirthdayL();
  }
  iSettings.Notify(iStatus);
  SetActive();
}

TInt CAlmOnGui::OnActivity(TAny* anAlm)
{
  ((CAlmOnGui*)anAlm)->iUserActive=ETrue;
  return 0;
}

TInt CAlmOnGui::OnInactivity(TAny* anAlm)
{
  ((CAlmOnGui*)anAlm)->iUserActive=EFalse;
  return 0;
}

void CAlmOnGui::SetBeeper(void) //устанавливает таймер до следующего часа
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
    SetBeeper();
  }
}

void CAlmOnGui::InitBeeperL(void)
{
  iActivity=CUserActivityManager::NewL(EPriorityLow);
  iActivity->Start(10,TCallBack(OnInactivity,this),TCallBack(OnActivity,this));
  iBeeper=CPeriodic::NewL(CActive::EPriorityIdle);
  SetBeeper();
}

void CAlmOnGui::DoBeeperTimeout(void)
{
  iBeeper->Cancel();
  if(iBeepAudio)
  {
    delete iBeepAudio;
    iBeepAudio=NULL;
    SetBeeper();
  }
  else
  {
    if(!iUserActive)
    {
      TTime time;
      time.HomeTime();
      time.RoundUpToNextMinute();
      TDateTime dtime=time.DateTime();
      TInt hour=dtime.Hour(),minute=dtime.Minute();
      if(!(iAlmInterface.AlarmActive())&&minute<2&&hour>=iSettings.BeepStart()&&hour<=iSettings.BeepFinish())
      {
        TBool btState=EFalse;
        CBTMCMSettings::GetPowerStateL(btState); //never leave. on error btState unchanged.
        if(!btState)
        {
          TRAPD(err,iBeepAudio=CAlmAudioBeep::NewL(iEnv,&iSettings));
        }
      }
    }
    if(iBeepAudio)
    {
      TCallBack callback(BeeperTimeout,this);
      iBeeper->Start(5000000,0,callback);
    }
    else SetBeeper();
  }
}

TInt CAlmOnGui::BeeperTimeout(TAny* anAlm)
{
  ((CAlmOnGui*)anAlm)->DoBeeperTimeout();
  return 0;
}

void CAlmOnGui::CancelBeeper(void)
{
  delete iBeeper;
  iBeeper=NULL;
  delete iBeepAudio;
  iBeepAudio=NULL;
  delete iActivity;
  iActivity=NULL;
}

void CAlmOnGui::InitBirthdayL(void)
{
  iBirthdayHour=iSettings.BirthdayHour();
  TCallBack callback(BirthdayTimeout,this);
  iBirthday=CBirthdayTimer::NewL(iBirthdayHour,callback);
}

void CAlmOnGui::DoBirthdayTimeoutL(void)
{
  CancelBirthdayAudio();
  TInt count;
  AlmUtils::ProcessBirthdaysL(iSettings.BirthdayStart(),count);
  if(count)
  {
    iBirthdayAudio=CAlmAudioSms::NewL(iEnv,&iSettings);
  }
}

TInt CAlmOnGui::BirthdayTimeout(TAny* anAlm)
{
  TRAPD(err,((CAlmOnGui*)anAlm)->DoBirthdayTimeoutL());
  return 0;
}

void CAlmOnGui::CancelBirthday(void)
{
  delete iBirthday;
  iBirthday=NULL;
  delete iBirthdayAudio;
  iBirthdayAudio=NULL;
}

void CAlmOnGui::CancelBirthdayAudio(void)
{
  delete iBirthdayAudio;
  iBirthdayAudio=NULL;
}
