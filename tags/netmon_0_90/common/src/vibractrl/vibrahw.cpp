/*
    vibrahw.cpp
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

#include "vibraimpl.hpp"
#include <zg_vibra.hpp>

void CVibraControlImpl::Open(void)
{
  if(!iCaptured)
  {
    iMutex.Wait();
    if(iPhoNet.Open(iObjectNumber)==KErrNone) iCaptured=ETrue;
    else iMutex.Signal();
  }
}

void CVibraControlImpl::Close(void)
{
  if(iCaptured)
  {
    iCaptured=EFalse;
    iPhoNet.Close();
    iMutex.Signal();
  }
}

void CVibraControlImpl::SendL(TBool aState)
{
  CVibraReq* send=CVibraReq::NewL(0,aState?1:0);
  CleanupStack::PushL(send);
  User::LeaveIfError(iPhoNet.Send(*send));
  CIsiMsg* recv=CIsiMsg::NewL(500);
  CleanupStack::PushL(recv);
  TRequestStatus status;
  TPnReceiveAllocationLengthPckg pckg;
  User::LeaveIfError(iPhoNet.Receive(status,*recv,pckg));
  User::WaitForRequest(status);
  CleanupStack::PopAndDestroy(2); //recv,send
}

void CVibraControlImpl::Send(TBool aState)
{
  if(iCaptured)
  {
    TRAPD(err,SendL(aState));
  }
}
