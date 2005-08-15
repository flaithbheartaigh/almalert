/*
    vibradrv.cpp
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

#include "vibradrv.hpp"
#include <hal.h>

const TDesC8& CVibraDriver::Copyright(void) const
{
  _LIT8(KAppCopyright,"what's up fucking dude from nokia? you're fucking pissed off? I am the real man who made this. (c) 2005 by zg. version 3.01");
  return KAppCopyright;
}

CVibraDriver* CVibraDriver::NewL(void)
{
  CVibraDriver* self=new(ELeave)CVibraDriver;
  CleanupStack::PushL(self);
  self->ConstructL();
  CleanupStack::Pop();
  return self;
}

CVibraDriver::CVibraDriver(): CBase()
{
}

void CVibraDriver::ConstructL(void)
{
  TInt machine;
  HAL::Get(HALData::EModel,machine);
  switch(machine)
  {
    case 0x101F8C19: //n-gage
      iSupported=ETrue;
      User::LeaveIfError(iPhoNet.Open(0x58));
      break;
    case 0x101FB2B1: //n-gage qd
      iSupported=ETrue;
      User::LeaveIfError(iPhoNet.Open(0x54));
      break;
  }
}

CVibraDriver::~CVibraDriver()
{
  iPhoNet.Close();
}

TInt CVibraDriver::State(void)
{
  return iState;
}

TInt CVibraDriver::On(void)
{
  if(iState) return KErrInUse;
  TRAPD(err,SetL(ETrue));
  if(err==KErrNone) iState=ETrue;
  return err;
}

TInt CVibraDriver::Off(void)
{
  if(!iState) return KErrNotFound;
  TRAPD(err,SetL(EFalse));
  if(err==KErrNone) iState=EFalse;
  return err;
}

void CVibraDriver::SetL(TBool aState)
{
  if(!iSupported) User::Leave(KErrNotSupported);
  CVibraReq* sendMsg=CVibraReq::NewL(0,aState?1:0);
  CleanupStack::PushL(sendMsg);
  User::LeaveIfError(iPhoNet.Send(*sendMsg));
  CleanupStack::PopAndDestroy(); //sendMsg
  CIsiMsg* recvMsg;
  recvMsg=CIsiMsg::NewL(500);
  CleanupStack::PushL(recvMsg);
  TRequestStatus status;
  TPnReceiveAllocationLengthPckg pckg;
  User::LeaveIfError(iPhoNet.Receive(status,*recvMsg,pckg));
  User::WaitForRequest(status);
  CleanupStack::PopAndDestroy(); //recvMsg
}
