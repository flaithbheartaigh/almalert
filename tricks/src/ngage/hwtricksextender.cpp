/*
    hwtricksextender.cpp
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

#include "hwtricksextender.hpp"
#include <f32file.h>
#include <isi_test.hpp>
#define sizeofa(array) (sizeof(array)/sizeof(array[0]))

CHWExtender* CHWExtender::NewL(CHWServer* aServer)
{
  CHWExtender* self=new(ELeave)CHWExtender;
  CleanupStack::PushL(self);
  self->ConstructL(aServer);
  CleanupStack::Pop(); //self
  return self;
}

CHWExtender::~CHWExtender()
{
  if(iPatched)
  {
    TRAPD(err,PatchL(iAddress,iOldValue,iOldValue));
  }
}

CHWExtender::CHWExtender()
{
}

void CHWExtender::ConstructL(CHWServer* aServer)
{
  iAddress=Address();
  if(!iAddress) User::Leave(KErrNotSupported);
  PatchL(iAddress,0,iOldValue);
  iPatched=ETrue;
  CTestStateReq* send=CTestStateReq::NewL(0);
  CleanupStack::PushL(send);
  aServer->SendL(*send);
  CIsiMsg* recv=CIsiMsg::NewL(500);
  CleanupStack::PushL(recv);
  TRequestStatus status;
  TPckgBuf<TUint16> pckg;
  aServer->ReceiveL(status,*recv,pckg);
  User::WaitForRequest(status);
  if(recv->Ptr()[10]!=KTestStateOn) User::Leave(KErrGeneral);
  CleanupStack::PopAndDestroy(2); //send,recv
}

TBool CHWExtender::IsSupported(void)
{
  return Address()!=0;
}

TUint CHWExtender::Address(void)
{
  // iSign - 1st export from euser.dll (memset)
  struct SModels
  {
    TUint iSign;
    TUint iOffset;
  };
  const SModels KModels[]=
  {
    {0x50397d84u,0x800c889cu}, //n-gage classic 4.03
    {0x50397984u,0x800ca480u}, //n-gage classic 3.30
    {0x50392d54u,0x800c7ea8u}, //n-gage qd 4.00 & 4.10
    {0x50392c74u,0x800c7ea8u}, //n-gage qd 3.15
    {0x503b2a74u,0x800c78c0u}  //3650 4.17
  };
  _LIT(KEuserDll,"euser.dll");

  TUint res=0;
  RLibrary lib;
  if(lib.Load(KEuserDll)==KErrNone)
  {
    TUint sign=(TUint)lib.EntryPoint();
    if(sign)
      for(TUint i=0;i<sizeofa(KModels);i++)
        if(sign==KModels[i].iSign)
        {
          res=KModels[i].iOffset;
          break;
        }
    lib.Close();
  }
  return res;
}

void CHWExtender::PatchL(TUint anAddress,TUint8 aValue,TUint8& anOldValue)
{
  _LIT(KKernelThreadMask,"ekern[100000b9]::supervisor");
  TFullName result;
  TFindThread threads(KKernelThreadMask);
  User::LeaveIfError(threads.Next(result));
  RThread thread;
  User::LeaveIfError(thread.Open(threads));
  CleanupClosePushL(thread);
  User::LeaveIfError(user::Open(16,16,16,0x10000));
  CleanupStack::PushL(TCleanupItem((TCleanupOperation)user::Close,NULL));
  TUint8 byte;
  TPtr8 byteP(&byte,sizeof(byte),sizeof(byte));
  User::LeaveIfError(user::op(Read,thread.Id(),anAddress,byte));
  anOldValue=byte;
  byte=aValue;
  User::LeaveIfError(user::op(Write,thread.Id(),anAddress,byte));
  CleanupStack::PopAndDestroy(2); //user,thread
}
