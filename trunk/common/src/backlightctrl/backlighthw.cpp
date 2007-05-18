/*
    backlighthw.cpp
    Copyright (C) 2007 zg

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

#include "backlightimpl.hpp"
#include <CommonEngine.hpp>
#include <SharedData.hpp>
#include <isimsg.hpp>
#include <isi_light.hpp>
#include <isi_units.hpp>

void CBackLightControlImpl::Open(void)
{
  if(!iCaptured)
  {
    iMutex.Wait();
    if(iPhoNet.Open(iObjectNumber)==KErrNone) iCaptured=ETrue;
    else iMutex.Signal();
  }
}

void CBackLightControlImpl::Close(void)
{
  if(iCaptured)
  {
    iCaptured=EFalse;
    iPhoNet.Close();
    iMutex.Signal();
  }
}

void CBackLightControlImpl::SetGameModeL(TBool aState)
{
  RSharedDataClient sysap;
  User::LeaveIfError(sysap.Connect(0));
  CleanupClosePushL(sysap);
  User::LeaveIfError(sysap.AssignToTemporaryFile(KSysAppUid));
  sysap.SetInt(KKeyLight,aState?1:0);
  if(aState)
  {
    User::After(100000);
    SwitchL(ESwitchBoth,EOn);
  }
  CleanupStack::PopAndDestroy(); //sysap
}

void CBackLightControlImpl::SwitchL(TInt aType,TInt aState)
{
  CLightSwitchReq* send=CLightSwitchReq::NewL(0,aType,aState,(CSubBlockArray*)NULL);
  SendL(send);
}

void CBackLightControlImpl::SetBrightnessL(TUint8 aValue)
{
  TBuf8<4> data;
  data.Append(3);
  data.Append(4);
  data.Append(aValue);
  data.Append(0);
  CSubBlock* block=CSubBlock::NewL(data,0,KPhoneLightUnit);
  CleanupStack::PushL(block);
  CLightBrightnessSetReq* send=CLightBrightnessSetReq::NewL(0,block);
  SendL(send);
  CleanupStack::PopAndDestroy(); //block
}

void CBackLightControlImpl::BrightnessL(TUint8& aValue)
{
  TBuf8<4> data;
  data.Append(0);
  data.Append(0);
  data.Append(0);
  data.Append(32);
  CLightBrightnessGetReq* send=CLightBrightnessGetReq::NewL(0,data);
  CIsiMsg* recv=SendAndReceiveLC(send);
  if(recv->Ptr()[9]!=2) User::Leave(KErrGeneral);
  CLightBrightnessGetResp* resp=new(ELeave)CLightBrightnessGetResp;
  CleanupStack::PushL(resp);
  recv->Move(resp);
  if(!resp->SubBlockCount()) User::Leave(KErrUnknown);
  CSubBlock* block=resp->SubBlock();
  CleanupStack::PushL(block);
  if(block->Ptr()[0]!=3) User::Leave(KErrUnknown);
  CLightBrightnessInfo* info=new(ELeave)CLightBrightnessInfo;
  block->Move(info);
  aValue=info->Brightness1();
  delete info;
  CleanupStack::PopAndDestroy(3); //block,resp,recv
}

CIsiMsg* CBackLightControlImpl::SendAndReceiveLC(CIsiMsg* aSend)
{
  CleanupStack::PushL(aSend);
  CIsiMsg* recv=CIsiMsg::NewL(500);
  CleanupStack::PushL(recv);
  User::LeaveIfError(iPhoNet.Send(*aSend));
  TRequestStatus status;
  TPnReceiveAllocationLengthPckg pckg;
  User::LeaveIfError(iPhoNet.Receive(status,*recv,pckg));
  User::WaitForRequest(status);
  User::LeaveIfError(status.Int());
  CleanupStack::Pop(); //recv
  CleanupStack::PopAndDestroy(); //aSend
  CleanupStack::PushL(recv);
  return recv;
}

void CBackLightControlImpl::SendL(CIsiMsg* aSend)
{
  SendAndReceiveLC(aSend);
  CleanupStack::PopAndDestroy(); //recv
}
