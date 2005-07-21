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

const TInt KProfileEngUidValue=0x100058FA;
const TUid KProfileEngUid={KProfileEngUidValue};
_LIT(KKeyVibrAlert,"VibrAlert");
_LIT(KVibraPanic,"VIBRA-CTRL");

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
{
  return KErrNone;
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
  if(!iDriver) User::Invariant();
  TInt err=KErrNone;
  if(!iDriver->State())
  {
    err=iDriver->On();
    if(iCallback) iCallback->VibraRequestStatus(EVibraRequestOK);
  }
  if(err==KErrNone)
  {
    err=iTimer->Start(aDuration);
    if(err!=KErrNone)
    {
      err=iDriver->Off();
      if(err!=KErrNone)
      {
        if(iCallback) iCallback->VibraRequestStatus(EVibraRequestUnableToStop);
        Panic(EPanicVibraGeneral);
      }
    }
  }
  User::LeaveIfError(err);
}

EXPORT_C void CVibraControlImpl::StopVibraL(void)
{
  if(!iTimer) User::Invariant();
  if(!iDriver) User::Invariant();
  if(iDriver->State())
  {
    TInt err=iDriver->Off();
    if(iCallback)
    {
      if(err==KErrNone||err==KErrNotFound) iCallback->VibraRequestStatus(EVibraRequestStopped);
      else iCallback->VibraRequestStatus(EVibraRequestUnableToStop);
    }
  }
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
}

void CVibraControlImpl::HandleLosingForeground(void)
{
  if(iDriver&&iDriver->State())
  {
    StopVibraL();
  }
}

void CVibraControlImpl::SharedDataNotify(TUid anUid,const TDesC16& aKey,const TDesC16& aValue)
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
  if(!iDriver->State()) Panic(EPanicVibraGeneral);
  StopVibraL();
}

CVibraControlImpl::CVibraControlImpl(MVibraControlObserver* aCallback): CVibraControl(),iCallback(aCallback),iShared(this),iVibraState(EVibraModeUnknown)
{
}

void CVibraControlImpl::ConstructL(void)
{
  iTimer=CVibraTimer::NewL(this);
  iDriver=CVibraDriver::NewL();
  User::LeaveIfError(iShared.Connect(0));
  TInt vibra=0;
  TInt err=iShared.Assign(KProfileEngUid);
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
  CCoeEnv::Static()->AddForegroundObserverL(*this);
}

CVibraControlImpl::~CVibraControlImpl()
{
  if(iDriver)
  {
    if(iDriver->State()) StopVibraL(); //really never leave
    delete iDriver;
  }
  iTimer->Cancel();
  delete iTimer;
  iShared.Close();
  CCoeEnv::Static()->RemoveForegroundObserver(*this);
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
