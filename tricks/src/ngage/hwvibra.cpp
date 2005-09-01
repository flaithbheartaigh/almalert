/*
    hwvibra.cpp
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

#include "hwtricks.hpp"
#include <zg_vibra.hpp>

EXPORT_C void HWVibra::SwitchL(TBool aState)
{
  CHWServer* server=CHWServer::NewLC();
  CVibraReq* sendMsg=CVibraReq::NewL(0,aState?1:0);
  CleanupStack::PushL(sendMsg);
  server->SendL(*sendMsg);
  CIsiMsg* recvMsg=CIsiMsg::NewL(500);
  CleanupStack::PushL(recvMsg);
  TRequestStatus status;
  TPnReceiveAllocationLengthPckg pckg;
  server->ReceiveL(status,*recvMsg,pckg);
  User::WaitForRequest(status);
  CleanupStack::PopAndDestroy(3); //sendMsg,recvMsg,server
}

EXPORT_C void HWVibra::Reserved_1(void)
{
  User::Leave(KErrNotSupported);
}

EXPORT_C void HWVibra::SetIntensityL(TUint8 anIntensity)
{
  if(anIntensity>KMaxIntensity) User::Leave(KErrTooBig);
  CHWServer* server=CHWServer::NewLC();
  CVibraIntensityReq* sendMsg=CVibraIntensityReq::NewL(0,anIntensity);
  CleanupStack::PushL(sendMsg);
  server->SendL(*sendMsg);
  CIsiMsg* recvMsg=CIsiMsg::NewL(500);
  CleanupStack::PushL(recvMsg);
  TRequestStatus status;
  TPnReceiveAllocationLengthPckg pckg;
  server->ReceiveL(status,*recvMsg,pckg);
  User::WaitForRequest(status);
  CleanupStack::PopAndDestroy(3); //sendMsg,recvMsg,server
}

EXPORT_C void HWVibra::Reserved_3(void)
{
  User::Leave(KErrNotSupported);
}
