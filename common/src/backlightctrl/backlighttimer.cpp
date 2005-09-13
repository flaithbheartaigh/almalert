#include "backlighttimer.hpp"

CBackLightTimer* CBackLightTimer::NewL(MTimeOutNotify* aTimeOutNotify,TUint aParam)
{
  CBackLightTimer* self=new(ELeave)CBackLightTimer(aTimeOutNotify,aParam);
  CleanupStack::PushL(self);
  self->ConstructL();
  CleanupStack::Pop();
  return self;
}

CBackLightTimer::CBackLightTimer(MTimeOutNotify* aTimeOutNotify,TUint aParam): CTimer(EPriorityHigh),iNotify(aTimeOutNotify),iUnknown1(0),iParam(aParam)
{
//  if(!aTimeOutNotify) User::Invariant();
}

CBackLightTimer::~CBackLightTimer()
{
//  iNotify=NULL;
}

TInt CBackLightTimer::Start(TInt anInterval)
{
  if(!CActiveScheduler::Current()) User::Invariant();
  if(!IsAdded()) CActiveScheduler::Add(this);
  if(IsActive()) Cancel();
  if(anInterval) After(anInterval*1000);
//  else After(15000000);
  return KErrNone;
}

void CBackLightTimer::RunL(void)
{
//  if(!iNotify) User::Invariant();
  iNotify->TimerExpired(iParam);
}

void CBackLightTimer::DoCancel(void)
{
}

CBackLightTimerContainer* CBackLightTimerContainer::NewL(MBackLightNotify* aCallback,TInt aType,CLightDrv* aDrv)
{
  CBackLightTimerContainer* self=new(ELeave)CBackLightTimerContainer(aCallback,aType,aDrv);
  CleanupStack::PushL(self);
  self->ConstructL();
  CleanupStack::Pop(); //self
  return self;
}

CBackLightTimerContainer::CBackLightTimerContainer(MBackLightNotify* aCallback,TInt aType,CLightDrv* aDrv): iState(0),iUnknown02(0),iUnknown03(0),iUnknown04(0),iUnknown05(0),iUnknown06(0),iType(aType),iBlink(0),iUnknown09(0),iUnknown0a(0),iUnknown0b(0),iUnknown0c(0),iUnknown0d(0),iCallback(aCallback),iDrv(aDrv)
{
  if(!aDrv) User::Invariant();
}

void CBackLightTimerContainer::ConstructL(void)
{
  iCommonTimer=CBackLightTimer::NewL(this,2);
  iBlinkTimer=CBackLightTimer::NewL(this,0);
  iBlinkPeriodTimer=CBackLightTimer::NewL(this,1);
}

CBackLightTimerContainer::~CBackLightTimerContainer()
{
  iCommonTimer->Cancel();
  delete iCommonTimer;
  iCommonTimer=NULL;
  iBlinkTimer->Cancel();
  delete iBlinkTimer;
  iBlinkTimer=NULL;
  iBlinkPeriodTimer->Cancel();
  delete iBlinkPeriodTimer;
  iBlinkPeriodTimer=NULL;
  iDrv=NULL;
  iCallback=NULL;
}

TInt CBackLightTimerContainer::BackLightState(void)
{
  return iState;
}

TInt CBackLightTimerContainer::StartBlink(TUint16 aDuration,TUint16 aOnTime,TUint16 aOffTime)
{
  if(!iBlinkTimer) User::Invariant();
  if(!iBlinkPeriodTimer) User::Invariant();
  if(!iDrv) User::Invariant();
  iBlinkStartTime.HomeTime();
  if(aOnTime==0||aOffTime==0) return KErrArgument;
  if(iBlink) CancelBlink();
  TInt res=iBlinkTimer->Start(aDuration);
  if(iState==0)
  {
    TInt value=1;
    iUnknown02=value;
    res=iBlinkPeriodTimer->Start(aOffTime);
    iDrv->SwitchLight(NormalizeType(iType),NormalizeState(value));
  }
  else if(iState==1)
  {
    TInt value=0;
    iUnknown02=value;
    res=iBlinkPeriodTimer->Start(aOnTime);
    iDrv->SwitchLight(NormalizeType(iType),NormalizeState(value));
  }
  iBlink=1;
  iUnknown04=iState;
  iState=2;
  iUnknown0c=aDuration;
  iUnknown0b=aOffTime;
  iUnknown0a=aOnTime;
  if(iCallback) iCallback->BackLightNotify(iType,iState);
  return res;
}

TInt CBackLightTimerContainer::CancelBlink(void)
{
  if(!iBlinkTimer) User::Invariant();
  if(!iBlinkPeriodTimer) User::Invariant();
  if(!iDrv) User::Invariant();
  TInt value=0;
  iBlinkTimer->Cancel();
  iBlinkPeriodTimer->Cancel();
  iBlink=value;
  iDrv->SwitchLight(NormalizeType(iType),NormalizeState(iUnknown04));
  iState=iUnknown04;
  if(iCallback) iCallback->BackLightNotify(iType,iUnknown04);
  return value;
}

TInt CBackLightTimerContainer::BackLightSwitch(TInt aState,TUint16 aDuration)
{
  if(!iDrv) User::Invariant();
  if(!iCommonTimer) User::Invariant();
  iStartTime.HomeTime();
  if(iBlink) CancelBlink();
  iUnknown03=iState;
  iState=aState;
  TInt res=iCommonTimer->Start(aDuration);
  iUnknown0d=aDuration;
  if(aDuration==0)
  {
    iUnknown09=EFalse;
  }
  else
  {
    iUnknown09=ETrue;
  }
  iDrv->SwitchLight(NormalizeType(iType),NormalizeState(aState));
  if(iCallback) iCallback->BackLightNotify(iType,iState);
  return res;
}

