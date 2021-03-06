/*
    hwlight.cpp
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

#include "hwtricks.hpp"
#include <isimsg.hpp>
#include <isi_light.hpp>
#include <isi_units.hpp>
#include <CommonEngine.hpp>
#include <SharedData.hpp>

const TUint8 KBrightnesParam[]={1,5,6,2,7,3,4};

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

EXPORT_C void HWBacklight::SwitchL(TInt aType,TInt aState)
{
  CLightSwitchReq* send=CLightSwitchReq::NewL(0,aType,aState,(CSubBlockArray*)NULL);
  CHWServer::SendL(send);
}

EXPORT_C void HWBacklight::SwitchL(TInt aType,TInt aState,const RArray<SParam>& aParams)
{
  HBufC8* data=HBufC8::NewLC(aParams.Count()*4);
  TPtr8 ptr=data->Des();
  CSubBlockArray* array=CSubBlockArray::NewL(aParams.Count());
  CleanupStack::PushL(array);
  for(TInt i=0;i<aParams.Count();i++)
  {
    ptr.Append(KBrightnesParam[aParams[i].iType]);
    ptr.Append(4);
    ptr.Append(aParams[i].iValue1);
    ptr.Append(aParams[i].iValue1);
    (*array)[i]=CSubBlock::NewL(ptr,ptr.Length()-4,KPhoneLightUnit);
  }
  CLightSwitchReq* send=CLightSwitchReq::NewL(0,aType,aState,array);
  CHWServer::SendL(send);
  CleanupStack::PopAndDestroy(2); //array,data
}

EXPORT_C void HWBacklight::Reserved_1(void)
{
  User::Leave(KErrNotSupported);
}

EXPORT_C void HWBacklight::SetBrightnessL(TBrightnessType aType,TUint8 aValue1,TUint8 aValue2)
{
  TBuf8<4> data;
  data.Append(KBrightnesParam[aType]);
  data.Append(4);
  data.Append(aValue1);
  data.Append(aValue2);
  CSubBlock* block=CSubBlock::NewL(data,0,KPhoneLightUnit);
  CleanupStack::PushL(block);
  CLightBrightnessSetReq* send=CLightBrightnessSetReq::NewL(0,block);
  CHWServer::SendL(send);
  CleanupStack::PopAndDestroy(); //block
}

EXPORT_C void HWBacklight::SetBrightnessL(const RArray<SParam>& aParams)
{
  HBufC8* data=HBufC8::NewLC(aParams.Count()*4);
  TPtr8 ptr=data->Des();
  CSubBlockArray* array=CSubBlockArray::NewL(aParams.Count());
  CleanupStack::PushL(array);
  for(TInt i=0;i<aParams.Count();i++)
  {
    ptr.Append(KBrightnesParam[aParams[i].iType]);
    ptr.Append(4);
    ptr.Append(aParams[i].iValue1);
    ptr.Append(aParams[i].iValue1);
    (*array)[i]=CSubBlock::NewL(ptr,ptr.Length()-4,KPhoneLightUnit);
  }
  CLightBrightnessSetReq* send=CLightBrightnessSetReq::NewL(0,array);
  CHWServer::SendL(send);
  CleanupStack::PopAndDestroy(2); //array,data
}

EXPORT_C void HWBacklight::BrightnessL(TBrightnessType aType,TUint8& aValue1,TUint8& aValue2)
{
  TBuf8<4> data;
  data.Append(0);
  data.Append(0);
  data.Append(0);
  data.Append(1<<aType);
  CLightBrightnessGetReq* send=CLightBrightnessGetReq::NewL(0,data);
  CIsiMsg* recv=CHWServer::SendAndReceiveLC(send);
  if(recv->Ptr()[9]!=2) User::Leave(KErrGeneral);
  CLightBrightnessGetResp* resp=new(ELeave)CLightBrightnessGetResp;
  CleanupStack::PushL(resp);
  recv->Move(resp);
  if(!resp->SubBlockCount()) User::Leave(KErrUnknown);
  CSubBlock* block=resp->SubBlock();
  CleanupStack::PushL(block);
  if(block->Ptr()[0]!=KBrightnesParam[aType]) User::Leave(KErrUnknown);
  CLightBrightnessInfo* info=new(ELeave)CLightBrightnessInfo;
  block->Move(info);
  aValue1=info->Brightness1();
  aValue2=info->Brightness2();
  delete info;
  CleanupStack::PopAndDestroy(3); //block,resp,recv
}

EXPORT_C void HWBacklight::BrightnessL(RArray<SParam>& aParams)
{
  TBuf8<4> data;
  data.Append(0);
  data.Append(0);
  data.Append(0);
  TUint8 values=0;
  for(TInt i=0;i<aParams.Count();i++) values|=1<<aParams[i].iType;
  data.Append(values);
  CLightBrightnessGetReq* send=CLightBrightnessGetReq::NewL(0,data);
  CIsiMsg* recv=CHWServer::SendAndReceiveLC(send);
  if(recv->Ptr()[9]!=2) User::Leave(KErrGeneral);
  CLightBrightnessGetResp* resp=new(ELeave)CLightBrightnessGetResp;
  CleanupStack::PushL(resp);
  recv->Move(resp);
  CSubBlockArray* array=resp->SubBlocksL();
  CleanupStack::PushL(array);
  for(TInt i=0;i<array->Number();i++)
  {
    CSubBlock* block=(*array)[i];
    for(TInt j=0;j<aParams.Count();j++)
    {
      if(block->Ptr()[0]==KBrightnesParam[aParams[j].iType])
      {
        CLightBrightnessInfo* info=new(ELeave)CLightBrightnessInfo;
        block->Move(info);
        aParams[j].iValue1=info->Brightness1();
        aParams[j].iValue2=info->Brightness2();
        delete info;
        break;
      }
    }
  }
  CleanupStack::PopAndDestroy(3); //array,resp,recv
}

EXPORT_C void HWBacklight::Reserved_3(void)
{
  User::Leave(KErrNotSupported);
}
