/*
    hwserver.cpp
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

#include "hwtricksimpl.hpp"
#include <hal.h>

const TDesC8& CHWServerImpl::Copyright(void)
{
  _LIT8(KAppCopyright,"hwtricks. (c) 2005 by zg.");
  return KAppCopyright;
}

EXPORT_C CHWServer* CHWServer::NewLC(void)
{
  CHWServerImpl* self=new(ELeave)CHWServerImpl;
  CleanupStack::PushL(self);
  self->ConstructL();
  return self;
}

CHWServerImpl::~CHWServerImpl()
{
  iPhoNet.Close();
  if(iCaptured) iMutex.Signal();
  iMutex.Close();
}

CHWServerImpl::CHWServerImpl(): CHWServer()
{
}

void CHWServerImpl::ConstructL(void)
{
  _LIT(KMutexName,"zg0x13nosunit");
  TInt err=iMutex.CreateGlobal(KMutexName);
  if(err==KErrAlreadyExists) err=iMutex.OpenGlobal(KMutexName);
  User::LeaveIfError(err);
  iMutex.Wait();
  iCaptured=ETrue;
  TInt machine;
  User::LeaveIfError(HAL::Get(HALData::EModel,machine));
  switch(machine)
  {
    case 0x101F466A: //3650&3660
      User::LeaveIfError(iPhoNet.Open(0x51));
      break;
    case 0x101F8C19: //n-gage
      User::LeaveIfError(iPhoNet.Open(0x58));
      break;
    case 0x101FB2B1: //n-gage qd
      User::LeaveIfError(iPhoNet.Open(0x54));
      break;
    default:
      User::Leave(KErrNotSupported);
      break;
  }
}

void CHWServerImpl::SendL(CPnMsg& aMsg)
{
  User::LeaveIfError(iPhoNet.Send(aMsg));
}

void CHWServerImpl::ReceiveL(TRequestStatus& aStatus,CPnMsg& aMsg,TPnReceiveAllocationLengthPckg& aLen)
{
  User::LeaveIfError(iPhoNet.Receive(aStatus,aMsg,aLen));
}

void CHWServerImpl::ReceiveCancelL(void)
{
  User::LeaveIfError(iPhoNet.ReceiveCancel());
}
