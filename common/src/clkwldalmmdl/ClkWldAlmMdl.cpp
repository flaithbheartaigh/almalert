/*
    ClkWldAlmMdl.cpp
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

#include <ClkWldAlmMdl.hpp>
#include <eikdll.h>

EXPORT_C CClkAlmIdleObserver* CClkAlmIdleObserver::NewL(MClkModelObserver* aObserver,TInt aPriority)
{
  CClkAlmIdleObserver* self=new(ELeave)CClkAlmIdleObserver;
  CleanupStack::PushL(self);
  self->ConstructL(aObserver,aPriority);
  CleanupStack::Pop(); //self
  return self;
}

EXPORT_C CClkAlmModel* CClkAlmModel::NewL(MClkModelObserver* aObserver,TInt aPriority)
{
  CClkAlmModel* self=new(ELeave)CClkAlmModel;
  CleanupStack::PushL(self);
  self->ConstructL(aObserver,aPriority);
  CleanupStack::Pop(); //self
  return self;
}

EXPORT_C CClkAlmModel::~CClkAlmModel()
{
  Stop();
  iServer.Close();
}

void CClkAlmModel::DoNotifyL(TInt aNotification)
{
  if(aNotification!=KErrCancel&&aNotification!=KErrNone)
    NotifyL(aNotification);
  else
    NotifyL(KErrNone);
}

EXPORT_C TInt CClkAlmModel::ClockAlarmDelete(TInt aAlarmId)
{
  return iServer.AlarmDelete(aAlarmId);
}

EXPORT_C TInt CClkAlmModel::ClockAlarmEnable(TInt aAlarmId,TAlarmSetState aState)
{
  TInt err;
  if(aState==EAlarmSet)
  {
    err=iServer.AlarmEnable(EFalse,aAlarmId);
    if(err==KErrNone)
    {
      err=iServer.AlarmEnable(ETrue,aAlarmId);
    }
  }
  else if(aState==EAlarmNotSet)
  {
    err=ClockAlarmDelete(aAlarmId);
  }
  else
  {
    err=iServer.AlarmEnable(EFalse,aAlarmId);
  }
  return err;
}

EXPORT_C TAlarmSetState CClkAlmModel::ClockAlarmState(TInt aAlarmId) const
{
  return iServer.AlarmState(aAlarmId);
}

EXPORT_C TInt CClkAlmModel::ClockAlarmType(TAlarmType& aType,TInt aAlarmId) const
{
  return iServer.AlarmType(aType,aAlarmId);
}

EXPORT_C void CClkAlmModel::ClockAlarmSet(TInt aClockAlarmIndex,const SClockAlarmInfo& anInfo)
{
  iServer.SetClockAlarm(aClockAlarmIndex,anInfo.iAlarmTime,anInfo.iMessage,anInfo.iSound,anInfo.iRepeat);
}

EXPORT_C TInt CClkAlmModel::ClockAlarmInfo(TInt aClockAlarmIndex,SClockAlarmInfo& anInfo) const
{
  TAlarmInfo info;
  TInt err=iServer.AlarmInfo(info,RAlarmServer::EInfoClock,aClockAlarmIndex);
  if(err==KErrNone)
  {
    anInfo.iRepeat=info.iRepeat;
    anInfo.iAlarmTime=info.iAlarmTime;
    anInfo.iMessage=info.iMessage;
    anInfo.iSound=info.iSound;
    anInfo.iState=iServer.AlarmState(info.iAlarmId);
  }
  return err;
}

void CClkAlmModel::ConstructL(MClkModelObserver* aObserver,TInt aPriority)
{
  SetClkModelObserver(aObserver);
  User::LeaveIfError(iServer.Connect());
  CClkAlmResponder* responder=new(ELeave)CClkAlmResponder(&iServer,this,aPriority);
  SetResponderActive(responder);
}

CClkAlmResponder::CClkAlmResponder(RAlarmServer* aServer,CClkAlmModel* aModel,TInt aPriority): CClkMdlResponder(*aModel,aPriority),iServer(aServer)
{
}

void CClkAlmResponder::DoStart(void)
{
  iServer->NotifyOnChange(iStatus);
}

void CClkAlmResponder::DoCancel(void)
{
  iServer->NotifyOnChangeCancel();
}



EXPORT_C CClkIdleObserver::CClkIdleObserver(void): CClkIntermediateObserver()
{
}

EXPORT_C CClkIdleObserver::~CClkIdleObserver(void)
{
  delete iIdle;
}

EXPORT_C void CClkIdleObserver::ConstructL(MClkModelObserver* aObserver,TInt aPriority)
{
  iIdle=CIdle::NewL(aPriority);
  Cancel();
  SetClkModelObserver(aObserver);
}

void CClkIdleObserver::Cancel(void)
{
  if(iIdle) iIdle->Cancel();
  DoIdleCancel();
}

EXPORT_C void CClkIdleObserver::HandleUpdateL(TInt aNotification)
{
  iNotification=aNotification;
  if(iNotification==KErrServerTerminated) DoIdleNotifyL();
  else Start();
}

EXPORT_C void CClkIdleObserver::DoIdleCancel(void)
{
  iNotification=KErrCancel;
}

EXPORT_C void CClkIdleObserver::DoIdleNotifyL(void)
{
  NotifyL(iNotification);
}

void CClkIdleObserver::Start(void)
{
  if(!iIdle) Fault(0);
  if(!iIdle->IsActive())
  {
    TCallBack callback(IdleTimeout,this);
    iIdle->Start(callback);
  }
}

TInt CClkIdleObserver::IdleTimeout(TAny* aObserver)
{
  STATIC_CAST(CClkIdleObserver*,aObserver)->DoIdleNotifyL();
  return KErrNone;
}


CClkIntermediateObserver::CClkIntermediateObserver(): CBase()
{
}

CClkIntermediateObserver::~CClkIntermediateObserver()
{
}

EXPORT_C void CClkIntermediateObserver::SetClkModelObserver(MClkModelObserver* aObserver)
{
  if(!aObserver) Panic(2);
  iObserver=aObserver;
}

EXPORT_C void CClkIntermediateObserver::HandleUpdateL(TInt aNotification)
{
  if(!iObserver) Panic(2);
  NotifyL(aNotification);
}

EXPORT_C void CClkIntermediateObserver::NotifyL(TInt aNotification)
{
  iObserver->HandleUpdateL(aNotification);
}

void CClkIntermediateObserver::Panic(TInt aCode)
{
  _LIT(KPanic,"CLK-ENG-PANIC");
  User::Panic(KPanic,aCode);
}

void CClkIntermediateObserver::Fault(TInt aCode)
{
  _LIT(KPanic,"CLK-ENG-FAULT");
  User::Panic(KPanic,aCode);
}

EXPORT_C CClkMdlResponder::CClkMdlResponder(CClkModelBase& aClkModel,TInt aPriority): CActive(aPriority),iClkModel(&aClkModel)
{
}

void CClkMdlResponder::NotifyL(TInt aNotification)
{
  iClkModel->DoNotifyL(aNotification);
}

EXPORT_C void CClkMdlResponder::RunL(void)
{
  TInt status=iStatus.Int();
  Start();
  NotifyL(status);
}

void CClkMdlResponder::Start(void)
{
  if(!IsActive())
  {
    SetActive();
    DoStart();
  }
}

EXPORT_C CClkModelBase::CClkModelBase(void): CBase()
{
}

EXPORT_C CClkModelBase::~CClkModelBase(void)
{
  delete iResponder;
}

EXPORT_C void CClkModelBase::Stop(void)
{
  if(iResponder) iResponder->Cancel();
}

EXPORT_C void CClkModelBase::Start(void)
{
  iResponder->Start();
}

EXPORT_C void CClkModelBase::SetClkModelObserver(MClkModelObserver* aObserver)
{
  iObserver=aObserver;
}

EXPORT_C void CClkModelBase::NotifyL(TInt aNotification)
{
  if(!iProgress&&iObserver) HandleUpdateL(aNotification);
}

EXPORT_C void CClkModelBase::SetResponderActive(CClkMdlResponder* aResponder)
{
  iResponder=aResponder;
  if(iResponder) CActiveScheduler::Add(iResponder);
}


void CClkModelBase::HandleUpdateL(TInt aNotification)
{
  iProgress=ETrue;
  iObserver->HandleUpdateL(aNotification);
  iProgress=EFalse;
}

EXPORT_C CClkNitzIdleObserver* CClkNitzIdleObserver::NewL(MClkModelObserver* aObserver,TInt aPriority)
{
  CClkNitzIdleObserver* self=new(ELeave)CClkNitzIdleObserver;
  CleanupStack::PushL(self);
  self->ConstructL(aObserver,aPriority);
  CleanupStack::Pop(); //self
  return self;
}

EXPORT_C CClkNitzModel* CClkNitzModel::NewL(MClkModelObserver* aObserver,TInt aPriority)
{
  CClkNitzModel* self=new(ELeave)CClkNitzModel;
  CleanupStack::PushL(self);
  self->ConstructL(aObserver,aPriority);
  CleanupStack::Pop(); //self
  return self;
}

EXPORT_C CClkNitzModel::~CClkNitzModel()
{
  Stop();
  iServer.Close();
}

void CClkNitzModel::DoNotifyL(TInt aNotification)
{
  if(aNotification!=KErrCancel&&aNotification!=KErrNone)
    NotifyL(aNotification);
  else
    NotifyL(KErrNone);
}

EXPORT_C TInt CClkNitzModel::NitzState(TInt& aSetting) const
{
  return iServer.GetNitzSetting(aSetting);
}

EXPORT_C TInt CClkNitzModel::SetNitzState(TInt aSetting)
{
  return iServer.ChangeNitzSetting(aSetting);
}

EXPORT_C TInt CClkNitzModel::GetCurrentNITZInfo(MAdvGsmPhoneNitz::TNITZInfo& aInfo)
{
  return iServer.GetCurrentNITZInfo(aInfo);
}

void CClkNitzModel::ConstructL(MClkModelObserver* aObserver,TInt aPriority)
{
  StartServer();
  SetClkModelObserver(aObserver);
  TInt i=0;
  while(iServer.Connect()!=KErrNone)
  {
    User::After(1000000);
    i++;
    if(i==6) User::Leave(KErrServerTerminated);
  }
  CClkNitzResponder* responder=new(ELeave)CClkNitzResponder(&iServer,this,aPriority);
  SetResponderActive(responder);
}

TInt CClkNitzModel::FindServer(void)
{
  TInt err=KErrNone;
  _LIT(KServer,"ClkNitzMdlServer");
  _LIT(KSem,"ClkNitzMdlStartSemaphore");
  TFindServer find(KServer);
  TFullName name;
  if(find.Next(name)!=KErrNone)
  {
    RSemaphore sem;
    TInt i=0;
    do
    {
      err=sem.OpenGlobal(KSem);
      if(err==KErrNone)
      {
        sem.Close();
        break;
      }
      User::After(1000000);
      i++;
    } while(i<6);
  }
  return err;
}

void CClkNitzModel::StartServer(void)
{
  _LIT(KExe,"Z:\\System\\programs\\ClkNitzMdls.exe");
  if(FindServer()!=KErrNone) EikDll::StartExeL(KExe);
}

CClkNitzResponder::CClkNitzResponder(RClkNitzMdlServer* aServer,CClkNitzModel* aModel,TInt aPriority): CClkMdlResponder(*aModel,aPriority),iServer(aServer)
{
}

void CClkNitzResponder::DoStart(void)
{
  iServer->NotifyOnChange(iStatus);
}

void CClkNitzResponder::DoCancel(void)
{
  iServer->NotifyOnChangeCancel();
}
