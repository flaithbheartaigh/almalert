/*
    ekern_power.cpp
    Copyright (C) 2006 zg

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

#include <k32std61.hpp>
#include <m32std61.hpp>

TInt PowerEmergency(TAny* aPtr) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TBool Kern::PowerGood(void) //FIXME: NOT IMPLEMENTED
{
  return ETrue;
}

TInt Kern::SwitchOnCount(void) //FIXME: NOT IMPLEMENTED
{
  return 0;
}

void Kern::NewBattery(void) //FIXME: NOT IMPLEMENTED
{
}

TInt K::TotalCurrentConsumptionInMilliAmps(void) //FIXME: NOT IMPLEMENTED
{
  return 0;
}

void K::PowerTick(void) //FIXME: NOT IMPLEMENTED
{
}

void K::PowerSecondTick(void) //FIXME: NOT IMPLEMENTED
{
}

void Power::SwitchOff(void) //FIXME: NOT IMPLEMENTED
{
}

TInt Power::PowerOn(void) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TBool DPowerHandler::PowerGood(void)
{
  return (iState==EPoweredDown)?EFalse:Kern::PowerGood();
}

EXPORT_C TInt DPowerHandler::PowerOn(void)
{
  if(iState==EPoweredUp) return KErrNone;
  SetCurrentConsumptionInMilliAmps(0);
  iState=EDoingPowerUp;
  DoPowerOn();
  TInt level=ImpHal::DisableIrqsToLevel2();
  iState=Kern::PowerGood()?EPoweredUp:EPoweredDown;
  ImpHal::RestoreIrqs(level);
  return KErrNone;
}

EXPORT_C void DPowerHandler::PowerStandby(void)
{
  if(iState==EPoweredDown) return;
  iState=EDoingPowerDown;
  DoPowerStandby();
}

EXPORT_C void DPowerHandler::PowerEmergencyStandby(void)
{
  if(iState!=EPoweredDown)
  {
    iState=EDoingPowerDown;
    DoPowerEmergencyStandby();
    SetCurrentConsumptionInMilliAmps(0);
  }
  iState=EPoweredDown;
}

EXPORT_C TInt DPowerHandler::SetRequirement(TUint32 aState) //FIXME: NOT IMPLEMENTED
{
  return 0;
}

EXPORT_C void DPowerHandler::SetCurrentConsumptionInMilliAmps(TInt aValue) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C DPowerModel::DPowerModel(): iPowerHandlers(_FOFF(DPowerHandler,iLink))
{
}

EXPORT_C TInt DPowerModel::PowerOn(void) //FIXME: NOT IMPLEMENTED
{
  return 0;
}

EXPORT_C TInt DPowerModel::PowerStandby(void) //FIXME: NOT IMPLEMENTED
{
  return 0;
}

EXPORT_C void DPowerModel::PowerEmergencyStandby(void) //FIXME: NOT IMPLEMENTED
{
}

void DPowerModel::PowerEmergencyStandbyDfc(void) //FIXME: NOT IMPLEMENTED
{
}

TInt DPowerModel::PowerTransition(TUint32 aState) //FIXME: NOT IMPLEMENTED
{
  return 0;
}

TInt DPowerModel::AddPowerHandler(DPowerHandler &aHandler)
{
  iPowerHandlers.AddLast(aHandler);
  return KErrNone;
}

void DPowerModel::RemovePowerHandler(DPowerHandler &aHandler)
{
  if(aHandler.iRequirement) KK::Panic(KK::EPanicPowerRequired);
  aHandler.iLink.Deque();
}

TUint32 DPowerModel::GetPowerRequirement(void) //FIXME: NOT IMPLEMENTED
{
  return 0;
}

EXPORT_C void DPowerModel::NotifyEvent(const TRawEvent& aEvent)
{
}

EXPORT_C void Power::SetPowerModel(const DPowerModel* aModel)
{
  if(K::PowerModel) KK::Panic(KK::EPanicPowerModelExists);
  K::PowerModel=const_cast<DPowerModel*>(aModel);
}

EXPORT_C TInt Power::AddPowerHandler(DPowerHandler& aHandler)
{
  return K::PowerModel->AddPowerHandler(aHandler);
}

EXPORT_C void Power::RemovePowerHandler(DPowerHandler& aHandler)
{
  K::PowerModel->RemovePowerHandler(aHandler);
}

EXPORT_C void Power::LockPowerState(void)
{
  K::PowerModel->iLock++;
}

EXPORT_C void Power::UnlockPowerState(void)
{
  K::PowerModel->iLock--;
}
