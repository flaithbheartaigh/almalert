/*
    hwserver.cpp
    Copyright (C) 2005-2006 zg

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
#include <isimsg.hpp>

const TDesC8& CHWServerImpl::Copyright(void)
{
  _LIT8(KAppCopyright,"hwtricks. (c) 2005-2006 by zg.");
  return KAppCopyright;
}

EXPORT_C CHWServer* CHWServer::NewLC(void)
{
  CHWServerImpl* self=new(ELeave)CHWServerImpl;
  CleanupStack::PushL(self);
  self->ConstructL(EFalse);
  return self;
}

CHWServer* CHWServer::NewLC(TBool anExtended)
{
  CHWServerImpl* self=new(ELeave)CHWServerImpl;
  CleanupStack::PushL(self);
  self->ConstructL(anExtended);
  return self;
}

CIsiMsg* CHWServer::SendAndReceiveLC(CIsiMsg* aSend)
{
  CleanupStack::PushL(aSend);
  CHWServer* server=CHWServer::NewLC();
  CIsiMsg* recv=CIsiMsg::NewL(500);
  CleanupStack::PushL(recv);
  server->SendL(*aSend);
  TRequestStatus status;
  TPnReceiveAllocationLengthPckg pckg;
  server->ReceiveL(status,*recv,pckg);
  User::WaitForRequest(status);
  User::LeaveIfError(status.Int());
  CleanupStack::Pop(); //recv
  CleanupStack::PopAndDestroy(2); //server,aSend
  CleanupStack::PushL(recv);
  return recv;
}

void CHWServer::SendL(CIsiMsg* aSend)
{
  SendAndReceiveLC(aSend);
  CleanupStack::PopAndDestroy(); //recv
}

CHWServerImpl::~CHWServerImpl()
{
  delete iExtender;
  iPhoNet.Close();
  if(iCaptured) iMutex.Signal();
  iMutex.Close();
}

CHWServerImpl::CHWServerImpl(): CHWServer()
{
}

void CHWServerImpl::ConstructL(TBool anExtended)
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
    case 0x101FB2B0: //n-gage qda
    case 0x101FB2B1: //n-gage qd
    case 0x101FB2B2: //n-gage qd unknown1
    case 0x101FB2B3: //n-gage qd unknown2
      User::LeaveIfError(iPhoNet.Open(0x54));
      break;
    default:
      User::Leave(KErrNotSupported);
      break;
  }
  if(anExtended) iExtender=CHWExtender::NewL(this);
}

void CHWServerImpl::SendL(CIsiMsg& aMsg)
{
  User::LeaveIfError(iPhoNet.Send(aMsg));
}

void CHWServerImpl::ReceiveL(TRequestStatus& aStatus,CIsiMsg& aMsg,TPckgBuf<TUint16>& aLen)
{
  User::LeaveIfError(iPhoNet.Receive(aStatus,aMsg,aLen));
}

void CHWServerImpl::ReceiveCancelL(void)
{
  User::LeaveIfError(iPhoNet.ReceiveCancel());
}
