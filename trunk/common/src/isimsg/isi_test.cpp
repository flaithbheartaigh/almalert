/*
    isi_test.cpp
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

#include <isi_test.hpp>
#include <isi_utils.hpp>

EXPORT_C CTestGetReq* CTestGetReq::NewL(TUint8 aTransactionId,TUint8 aUnit,TUint8 aParam,TUint8 aCount,const TDesC16& aData)
{
  CTestGetReq* self=new(ELeave)CTestGetReq;
  TInt length=aData.Length();
  length*=2;
  length+=12;
  CleanupStack::PushL(self);
  self->ConstructL(length);
  TPtr8& ptr=self->iPtr;
  self->SetUnit(aUnit);
  ptr.Append(aTransactionId);
  ptr.Append(0xf0);
  ptr.Append(aParam);
  ptr.Append(aCount);
  TBuf8<100> buffer;
  Des162Des8(aData,buffer);
  ptr.Append(buffer);
  CleanupStack::Pop(); //self
  return self;
}

EXPORT_C CTestRemoveReq* CTestRemoveReq::NewL(TUint8 aTransactionId,TUint8 aUnit,TUint8 aParam,TUint8 aCount,const TDesC16& aData) //971
{
  CTestRemoveReq* self=new(ELeave)CTestRemoveReq;
  TInt length=aData.Length();
  length*=2;
  length+=12;
  CleanupStack::PushL(self);
  self->ConstructL(length);
  TPtr8& ptr=self->iPtr;
  self->SetUnit(aUnit);
  ptr.Append(aTransactionId);
  ptr.Append(0xf0);
  ptr.Append(aParam);
  ptr.Append(aCount);
  TBuf8<100> buffer;
  Des162Des8(aData,buffer);
  ptr.Append(buffer);
  CleanupStack::Pop(); //self
  return self;
}

EXPORT_C CTestSetReq* CTestSetReq::NewL(TUint8 aTransactionId,TUint8 aUnit,TUint8 aParam1,TUint8 aParam2,TUint16 aParam3,TUint16 aParam4,const TDesC16& aData)
{
  CTestSetReq* self=new(ELeave)CTestSetReq;
  CleanupStack::PushL(self);
  self->ConstructL(20);
  TPtr8& ptr=self->iPtr;
  self->SetUnit(aUnit);
  ptr.Append(aTransactionId);
  ptr.Append(0xf0);
  ptr.Append(aParam1);
  ptr.Append(aParam2);
  ptr.Append((TUint8)(aParam3>>8));
  ptr.Append((TUint8)aParam3);
  ptr.Append((TUint8)(aParam4>>8));
  ptr.Append((TUint8)aParam4);
  if(aData.Length()!=4)
  {
    User::Leave(KErrGeneral);
  }
  else
  {
    self->iPtr.Append(aData);
  }
  CleanupStack::Pop(); //self
  return self;
}

EXPORT_C CSubBlockArray* CTestGetResp::SubBlocksL()
{
  TInt countIndex=SubBlockCountIndex();
  if(!countIndex) return NULL;
  TInt dummy=1;
  TUint8 count;
  if(countIndex!=-dummy) count=iPtr[countIndex];
  else count=dummy;
  TInt start=SubBlockStart();
  CSubBlockArray* array=CSubBlockArray::NewL(count);
  for(TUint8 i=0;i<count;i++)
  {
    CSubBlock* block=CSubBlock::NewL0(iPtr,start);
    (*array)[i]=block;
    start+=block->Length();
  }
  return array;
}

EXPORT_C TInt CTestGetResp::SubBlockCountIndex(void)
{
  return 11;
}

EXPORT_C TInt CTestGetResp::SubBlockStart(void)
{
  return 12;
}

#if 0
LOCAL_C void Copy1(TPtrC8 aSource,TDes16& aTarget)
{
  aTarget.Zero();
  TInt length=aSource.Length();
  aTarget.SetLength(length/2);
  const TUint8* src=&aSource[0];
  TUint8* dest=(TUint8*)&aTarget[0];
  for(TInt i=0;i<length;src++,i++)
  {
    TUint8* dest2=dest++;
    if(i&1)
    {
      *dest2=*(src-1);
    }
    else
    {
      *dest2=*(src+1);
    }
  }
}

LOCAL_C const TDesC8& Copy2(TDes16& aSource)
{
  TInt length=aSource.Length();
  if(!length) User::Leave(KErrGeneral);
  TBuf8<256> buffer;
  TInt length2=length*2;
  buffer.SetLength(length2);
  TUint8* src=(TUint8*)&aSource[0];
  TUint8* dest=&buffer[0];
  for(TInt i=0;i<length2;i++)
  {
    TUint8* dest2=dest++;
    if(i&1)
    {
      *dest2=*(src++-1);
    }
    else
    {
      *dest2=*(++src);
    }
  }
  return buffer;
}
#endif

EXPORT_C void CTestInfo::String(TDes16& aTarget)
{
#if 0
  Copy1(iPtr.Mid(4,iPtr[3]),aTarget);
  const TDesC8& data=Copy2(aTarget);
  aTarget.Copy(data);
#else
  aTarget.Copy(iPtr.Mid(4,iPtr[3]));
#endif
}
