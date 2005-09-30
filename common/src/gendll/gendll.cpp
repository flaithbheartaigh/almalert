/*
    gendll.cpp
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

#include <gendll.hpp>

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
{
  return KErrNone;
}

EXPORT_C CGeneral::CGeneral(): CActive(EPriorityStandard),iRecvMsg(NULL),iPhoNet(NULL),iSendMsg(NULL),iBuf(NULL),iPckgLength(0)
{
  CActiveScheduler::Add(this);
}

EXPORT_C CGeneral::~CGeneral()
{
  if(iPhoNet!=NULL)
  {
    if(iStatus==KRequestPending) Cancel();
    iPhoNet->Close();
    delete iPhoNet;
  }
}

EXPORT_C void CGeneral::ConstructL(const TUint8 aObjectNumber)
{
  iPhoNet=new(ELeave)RPhoNet;
  User::LeaveIfError(iPhoNet->Open(aObjectNumber));
}

EXPORT_C void CGeneral::RunL(void)
{
  if(iStatus!=KErrNone)
  {
    ErrorResponse();
  }
  else
  {
    TRAPD(err,Response());
  }
}

EXPORT_C void CGeneral::DoCancel(void)
{
  iPhoNet->ReceiveCancel();
  delete iRecvMsg;
  iRecvMsg=NULL;
}

void CGeneral::Response(void)
{
  TUint8 transactionId=iRecvMsg->Transaction();
  TUint8 unit=iRecvMsg->Unit();
  TUint8 function=iRecvMsg->Function();
  TInt len=iRecvMsg->Ptr().Length();
  if(len>10)
  {
    iResponseData.Set(iRecvMsg->Ptr().Right(len-10));
  }
  else
  {
    iResponseData.Set(NULL,0);
  }
  DoResponse(function,transactionId,unit);
}

void CGeneral::ErrorResponse(void)
{
  if(KErrNoMemory==iStatus.Int())
  {
    delete iRecvMsg;
    iRecvMsg=NULL;
    iRecvMsg=CIsiMsg::NewL(iPckgLength());
    iPckgLength()=0;
    iPhoNet->Receive(iStatus,*iRecvMsg,iPckgLength);
    SetActive();
  }
  else
  {
    delete iRecvMsg;
    iRecvMsg=NULL;
  }
}

EXPORT_C TInt CGeneral::DoSendL(TUint8 aFunction,TDesC8* aData,TUint8 aUnit,TUint8 aParam4,TUint8 aParam5,TUint8 aTransactionId)
{
  User::LeaveIfError(CreateCustomMessage(aFunction,aData,aUnit,aParam4,aParam5,aTransactionId));
  return SendCustomMessage();
}

EXPORT_C TInt CGeneral::DoSendL(CPnMsg &aMsg)
{
  return iPhoNet->Send(aMsg);
}

EXPORT_C void CGeneral::DoReceiveL(void)
{
  TInt active=IsActive();
  if(!active)
  {
    if(iRecvMsg)
    {
      delete iRecvMsg;
      iRecvMsg=(CIsiMsg*)active;
    }
    iRecvMsg=CIsiMsg::NewL(500);
    User::LeaveIfError(iPhoNet->Receive(iStatus,*iRecvMsg,iPckgLength));
    SetActive();
  }
}

TInt CGeneral::CreateCustomMessage(TUint8 aFunction,TDesC8* aData,TUint8 aUnit,TUint8 aParam4,TUint8 aParam5,TUint8 aTransactionId)
{
  TUint len=aTransactionId;
  if(aData) len=aData->Length()+10;
  else len=10;
  delete iBuf;
  iBuf=NULL;
  TRAPD(err,iBuf=HBufC8::NewL(len));
  if(err!=KErrNone) return err;
  TPtr8 ptr=iBuf->Des();
  ptr.Append(0);
  ptr.Append(aParam4);
  ptr.Append(0);
  ptr.Append(aUnit);
  TUint len2=len-6;
  len-=6;
  ptr.Append(len);
  ptr.Append(len2/256);
  ptr.Append(aParam5);
  ptr.Append(0);
  ptr.Append(aTransactionId);
  ptr.Append(aFunction);
  if(aData) ptr.Append(*aData);
  TRAP(err,iSendMsg=CPnMsg::NewL(ptr.Length()));
  if(err!=KErrNone)
  {
    delete iBuf;
    iBuf=NULL;
    return err;
  }
  else
  {
    iSendMsg->Ptr().Set(ptr);
    return KErrNone;
  }
}

TInt CGeneral::SendCustomMessage(void)
{
  TInt err=iPhoNet->Send(*iSendMsg);
  delete iSendMsg;
  iSendMsg=NULL;
  return err;
}

EXPORT_C TInt CGeneral::Unknown_10(void)
{
  return KErrNone;
}

EXPORT_C TInt CGeneral::SendIsiEvent(CPnMsg &aMsg) const
{
  return iPhoNet->SendIsiEvent(aMsg);
}

EXPORT_C TInt CGeneral::Unknown_12(void)
{
  return KErrNotSupported;
}

EXPORT_C TInt CGeneral::Unknown_07(void)
{
  return KErrNotSupported;
}
