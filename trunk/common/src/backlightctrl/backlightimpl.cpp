/*
    backlightimpl.cpp
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

#include <hwtricks.hpp>
#include "backlightimpl.hpp"
#include "backlighttimer.hpp"

const TDesC8& CBackLightControlImpl::Copyright(void)
{
  _LIT8(KAppCopyright,"backlightctrl. (c) 2005 by zg. version 1.00");
  return KAppCopyright;
}

CBackLightControlImpl::CBackLightControlImpl(MBackLightControlObserver* aCallback): CBackLightControl(),iCallback(aCallback)
{
}

CBackLightControlImpl::~CBackLightControlImpl()
{
  iCallback=NULL;
  TRAPD(err,HWBacklight::SetGameModeL(EFalse));
  BackLightOn(EBackLightTypeBoth,0);
  CCoeEnv::Static()->RemoveForegroundObserver(*this);
  delete iScreen;
  delete iKeys;
  delete iScreenBlinker;
  delete iKeysBlinker;
}

void CBackLightControlImpl::ConstructL(void)
{
  HWBacklight::SetGameModeL(ETrue);
  iScreen=CBackLightTimer::NewL(this,EScreen);
  iKeys=CBackLightTimer::NewL(this,EKeys);
  iScreenBlinker=CBackLightTimer::NewL(this,EScreenBlink);
  iKeysBlinker=CBackLightTimer::NewL(this,EKeysBlink);
  CCoeEnv::Static()->AddForegroundObserverL(*this);
  BackLightOn(EBackLightTypeBoth,0);
}

TInt CBackLightControlImpl::Switch(void)
{
  TInt type=HWBacklight::ESwitchBoth;
  TInt state=HWBacklight::EOn;
  if(iScreenCurrentState==iKeysCurrentState)
  {
    if(iScreenCurrentState==EBackLightStateOff)
    {
      type=HWBacklight::ESwitchKeys;
      state=HWBacklight::ESmoothOff;
    }
  }
  else
  {
    if(iScreenCurrentState==EBackLightStateOn)
    {
      type=HWBacklight::ESwitchScreen;
    }
    else
    {
      type=HWBacklight::ESwitchKeys;
    }
  }
  TRAPD(err,HWBacklight::SwitchL(type,state));
  if(iScreenCurrentBlink)
  {
    if(!iScreenBlinker->IsActive()) iScreenBlinker->Start((iScreenCurrentState==EBackLightStateOn)?iScreenCurrentTime.iOn:iScreenCurrentTime.iOff);
  }
  else
  {
    iScreenBlinker->Cancel();
  }
  if(iKeysCurrentBlink)
  {
    if(!iKeysBlinker->IsActive()) iKeysBlinker->Start((iKeysCurrentState==EBackLightStateOn)?iKeysCurrentTime.iOn:iKeysCurrentTime.iOff);
  }
  else
  {
    iKeysBlinker->Cancel();
  }
  if(iCallback)
  {
    iCallback->ScreenNotify(iScreenCurrentBlink?EBackLightStateBlink:iScreenCurrentState);
    iCallback->KeysNotify(iKeysCurrentBlink?EBackLightStateBlink:iKeysCurrentState);
  }
  return err;
}

void CBackLightControlImpl::UpdateState(TInt aType,TInt aState,TUint16 aDuration,SBlink aBlink)
{
  if(aType==EBackLightTypeScreen||aType==EBackLightTypeBoth)
  {
    iScreenCurrentState=(aState==EBackLightStateBlink)?(!iScreenCurrentState):aState;
    iScreenCurrentBlink=(aState==EBackLightStateBlink&&aBlink.iOn);
    iScreenCurrentTime=aBlink;
    if(!aDuration)
    {
      iScreenState=iScreenCurrentState;
      iScreenBlink=iScreenCurrentBlink;
      iScreenTime=iScreenCurrentTime;
    }
  }
  if(aType==EBackLightTypeKeys||aType==EBackLightTypeBoth)
  {
    iKeysCurrentState=(aState==EBackLightStateBlink)?(!iKeysCurrentState):aState;
    iKeysCurrentBlink=(aState==EBackLightStateBlink&&aBlink.iOn);
    iKeysCurrentTime=aBlink;
    if(!aDuration)
    {
      iKeysState=iKeysCurrentState;
      iKeysBlink=iKeysCurrentBlink;
      iKeysTime=iKeysCurrentTime;
    }
  }
}

TInt CBackLightControlImpl::Start(TInt aType,TUint16 aDuration)
{
  TInt err;
  switch(aType)
  {
    case EBackLightTypeScreen:
      err=iScreen->Start(aDuration);
      break;
    case EBackLightTypeKeys:
      err=iKeys->Start(aDuration);
      break;
    case EBackLightTypeBoth:
      err=iScreen->Start(aDuration);
      if(err==KErrNone) err=iKeys->Start(aDuration);
      break;
    default:
      err=KErrNotSupported;
      break;
  }
  return err;
}

void CBackLightControlImpl::TimerExpired(TUint aParam)
{
  switch(aParam)
  {
    case EScreen:
      iScreenBlinker->Cancel();
      iScreenCurrentState=iScreenState;
      iScreenCurrentBlink=iScreenBlink;
      iScreenCurrentTime=iScreenTime;
      break;
    case EKeys:
      iKeysBlinker->Cancel();
      iKeysCurrentState=iKeysState;
      iKeysCurrentBlink=iKeysBlink;
      iKeysCurrentTime=iKeysTime;
      break;
    case EScreenBlink:
      iScreenCurrentState=(iScreenCurrentState==EBackLightStateOn)?EBackLightStateOff:EBackLightStateOn;
      break;
    case EKeysBlink:
      iKeysCurrentState=(iKeysCurrentState==EBackLightStateOn)?EBackLightStateOff:EBackLightStateOn;
      break;
  }
  Switch();
}

EXPORT_C TInt CBackLightControlImpl::BackLightOn(TInt aType,TUint16 aDuration)
{
  TInt err;
  SBlink blink={0,0};
  UpdateState(aType,EBackLightStateOn,aDuration,blink);
  err=Switch();
  if(err==KErrNone) err=Start(aType,aDuration);
  return err;
}

EXPORT_C TInt CBackLightControlImpl::BackLightBlink(TInt aType,TUint16 aDuration,TUint16 aOnTime,TUint16 aOffTime)
{
  TInt err;
  if(aOnTime==0||aOffTime==0) return KErrArgument;
  SBlink blink={aOnTime,aOffTime};
  UpdateState(aType,EBackLightStateBlink,aDuration,blink);
  err=Switch();
  if(err==KErrNone) err=Start(aType,aDuration);
  return err;
}

EXPORT_C TInt CBackLightControlImpl::BackLightOff(TInt aType)
{
  TInt err;
  SBlink blink={0,0};
  UpdateState(aType,EBackLightStateOff,0,blink);
  err=Switch();
  if(err==KErrNone) err=Start(aType,0);
  return err;
}

EXPORT_C TInt CBackLightControlImpl::BackLightChange(TInt aType,TUint16 aDuration)
{
  TInt err;
  SBlink blink={0,0};
  UpdateState(aType,EBackLightStateBlink,aDuration,blink);
  err=Switch();
  if(err==KErrNone) err=Start(aType,aDuration);
  return err;
}

EXPORT_C TInt CBackLightControlImpl::BackLightState(TInt aType)
{
  TInt res;
  switch(aType)
  {
    case EBackLightTypeScreen:
      res=iScreenCurrentState;
      break;
    case EBackLightTypeKeys:
      res=iKeysCurrentState;
      break;
    default:
      res=EBackLightStateUnknown;
      break;
  }
  return res;
}

void CBackLightControlImpl::HandleGainingForeground(void)
{
  TRAPD(err,HWBacklight::SetGameModeL(ETrue));
  BackLightOn(EBackLightTypeBoth,0);
}

void CBackLightControlImpl::HandleLosingForeground(void)
{
  BackLightOn(EBackLightTypeBoth,0);
  TRAPD(err,HWBacklight::SetGameModeL(EFalse));
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