TInt CBackLightTimerContainer::CancelCommon(void)
{
  if(!iDrv) User::Invariant();
  if(!iCommonTimer) User::Invariant();
  iCommonTimer->Cancel();
  iUnknown09=0;
  iDrv->SwitchLight(NormalizeType(iType),NormalizeState(iUnknown03));
  iState=iUnknown03;
  if(iCallback) iCallback->BackLightNotify(iType,iState);
  return 0;
}

TInt CBackLightTimerContainer::BackLightChange(TUint16 aDuration)
{
  if(!iDrv) User::Invariant();
  if(!iCommonTimer) User::Invariant();
  iStartTime.HomeTime();
  if(iBlink) CancelBlink();
  iUnknown03=iState;
  if(iState==0)
  {
    iState=1;
  }
  else
  {
    iState=0;
  }
  TInt res=iCommonTimer->Start(aDuration);
  iUnknown0d=aDuration;
  if(aDuration==0)
  {
    iUnknown09=0;
  }
  else
  {
    iUnknown09=1;
  }
  iDrv->SwitchLight(NormalizeType(iType),NormalizeState(iState));
  if(iCallback) iCallback->BackLightNotify(iType,iState);
  return res;
}

void CBackLightTimerContainer::PauseBackLight(void)
{
  if(!iDrv) User::Invariant();
  if(!iCommonTimer) User::Invariant();
  if(!iBlinkTimer) User::Invariant();
  if(!iBlinkPeriodTimer) User::Invariant();
  iDrv->SwitchLight(NormalizeType(iType),NormalizeState(0));
  TTime time;
  time.HomeTime();
  TInt64 zero(0);
  TTimeIntervalMicroSeconds period=zero;
  TInt64 divider(0U,1000U);
  if(iBlink)
  {
    period=time.MicroSecondsFrom(iBlinkStartTime);
    TInt64 ms=period.Int64()/divider;
    if(iUnknown0c>ms.Low())
    {
      iUnknown0c=iUnknown0c-ms.Low();
    }
    iBlinkTimer->Cancel();
    iBlinkPeriodTimer->Cancel();
    iUnknown06=iUnknown02;
  }
  if(iUnknown09)
  {
    period=time.MicroSecondsFrom(iStartTime);
    TInt64 ms=period.Int64()/divider;
    if(iUnknown0d>ms.Low())
    {
      iUnknown0d=iUnknown0d-ms.Low();
    }
    iCommonTimer->Cancel();
    if(iBlink==0)
    {
      iUnknown05=iState;
    }
    else
    {
      iUnknown05=iUnknown04;
    }
  }
  if(iUnknown09==0)
  {
    if(iBlink==0)
    {
      iUnknown05=iState;
    }
  }
}

TInt CBackLightTimerContainer::ResumeBackLight(void)
{
  if(!iDrv) User::Invariant();
  if(!iCommonTimer) User::Invariant();
  if(!iBlinkTimer) User::Invariant();
  if(!iBlinkPeriodTimer) User::Invariant();
  TInt res=KErrNone;
  if(iUnknown09)
  {
    res=iCommonTimer->Start(iUnknown0d);
    iDrv->SwitchLight(NormalizeType(iType),NormalizeState(iUnknown05));
    iState=iUnknown05;
  }
  if(iBlink)
  {
    iBlinkTimer->Start(iUnknown0c);
    if(iUnknown02==0)
    {
      res=iBlinkPeriodTimer->Start(iUnknown0a);
    }
    else
    {
      res=iBlinkPeriodTimer->Start(iUnknown0b);
    }
    iDrv->SwitchLight(NormalizeType(iType),NormalizeState(iUnknown06));
    iState=2;
  }
  if(iUnknown09==0&&iBlink==0)
  {
    iDrv->SwitchLight(NormalizeType(iType),NormalizeState(iUnknown05));
    iState=iUnknown05;
  }
  return res;
}

void CBackLightTimerContainer::TimerExpired(TUint aParam)
{
  if(!iDrv) User::Invariant();
  if(!iCommonTimer) User::Invariant();
  if(!iBlinkTimer) User::Invariant();
  if(!iBlinkPeriodTimer) User::Invariant();
  switch(aParam)
  {
    case 0:
      CancelBlink();
      break;
    case 1:
      if(!iUnknown02)
      {
        iBlinkPeriodTimer->Start(iUnknown0b);
        TInt value=1;
        iUnknown02=value;
        iDrv->SwitchLight(NormalizeType(iType),NormalizeState(iUnknown02));
      }
      else
      {
        iBlinkPeriodTimer->Start(iUnknown0a);
        TInt value=0;
        iUnknown02=value;
        iDrv->SwitchLight(NormalizeType(iType),NormalizeState(iUnknown02));
      }
      break;
    case 2:
      if(!iBlink)
      {
        CancelCommon();
      }
      else
      {
        iCommonTimer->Cancel();
        iUnknown09=0;
        iUnknown04=iUnknown03;
      }
      break;
    default:
      CBackLightControlImpl::Panic(1);
      break;
  }
}

TInt CBackLightTimerContainer::NormalizeState(TInt aState)
{
  switch(aState)
  {
    case 0:
      return 0;
    case 1:
      return 1;
    case 2:
      return 2;
    default:
      return KErrNotFound;
  }
}

TInt CBackLightTimerContainer::NormalizeType(TInt aType)
{
  switch(aType)
  {
    case 0:
      return 0;
    case 1:
      return 1;
    case 2:
      return 2;
    default:
      return KErrNotFound;
  }
}
