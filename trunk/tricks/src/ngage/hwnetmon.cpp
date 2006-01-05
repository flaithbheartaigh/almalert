/*
    hwnetmon.cpp
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

#include "hwtricksnetmon.hpp"
#include <isi_units.hpp>

EXPORT_C void HWNetmon::ValueL(TUint8 aUnit,TUint16 anAddress,TDes16& aValue,TUint32 aFlags)
{
  CNetmonValue* value=CNetmonValue::NewLC(aUnit,anAddress,aFlags&EExt);
  value->ValueL(aValue,aFlags&ERaw);
  CleanupStack::PopAndDestroy(); //value
}

EXPORT_C void HWNetmon::SetChannelL(TUint16 aChannel)
{
  TBuf8<4> data;
  data.Append(0);
  data.Append(0);
  data.Append((TUint8)(aChannel>>8));
  data.Append((TUint8)aChannel);
  CHWServer* server=CHWServer::NewLC();
  CTestSetReq* sendMsg=CTestSetReq::NewL(0,KPhoneNetUnit,6,0,0x1303,0,data);
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

EXPORT_C void HWNetmon::SetFrequencyL(TBandFrequency aFrequency)
{
  TBuf8<4> data;
  data.Append(0);
  data.Append(0);
  data.Append(0);
  switch(aFrequency)
  {
    case E900MHz:
      data.Append(9);
      break;
    case E1800MHz:
      data.Append(18);
      break;
    case E1900MHz:
      data.Append(19);
      break;
    default:
      data.Append(0);
      break;
  }
  CHWServer* server=CHWServer::NewLC();
  CTestSetReq* sendMsg=CTestSetReq::NewL(0,KPhoneGssUnit,6,0,0x32f2,0,data);
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

CNetmonValue::~CNetmonValue()
{
  TRAPD(err,CleanupL());
  delete iSendMsg;
  delete iRecvMsg;
  delete iServer;
}

CNetmonValue* CNetmonValue::NewLC(TUint8 aUnit,TUint16 aAddress,TBool anExtended)
{
  CNetmonValue* self=new(ELeave)CNetmonValue(aUnit,aAddress);
  CleanupStack::PushL(self);
  self->ConstructL(anExtended);
  return self;
}

void CNetmonValue::ValueL(TDes16& aTarget,TBool aRaw)
{
  CTestGetResp* resp=new(ELeave)CTestGetResp;
  CleanupStack::PushL(resp);
  iRecvMsg->Move(resp);
  CSubBlockArray* array=resp->SubBlocksL();
  CleanupStack::PopAndDestroy(); //resp
  CleanupStack::PushL(array);
  CSubBlock* block=NULL;
  for(TInt i=0;i<array->Number();i++)
  {
    if((*array)[i]->Address()==iAddress)
    {
      block=(*array)[i];
      break;
    }
  }
  if(!block) User::Leave(KErrNotFound);
  if(aRaw)
  {
    aTarget.Copy(block->Ptr().Mid(3));
  }
  else
  {
    TInt blockLen=block->Ptr()[2];
    if(blockLen<4) User::Leave(KErrUnderflow);
    if(blockLen==4)
    {
      aTarget.Num((TUint)block->Ptr()[3]);
    }
    else
    {
      TInt length=block->Ptr()[3];
      if(blockLen==8&&length==0)
      {
        aTarget.Num((TInt16)(block->Ptr()[4]*256+block->Ptr()[5]));
      }
      else if((length+3)<block->Ptr().Length())
      {
        if(length&&block->Ptr()[3+length]==0) --length;
        aTarget.Copy(block->Ptr().Mid(4,length));
      }
      else User::Leave(KErrUnderflow);
    }
  }
  CleanupStack::PopAndDestroy(); //array
}

CNetmonValue::CNetmonValue(TUint8 aUnit,TUint16 aAddress): CBase(),iUnit(aUnit),iAddress(aAddress)
{
}

void CNetmonValue::ConstructL(TBool anExtended)
{
  iServer=CHWServer::NewLC(anExtended);
  CleanupStack::Pop();
  TBuf<1> data;
  data.Append(iAddress);
  iSendMsg=CTestGetReq::NewL(0,iUnit,0x4,0x1,data);
  iServer->SendL(*iSendMsg);
  iRecvMsg=CIsiMsg::NewL(500);
  TRequestStatus status;
  TPnReceiveAllocationLengthPckg pckg;
  iServer->ReceiveL(status,*iRecvMsg,pckg);
  User::WaitForRequest(status);
  if(iRecvMsg->Function()!=0xf0||iRecvMsg->Ptr()[10]!=5) User::Leave(KErrNotSupported);
}

void CNetmonValue::CleanupL(void)
{
  if(!iServer) return;
  TBuf<1> data;
  data.Append(iAddress);
  CIsiMsg* sendMsg=CTestRemoveReq::NewL(0,iUnit,0x8,0x1,data);
  CleanupStack::PushL(sendMsg);
  iServer->SendL(*sendMsg);
  CIsiMsg* recvMsg=CIsiMsg::NewL(500);
  CleanupStack::PushL(recvMsg);
  TRequestStatus status;
  TPnReceiveAllocationLengthPckg pckg;
  iServer->ReceiveL(status,*recvMsg,pckg);
  User::WaitForRequest(status);
  CleanupStack::PopAndDestroy(2); //recvMsg,sendMsg
}
