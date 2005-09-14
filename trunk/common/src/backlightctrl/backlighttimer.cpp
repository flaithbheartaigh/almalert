#include "backlighttimer.hpp"

CBackLightTimer* CBackLightTimer::NewL(MTimeOutNotify* aTimeOutNotify,TUint aParam)
{
  CBackLightTimer* self=new(ELeave)CBackLightTimer(aTimeOutNotify,aParam);
  CleanupStack::PushL(self);
  self->ConstructL();
  CleanupStack::Pop();
  return self;
}

CBackLightTimer::CBackLightTimer(MTimeOutNotify* aTimeOutNotify,TUint aParam): CTimer(EPriorityHigh),iNotify(aTimeOutNotify),iParam(aParam)
{
  if(!aTimeOutNotify) User::Invariant();
}

CBackLightTimer::~CBackLightTimer()
{
  iNotify=NULL;
}

TInt CBackLightTimer::Start(TInt anInterval)
{
  if(!CActiveScheduler::Current()) User::Invariant();
  if(!IsAdded()) CActiveScheduler::Add(this);
  if(IsActive()) Cancel();
  if(anInterval) After(anInterval*1000);
  return KErrNone;
}

void CBackLightTimer::RunL(void)
{
  if(!iNotify) User::Invariant();
  iNotify->TimerExpired(iParam);
}
