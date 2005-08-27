/*
    lightdrv.cpp
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

#include "lightdrvimpl.hpp"
#include <SharedData.hpp>

const TUint16 KLightPanic[]={L'B',L'A',L'C',L'K',L'L',L'I',L'G',L'H',L'T',L'-',L'C',L'T',L'R',L'L',L'-',L'D',L'R',L'V',0};

EXPORT_C CLightDrv::~CLightDrv() //ok
{
}

EXPORT_C CLightDrv* CLightDrv::NewL(TBool aGameModeType)
{
  CLightDrvImpl* self=new(ELeave)CLightDrvImpl;
  CleanupStack::PushL(self);
  self->ConstructL(aGameModeType);
  CleanupStack::Pop(self);
  return self;
}

EXPORT_C void CLightDrv::SwitchLight(TInt aType,TInt aState)
{
  DoSwitchLight(aType,aState);
}

EXPORT_C void CLightDrv::SetGameMode(TInt aGameMode)
{
  DoSetGameMode(aGameMode);
}

CLightDrvImpl::CLightDrvImpl(): iScreenState(0),iKeysState(0) //ok
{
}

CLightDrvImpl::~CLightDrvImpl() //ok
{
  iLights.Close();
  iServer.Close();
  if(iGameModeType)
  {
    iSysAp.SetInt(KKeyGameMode,0);
    iSysAp.Close();
  }
  else if(iTimer)
  {
    iTimer->Cancel();
    delete iTimer;
    iTimer=NULL;
  }
}


void CLightDrvImpl::ConstructL(TBool aGameModeType) //ok
{
  iGameModeType=aGameModeType;
  User::LeaveIfError(iServer.Connect());
  User::LeaveIfError(iLights.Open(iServer));
  iLights.SwitchLights(RDosLights::EKeyAsScreen,RDosLights::EScreenSlowOn);
  if(iGameModeType)
  {
    User::LeaveIfError(iSysAp.Connect(0));
    TInt err=iSysAp.AssignToTemporaryFile(KSysAppUid);
    if(err==KErrNone) err=iSysAp.SetInt(KKeyGameMode,1);
    switch(err)
    {
      case KErrNone:
        break;
      case KErrNoMemory:
        iSysAp.Close();
        User::Leave(err);
        break;
      default:
        TPtrC ptr(KLightPanic);
        User::Panic(ptr,KErrGeneral);
        break;
    }
  }
  else
  {
    iTimer=CPeriodic::NewL(450);
    iPeriod=14900000;
  }
}

void CLightDrvImpl::DoSwitchLight(TInt aType,TInt aState) //ok
{
  switch(aType)
  {
    case EScreen:
      iScreenState=aState;
      if(iKeysState==EOn)
      {
        if(iScreenState==EOn)
        {
          iLights.SwitchLights(RDosLights::EKeyAsScreen,RDosLights::EScreenSlowOn); //s:on,k:on
        }
        if(iScreenState==EOff)
        {
          iLights.SwitchLights(RDosLights::EKeyExtra,RDosLights::EScreenSlowOn); //s:off,k:on
        }
      }
      else
      {
        if(iScreenState==EOn)
        {
          iLights.SwitchLights(RDosLights::EKeyOff,RDosLights::EScreenSlowOn); //s:on,k:off
        }
        if(iScreenState==EOff)
        {
          iLights.SwitchLights(RDosLights::EKeyAsScreen,RDosLights::EScreenSlowOff); //s:off,k:off
        }
      }
      break;
    case EKeys:
      iKeysState=aState;
      if(iScreenState==EOn)
      {
        if(iKeysState==EOn)
        {
          iLights.SwitchLights(RDosLights::EKeyAsScreen,RDosLights::EScreenSlowOn); //s:on,k:on
        }
        if(iKeysState==EOff)
        {
          iLights.SwitchLights(RDosLights::EKeyOff,RDosLights::EScreenSlowOn); //s:on,k:off
        }
      }
      else
      {
        if(iKeysState==EOn)
        {
          iLights.SwitchLights(RDosLights::EKeyExtra,RDosLights::EScreenSlowOn); //s:off,k:on
        }
        if(iKeysState==EOff)
        {
          iLights.SwitchLights(RDosLights::EKeyAsScreen,RDosLights::EScreenSlowOff); //s:off,k:off
        }
      }
      break;
    default:
      TPtrC ptr(KLightPanic);
      User::Panic(ptr,KErrNotFound);
      break;
  }
}

void CLightDrvImpl::DoSetGameMode(TInt aGameMode)
{
  if(iGameModeType)
  {
    if(aGameMode)
    {
      iSysAp.SetInt(KKeyGameMode,1);
    }
    else
    {
      iSysAp.SetInt(KKeyGameMode,0);
    }
  }
  else
  {
    if(aGameMode) StartTimer();
    else StopTimer();
  }
}

void CLightDrvImpl::StartTimer(void)
{
  if(!iTimer) User::Invariant();
  StopTimer();
  if(!iTimer) iTimer=CPeriodic::NewL(450);
  TInt diff=-99985-User::InactivityTime().Int()*1000000;
  if(diff>0)
  {
    iTimer->Start(diff,iPeriod,TCallBack(Timeout,this));
  }
  else
  {
    iTimer->Start(iPeriod,iPeriod,TCallBack(Timeout,this));
  }
  DoTimeout();
}

void CLightDrvImpl::StopTimer(void)
{
  if(iTimer) iTimer->Cancel();
}

TInt CLightDrvImpl::Timeout(TAny* aLightDrv)
{
  ((CLightDrvImpl*)aLightDrv)->DoTimeout();
  return ETrue;
}

void CLightDrvImpl::DoTimeout(void)
{
  User::ResetInactivityTime();
  if(iScreenState==EOn) DoSwitchLight(EScreen,EOn);
  if(iKeysState==EOn) DoSwitchLight(EKeys,EOn);
}

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
{
  return KErrNone;
}
