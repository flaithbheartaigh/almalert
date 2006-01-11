/*
    hwlight.cpp
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
#include <isimsg.hpp>
#include <isi_light.hpp>
#include <CommonEngine.hpp>
#include <SharedData.hpp>

EXPORT_C void HWBacklight::SetGameModeL(TBool aState)
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

EXPORT_C void HWBacklight::SwitchL(TInt aType,TInt aState,void* aParams=NULL)
{
  CHWServer* server=CHWServer::NewLC();
  CLightSwitchReq* sendMsg=CLightSwitchReq::NewL(0,aType,aState,NULL);
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

EXPORT_C void HWBacklight::Reserved_1(void)
{
  User::Leave(KErrNotSupported);
}

const TUint8 KBrightnesParam[]={1,5,6,2,7,3,4};

EXPORT_C void HWBacklight::SetBrightnessL(TBrightnessType aType,TUint8 aValue1,TUint8 aValue2)
{
  CHWServer* server=CHWServer::NewLC();
  TBuf8<4> data;
  data.Append(KBrightnesParam[aType]);
  data.Append(4);
  data.Append(aValue1);
  data.Append(aValue2);
  CSubBlock* subBlock=CSubBlock::NewL(data,0,0x3a);
  CleanupStack::PushL(subBlock);
  CLightBrightnessSetReq* sendMsg=CLightBrightnessSetReq::NewL(0,subBlock);
  CleanupStack::PushL(sendMsg);
  server->SendL(*sendMsg);
  CIsiMsg* recvMsg=CIsiMsg::NewL(500);
  CleanupStack::PushL(recvMsg);
  TRequestStatus status;
  TPnReceiveAllocationLengthPckg pckg;
  server->ReceiveL(status,*recvMsg,pckg);
  User::WaitForRequest(status);
  CleanupStack::PopAndDestroy(4); //recvMsg,sendMsg,subBlock,server
}

EXPORT_C void HWBacklight::BrightnessL(TBrightnessType aType,TUint8& aValue1,TUint8& aValue2)
{
  CHWServer* server=CHWServer::NewLC();
  TBuf8<4> data;
  data.Append(0);
  data.Append(0);
  data.Append(0);
  data.Append(1<<aType);
  CLightBrightnessGetReq* sendMsg=CLightBrightnessGetReq::NewL(0,data);
  CleanupStack::PushL(sendMsg);
  server->SendL(*sendMsg);
  CIsiMsg* recvMsg=CIsiMsg::NewL(500);
  CleanupStack::PushL(recvMsg);
  TRequestStatus status;
  TPnReceiveAllocationLengthPckg pckg;
  server->ReceiveL(status,*recvMsg,pckg);
  User::WaitForRequest(status);
  if(recvMsg->Ptr()[9]!=2) User::Leave(KErrGeneral);
  CLightBrightnessGetResp* resp=new(ELeave)CLightBrightnessGetResp;
  CleanupStack::PushL(resp);
  recvMsg->Move(resp);
  if(!resp->SubBlockCount()) User::Leave(KErrUnknown);
  CSubBlock* subBlock=resp->SubBlock();
  CleanupStack::PushL(subBlock);
  if(subBlock->Ptr()[0]!=KBrightnesParam[aType]) User::Leave(KErrUnknown);
  CLightBrightnessInfo* info=new(ELeave)CLightBrightnessInfo;
  subBlock->Move(info);
  aValue1=info->Brightness1();
  aValue2=info->Brightness2();
  delete info;
  CleanupStack::PopAndDestroy(5); //subBlock,resp,recvMsg,sendMsg,server
}

EXPORT_C void HWBacklight::Reserved_2(void)
{
  User::Leave(KErrNotSupported);
}

EXPORT_C void HWBacklight::Reserved_3(void)
{
  User::Leave(KErrNotSupported);
}

EXPORT_C void HWBacklight::Reserved_4(void)
{
  User::Leave(KErrNotSupported);
}

EXPORT_C void HWBacklight::Reserved_5(void)
{
  User::Leave(KErrNotSupported);
}
