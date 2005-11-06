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
  _LIT8(KAppCopyright,"backlightctrl. (c) 2005 by zg. version 1.01");
  return KAppCopyright;
}

CBackLightControlImpl::CBackLightControlImpl(MBackLightControlObserver* aCallback): CBackLightControl(),iCallback(aCallback)
{
}

CBackLightControlImpl::~CBackLightControlImpl()
{
  iCallback=NULL;
  TRAPD(err,HWBacklight::SetGameModeL(EFalse));
  TRAP(err,HWBacklight::SetBrightnessL(HWBacklight::EBrightnessScreen,iBrightnessOriginalState));
  BackLightOn(EBackLightTypeBoth,0);
  CCoeEnv::Static()->RemoveForegroundObserver(*this);
  delete iScreen;
  delete iKeys;
  delete iBrightness;
  delete iScreenBlinker;
  delete iKeysBlinker;
}

void CBackLightControlImpl::ConstructL(void)
{
  TUint8 dummy;
  HWBacklight::BrightnessL(HWBacklight::EBrightnessScreen,iBrightnessOriginalState,dummy);
  iBrightnessState=iBrightnessCurrentState=iBrightnessOriginalState;
  HWBacklight::SetGameModeL(ETrue);
  iScreen=CBackLightTimer::NewL(this,EScreen);
  iKeys=CBackLightTimer::NewL(this,EKeys);
  iBrightness=CBackLightTimer::NewL(this,EBrightness);
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

TInt CBackLightControlImpl::SwitchBrightness(void)
{
  TRAPD(err,HWBacklight::SetBrightnessL(HWBacklight::EBrightnessScreen,iBrightnessCurrentState));
  if(err==KErrNone) Switch();
  if(iCallback) iCallback->BrightnessNotify(NormalizeBrightness(iBrightnessCurrentState));
  return err;
}

void CBackLightControlImpl::UpdateBrightness(TUint8 aBrightness,TUint16 aDuration)
{
  iBrightnessCurrentState=aBrightness;
  if(!aDuration) iBrightnessState=iBrightnessCurrentState;
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
    case EBrightness:
      iBrightnessCurrentState=iBrightnessState;
      SwitchBrightness();
      break;
  }
  Switch();
}

EXPORT_C TInt CBackLightControlImpl::BackLightOn(TInt aType,TUint16 aDuration)
{
  SBlink blink={0,0};
  UpdateState(aType,EBackLightStateOn,aDuration,blink);
  TInt err=Switch();
  if(err==KErrNone) err=Start(aType,aDuration);
  return err;
}

EXPORT_C TInt CBackLightControlImpl::BackLightBlink(TInt aType,TUint16 aDuration,TUint16 aOnTime,TUint16 aOffTime)
{
  if(aOnTime==0||aOffTime==0) return KErrArgument;
  SBlink blink={aOnTime,aOffTime};
  UpdateState(aType,EBackLightStateBlink,aDuration,blink);
  TInt err=Switch();
  if(err==KErrNone) err=Start(aType,aDuration);
  return err;
}

EXPORT_C TInt CBackLightControlImpl::BackLightOff(TInt aType)
{
  SBlink blink={0,0};
  UpdateState(aType,EBackLightStateOff,0,blink);
  TInt err=Switch();
  if(err==KErrNone) err=Start(aType,0);
  return err;
}

EXPORT_C TInt CBackLightControlImpl::BackLightChange(TInt aType,TUint16 aDuration)
{
  SBlink blink={0,0};
  UpdateState(aType,EBackLightStateBlink,aDuration,blink);
  TInt err=Switch();
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

TInt CBackLightControlImpl::NormalizeBrightness(TUint8 aBrightness)
{
  const TUint8 KBrightnessValues[]={0,13,14,18,24,30,36,42,48,54,60,66,72,78,84,90};
  TInt res=16;
  for(TInt i=15;i>=0;i--,res--)
  {
    if(aBrightness>=KBrightnessValues[i]) break;
  }
  return res;
}

TInt CBackLightControlImpl::SetScreenBrightnessInternal(TUint8 aBrightness,TUint16 aDuration)
{
  UpdateBrightness(aBrightness,aDuration);
  TInt err=SwitchBrightness();
  if(err==KErrNone) err=iBrightness->Start(aDuration);
  return err;
}

EXPORT_C TInt CBackLightControlImpl::SetScreenBrightness(TInt aState,TUint16 aDuration)
{
  TUint8 brightness=56;
  if(!aState)
  {
    brightness=iBrightnessState;
    aDuration=0;
  }
  else if(aState>0&&aState<=16)
  {
    const TUint8 KBrightnessValues[]={0,13,17,23,29,35,41,47,53,59,65,71,77,83,89,100};
    brightness=KBrightnessValues[aState-1];
  }
  return SetScreenBrightnessInternal(brightness,aDuration);
}

EXPORT_C TInt CBackLightControlImpl::ScreenBrightness(void)
{
  return NormalizeBrightness(iBrightnessCurrentState);
}

void CBackLightControlImpl::HandleGainingForeground(void)
{
  TRAPD(err,HWBacklight::SetGameModeL(ETrue));
  BackLightOn(EBackLightTypeBoth,0);
}

void CBackLightControlImpl::HandleLosingForeground(void)
{
  SetScreenBrightnessInternal(iBrightnessOriginalState,0);
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
