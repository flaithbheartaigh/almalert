/*
    vibractrl.cpp
    Copyright (C) 2005 zg

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "vibraimpl.hpp"
#include <hal.h>

const TInt KProfileEngUidValue=0x100058FA;
const TUid KProfileEngUid={KProfileEngUidValue};
_LIT(KKeyVibrAlert,"VibrAlert");
_LIT(KVibraPanic,"VIBRA-CTRL");
_LIT(KMutexName,"zg0x13nosunit");

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
{
  return KErrNone;
}

const TDesC8& CVibraControlImpl::Copyright(void)
{
  _LIT8(KAppCopyright,"vibractrl. (c) 2005 by zg. version 3.05");
  return KAppCopyright;
}

CVibraControl::CVibraControl(): CBase()
{
}

CVibraControl::~CVibraControl()
{
}

void Panic(CVibraControl::TVibraCtrlPanic aPanic)
{
  User::Panic(KVibraPanic,aPanic);
}

EXPORT_C void CVibraControlImpl::StartVibraL(TUint16 aDuration)
{
  if(!iTimer) User::Invariant();
  Send(ETrue);
  if(iCallback) iCallback->VibraRequestStatus(EVibraRequestOK);
  if(iTimer->Start(aDuration)!=KErrNone) Send(EFalse);
}

EXPORT_C void CVibraControlImpl::StopVibraL(void)
{
  if(!iTimer) User::Invariant();
  Send(EFalse);
  if(iCallback) iCallback->VibraRequestStatus(EVibraRequestStopped);
  if(iTimer->IsActive()) iTimer->Cancel();
}

EXPORT_C CVibraControl::TVibraModeState CVibraControlImpl::VibraSettings(void) const
{
  return iVibraState;
}

EXPORT_C void CVibraControlImpl::StartVibraL(TUint16 aDuration,TInt aIntensity)
{
}

void CVibraControlImpl::HandleGainingForeground(void)
{
  Open();
}

void CVibraControlImpl::HandleLosingForeground(void)
{
  StopVibraL(); //never leave
  Close();
}

void CVibraControlImpl::HandleNotifyL(TUid anUid,const TDesC16& aKey,const TDesC16& aValue)
{
  if(anUid==KProfileEngUid&&aKey==KKeyVibrAlert)
  {
    if(aValue[0]==0x30) iVibraState=EVibraModeOFF;
    else iVibraState=EVibraModeON;
    if(iCallback)
    {
      iCallback->VibraModeStatus(iVibraState);
    }
  }
}

void CVibraControlImpl::TimerExpired(void)
{
  StopVibraL(); //never leave
}

CVibraControlImpl::CVibraControlImpl(MVibraControlObserver* aCallback): CVibraControl(),iCallback(aCallback),iShared(this),iVibraState(EVibraModeUnknown)
{
}

void CVibraControlImpl::ConstructL(void)
{
  _LIT(KMutexName,"zg0x13nosunit");
  TInt err=iMutex.CreateGlobal(KMutexName);
  if(err==KErrAlreadyExists) err=iMutex.OpenGlobal(KMutexName);
  User::LeaveIfError(err);
  TInt machine;
  User::LeaveIfError(HAL::Get(HALData::EModel,machine));
  switch(machine)
  {
    case 0x101F466A: //3650&3660
      iObjectNumber=0x51;
      break;
    case 0x101F8C19: //n-gage
      iObjectNumber=0x58;
      break;
    case 0x101FB2B0: //n-gage qda
    case 0x101FB2B1: //n-gage qd
    case 0x101FB2B2: //n-gage qd unknown1
    case 0x101FB2B3: //n-gage qd unknown2
      iObjectNumber=0x54;
      break;
    default:
      User::Leave(KErrNotSupported);
      break;
  }
  iTimer=CVibraTimer::NewL(this);
  User::LeaveIfError(iShared.Connect(0));
  TInt vibra=0;
  err=iShared.Assign(KProfileEngUid);
  if(err==KErrNone)
  {
    err=iShared.GetInt(KKeyVibrAlert,vibra);
    if(err==KErrNone)
    {
      err=iShared.NotifyChange(KProfileEngUid,&KKeyVibrAlert);
    }
  }
  if(err==KErrNoMemory)
  {
    iShared.Close();
    User::Leave(err);
  }
  else if(err)
  {
    Panic(EPanicUnableToGetVibraSetting);
  }
  iVibraState=vibra?EVibraModeON:EVibraModeOFF;
  Open();
  CCoeEnv::Static()->AddForegroundObserverL(*this);
}

CVibraControlImpl::~CVibraControlImpl()
{
  StopVibraL(); //never leave
  iTimer->Cancel();
  delete iTimer;
  iShared.Close();
  CCoeEnv::Static()->RemoveForegroundObserver(*this);
  Close();
}

EXPORT_C CVibraControl* VibraFactory::NewL(void)
{
  CVibraControl* self=NewLC(NULL);
  CleanupStack::Pop();
  return self;
}

EXPORT_C CVibraControl* VibraFactory::NewL(MVibraControlObserver* aCallback)
{
  CVibraControl* self=NewLC(aCallback);
  CleanupStack::Pop();
  return self;
}

EXPORT_C CVibraControl* VibraFactory::NewLC(MVibraControlObserver* aCallback)
{
  CVibraControlImpl* self=new(ELeave)CVibraControlImpl(aCallback);
  CleanupStack::PushL(self);
  self->ConstructL();
  return self;
}
