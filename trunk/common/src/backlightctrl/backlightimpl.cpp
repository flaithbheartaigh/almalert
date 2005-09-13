#include "backlightimpl.hpp"
#include <hal.h>
#include "backlighttimer.hpp"

CBackLightControlImpl::CBackLightControlImpl(MBackLightControlObserver* aCallback): CBackLightControl(),iCallback(aCallback)
{
}

CBackLightControlImpl::~CBackLightControlImpl()
{
  iCallback=NULL;
  iDrv->SetGameMode(EFalse);
  BackLightOn(2,0);
  CCoeEnv::Static()->RemoveForegroundObserver(*this);
  delete iTimerKeys;
  delete iTimerScreen;
  delete iDrv;
  iDrv=NULL;
  iTimerKeys=NULL;
  iTimerScreen=NULL;
}

void CBackLightControlImpl::ConstructL(void)
{
  iExtended=IsOriginalNGage()?EFalse:ETrue;
  iDrv=CLightDrv::NewL(iExtended);
  iDrv->SetGameMode(ETrue);
  iTimerKeys=CBackLightTimerContainer::NewL(this,1,iDrv);
  iTimerScreen=CBackLightTimerContainer::NewL(this,0,iDrv);
  CCoeEnv::Static()->AddForegroundObserverL(*this);
  BackLightOn(2,0);
}

EXPORT_C TInt CBackLightControlImpl::BackLightOn(TInt aType,TUint16 aDuration)
{
  TInt res;
  if(!iTimerScreen) User::Invariant();
  if(!iTimerKeys) User::Invariant();
  switch(NormalizeType(aType))
  {
    case 0:
      res=iTimerScreen->BackLightSwitch(0,aDuration);
      break;
    case 1:
      res=iTimerKeys->BackLightSwitch(0,aDuration);
      break;
    case 2:
      iTimerScreen->BackLightSwitch(0,aDuration);
      res=iTimerKeys->BackLightSwitch(0,aDuration);
      if(res==KErrNone) res=iTimerScreen->BackLightSwitch(0,aDuration);
      break;
    default:
      res=KErrNotSupported;
      break;
  }
  return res;
}

EXPORT_C TInt CBackLightControlImpl::BackLightBlink(TInt aType,TUint16 aDuration,TUint16 aOnTime,TUint16 aOffTime)
{
  TInt res;
  if(!iTimerScreen) User::Invariant();
  if(!iTimerKeys) User::Invariant();
  switch(NormalizeType(aType))
  {
    case 0:
      res=iTimerScreen->StartBlink(aDuration,aOnTime,aOffTime);
      break;
    case 1:
      res=iTimerKeys->StartBlink(aDuration,aOnTime,aOffTime);
      break;
    case 2:
      res=iTimerKeys->StartBlink(aDuration,aOnTime,aOffTime);
      if(res==KErrNone) res=iTimerScreen->StartBlink(aDuration,aOnTime,aOffTime);
      break;
    default:
      res=KErrNotSupported;
      break;
  }
  return res;
}

EXPORT_C TInt CBackLightControlImpl::BackLightOff(TInt aType)
{
  TInt res;
  if(!iTimerScreen) User::Invariant();
  if(!iTimerKeys) User::Invariant();
  switch(NormalizeType(aType))
  {
    case 0:
      res=iTimerScreen->BackLightSwitch(1,0);
      break;
    case 1:
      res=iTimerKeys->BackLightSwitch(1,0);
      break;
    case 2:
      res=iTimerScreen->BackLightSwitch(1,0);
      if(res==KErrNone) res=iTimerKeys->BackLightSwitch(1,0);
      break;
    default:
      res=KErrNotSupported;
      break;
  }
  return res;
}

EXPORT_C TInt CBackLightControlImpl::BackLightChange(TInt aType,TUint16 aDuration)
{
  TInt res;
  if(!iTimerScreen) User::Invariant();
  if(!iTimerKeys) User::Invariant();
  switch(NormalizeType(aType))
  {
    case 0:
      res=iTimerScreen->BackLightChange(aDuration);
      break;
    case 1:
      res=iTimerKeys->BackLightChange(aDuration);
      break;
    case 2:
      res=iTimerScreen->BackLightChange(aDuration);
      if(res==KErrNone) res=iTimerKeys->BackLightChange(aDuration);
      break;
    default:
      res=KErrNotSupported;
      break;
  }
  return res;
}

EXPORT_C TInt CBackLightControlImpl::BackLightState(TInt aType)
{
  TInt res;
  if(!iTimerScreen) User::Invariant();
  if(!iTimerKeys) User::Invariant();
  switch(aType)
  {
    case 0:
      res=iTimerScreen->BackLightState();
      break;
    case 1:
      res=iTimerKeys->BackLightState();
      break;
    default:
      res=3;
      break;
  }
  return res;
}

TInt CBackLightControlImpl::NormalizeType(TInt aType)
{
  TInt res=2;
  switch(aType)
  {
    case 0:
      if(!iExtended) res=2;
      else res=0;
      break;
    case 1:
      if(!iExtended) res=3;
      else res=1;
      break;
    case 2:
      res=2;
      break;
    default:
      Panic(0);
      break;
  }
  return res;
}

void CBackLightControlImpl::Panic(TInt aPanic)
{
  User::Panic(_L("BACKLIGHT-CTRL"),aPanic);
}

TBool CBackLightControlImpl::IsOriginalNGage(void)
{
  TInt model=0;
  if(HAL::Get(HALData::EModel,model)!=KErrNone||model==0x101F8C19) return ETrue;
  return EFalse;
}

void CBackLightControlImpl::BackLightNotify(TInt aType,TInt aState)
{
  if(iCallback)
  {
    if(aType==0)
    {
      iCallback->ScreenNotify(aState);
    }
    else
    {
      iCallback->KeysNotify(aState);
    }
  }
}

void CBackLightControlImpl::HandleGainingForeground(void)
{
  if(!iTimerScreen) User::Invariant();
  if(!iTimerKeys) User::Invariant();
  if(!iDrv) User::Invariant();
  if(iDrv->SetGameMode(ETrue)!=KErrNone||iTimerScreen->ResumeBackLight()!=KErrNone||iTimerKeys->ResumeBackLight()!=KErrNone) Panic(2);
}

void CBackLightControlImpl::HandleLosingForeground(void)
{
  if(!iTimerScreen) User::Invariant();
  if(!iTimerKeys) User::Invariant();
  if(!iDrv) User::Invariant();
  iDrv->SetGameMode(EFalse);
  iTimerScreen->PauseBackLight();
  iTimerKeys->PauseBackLight();
}

EXPORT_C CBackLightControl* CBackLightControl::NewL(void)
{
  CBackLightControl* self=NewLC(NULL);
  CleanupStack::Pop(); //self
  return self;
}

EXPORT_C CBackLightControl* CBackLightControl::NewL(MBackLightControlObserver* aCallback)
{
  CBackLightControl* self=NewLC(aCallback);
  CleanupStack::Pop(); //self
  return self;
}

EXPORT_C CBackLightControl* CBackLightControl::NewLC(MBackLightControlObserver* aCallback)
{
  CBackLightControlImpl* self=new(ELeave)CBackLightControlImpl(aCallback);
  CleanupStack::PushL(self);
  self->ConstructL();
  return self;
}
