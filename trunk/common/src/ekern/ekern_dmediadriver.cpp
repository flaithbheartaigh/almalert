/*
    ekern_dmediadriver.cpp
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

#include <k32std61.hpp>

EXPORT_C DMediaDriver61::DMediaDriver61()
{
}

TInt DMediaDriver61::RequestSetup(TInt aReqNo,TInt64 &aPos,TInt* aLength,DThread* aThread,TRequestStatus* aStatus)
{
  if(*aLength<=0||(aPos+TInt64(*aLength))>iTotalSizeInBytes)
  {
    aThread->RequestComplete(aStatus,KErrArgument);
    return KErrNotFound;
  }
  if((iFlags&1)?IsPending(aReqNo):AnyPending())
  {
    aThread->RequestComplete(aStatus,KErrInUse);
    return KErrNotFound;
  }
  iReqThread[aReqNo]=aThread;
  iReqStat[aReqNo]=aStatus;
  iReqLenAddr[aReqNo]=aLength;
  return KErrNone;
}

void DMediaDriver61::Read(TInt64& aPos,TInt* aLength,TLocalDriveMessageData& aTrg,DThread* aThread,TRequestStatus* aStatus)
{
  TMediaDrvDescData trg(aTrg);
  if(RequestSetup(KMediaDrvReadReq,aPos,aLength,aThread,aStatus)==KErrNone) DoRead(aPos,*aLength,trg);
}

void DMediaDriver61::Write(TInt64& aPos,TInt* aLength,TLocalDriveMessageData& aSrc,DThread* aThread,TRequestStatus* aStatus)
{
  TMediaDrvDescData src(aSrc);
  if(RequestSetup(KMediaDrvWriteReq,aPos,aLength,aThread,aStatus)==KErrNone) DoWrite(aPos,*aLength,src);
}

void DMediaDriver61::Format(TInt64& aPos,TInt* aLength,DThread* aThread,TRequestStatus* aStatus)
{
  if(RequestSetup(KMediaDrvFormatReq,aPos,aLength,aThread,aStatus)==KErrNone) DoFormat(aPos,*aLength);
}

void DMediaDriver61::PartitionInfo(TPartitionInfo& anInfo)
{
  anInfo=iPartitionInfo;
}

EXPORT_C void DMediaDriver61::DeltaCurrentConsumptionInMilliAmps(TInt aValue)
{
  K::PowerModel->iCurrentConsumptionInMilliAmps+=aValue;
}

EXPORT_C void DMediaDriver61::Complete(TInt aReqNo,TInt aReason)
{
  iReqThread[aReqNo]->RequestComplete(iReqStat[aReqNo],aReason);
}

EXPORT_C void DMediaDriver61::CompleteAll(TInt aReason)
{
  for(TInt i=0;i<KMediaDrvMaxReqs;i++)
    if(iReqStat[i]) iReqThread[i]->RequestComplete(iReqStat[i],aReason);
}

EXPORT_C void DMediaDriver61::Complete(TInt aReqNo,TInt aReason,TInt aLenWriteBack)
{
  iReqThread[aReqNo]->Write(iReqLenAddr[aReqNo],(TUint8*)&aLenWriteBack,sizeof(aLenWriteBack));
  iReqThread[aReqNo]->RequestComplete(iReqStat[aReqNo],aReason);
}

EXPORT_C void DMediaDriver61::SetTotalSizeInBytes(TInt64 aTotalSizeInBytes)
{
  iTotalSizeInBytes=aTotalSizeInBytes;
}

EXPORT_C TInt64 DMediaDriver61::TotalSizeInBytes(void)
{
  return iTotalSizeInBytes;
}

EXPORT_C TInt DMediaDriver61::DoSyncRead(TInt64& aPos,TInt& aLength,TMediaDrvDescData& aTrg)
{
  return KErrNotSupported;
}

EXPORT_C TInt DMediaDriver61::DoSyncWrite(TInt64& aPos,TInt& aLength,TMediaDrvDescData& aSrc)
{
  return KErrNotSupported;
}

EXPORT_C TInt DMediaDriver61::DoSyncFormat(TInt64& aPos,TInt& aLength)
{
  return KErrNotSupported;
}

EXPORT_C void DMediaDriver61::PasswordControl(TPBusPasswordFunc aFunc,TPBusPasswordData& aPasswordData,DThread* aThread,TRequestStatus* aStat)
{
  aThread->RequestComplete(aStat,KErrNotSupported);
}

TDriveChangedFlags::TDriveChangedFlags(DThread* aThread,TInt* aMountStatus,TBool* aChangedFlag,TInt aDriveNum): iOpenReqPending(EFalse),iOwningThread(aThread),iMountStatus(aMountStatus),iChangedFlag(aChangedFlag),iDriveNum(aDriveNum)
{
}

TBool TDriveChangedFlags::ThreadIsValid()
{
  for(TInt i=K::Threads->Count();i>0;i--)
  {
    if((*K::Threads)[i-1]==iOwningThread) return ETrue;
  }
  return EFalse;
}

TInt TDriveChangedFlags::UpdateMountStatus()
{
  if(!ThreadIsValid()) return KErrGeneral;
  if(iMountStatus)
  {
    TInt value=KErrNotReady;
    iOwningThread->Write(iMountStatus,(TUint8*)&value,sizeof(value));
  }
  return KErrNone;
}


TInt TDriveChangedFlags::UpdateMediaChange()
{
  if(!ThreadIsValid()) return KErrGeneral;
  if(iChangedFlag)
  {
    TBool value=ETrue;
    iOwningThread->Write(iChangedFlag,(TUint8*)&value,sizeof(value));
  }
  return KErrNone;
}

TInt TDriveChangedFlags::CompleteOpenRequest(TInt aReason)
{
  if(!ThreadIsValid()) return KErrGeneral;
  if(iOpenReqPending&&iMountStatus)
  {
    TInt value;
    iOwningThread->Read(iMountStatus,(TUint8*)&value,sizeof(value));
    if(value!=KRequestPending) aReason=value;
    TRequestStatus* status=(TRequestStatus*)iMountStatus;
    iOwningThread->RequestComplete(status,aReason);
    iOpenReqPending=EFalse;
  }
  return KErrNone;
}

TPartitionInfo::TPartitionInfo(): iMediaSizeInBytes(0),iPartitionCount(0)
{
  for(TInt i=0;i<KMaxPartitionEntries;i++)
  {
    iEntry[i].iPartitionBaseAddr=0;
    iEntry[i].iPartitionLen=0;
  }
}
