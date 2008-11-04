/*
    lineobserver.cpp
    Copyright (C) 2008 zg

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "lineobserver.hpp"
#include <hwtricks.hpp>
_LIT(KTsyName,"phonetsy.tsy");

void CLineObserver::NewLC(void)
{
  CLineObserver* self=new(ELeave)CLineObserver;
  CleanupStack::PushL(self);
  self->ConstructL();
}

CLineObserver::~CLineObserver()
{
  Cancel();
  Close();
}

void CLineObserver::ConstructL(void)
{
  User::LeaveIfError(iServer.Connect());
  User::LeaveIfError(iServer.LoadPhoneModule(KTsyName));
  TInt numberPhones;
  User::LeaveIfError(iServer.EnumeratePhones(numberPhones));
  if(numberPhones<1) User::Leave(KErrNotFound);
  RTelServer::TPhoneInfo info;
  User::LeaveIfError(iServer.GetPhoneInfo(0,info));
  User::LeaveIfError(iPhone.Open(iServer,info.iName));
  TInt lines=0,line=KErrNotFound;
  RPhone::TLineInfo infoLine;
  User::LeaveIfError(iPhone.EnumerateLines(lines));
  if(lines<=0) User::Leave(KErrNotFound);
  for(TInt ii=0;ii<lines;ii++)
  {
    User::LeaveIfError(iPhone.GetLineInfo(ii,infoLine));
    if(infoLine.iLineCapsFlags&RLine::KCapsVoice)
    {
      line=ii;
      break;
    }
  }
  User::LeaveIfError(line);
  User::LeaveIfError(iLine.Open(iPhone,infoLine.iName));
  iLine.NotifyAdvGsmLineStatusChange(iStatus,iLineStatus);
  SetActive();
}

CLineObserver::CLineObserver(): CActive(EPriorityIdle),iPrevLineStatus(MAdvGsmLineStatusAndCallInfo::EGsmStatusIdle)
{
  CActiveScheduler::Add(this);
}

void CLineObserver::Close(void)
{
  iLine.Close();
  iPhone.Close();
  iServer.Close();
}

void CLineObserver::RunL(void)
{
  if(iStatus==KErrNone)
  {
    if(MAdvGsmLineStatusAndCallInfo::EGsmStatusConnected==iLineStatus&&MAdvGsmLineStatusAndCallInfo::EGsmStatusAnswering!=iPrevLineStatus) Vibrate();
    iPrevLineStatus=iLineStatus;
    iLine.NotifyAdvGsmLineStatusChange(iStatus,iLineStatus);
    SetActive();
  }
}

void CLineObserver::DoCancel(void)
{
  iLine.NotifyAdvGsmLineStatusChangeCancel();
}

void CLineObserver::Vibrate(void)
{
  TRAPD(err,VibrateL());
}

void CLineObserver::VibrateL(void)
{
  HWVibra::SwitchL(ETrue);
  User::After(100000);
  HWVibra::SwitchL(EFalse);
}
