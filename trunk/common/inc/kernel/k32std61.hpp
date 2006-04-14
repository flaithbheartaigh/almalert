/*
    k32std61.hpp
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

#ifndef __K32STD61_HPP__
#define __K32STD61_HPP__

class TProcessMemoryInfo; //don't exists in symbian 6.1
#include <k32std.h>
#include <e32std.h>

class DMediaDriver61: public CBase
{
  public:
    IMPORT_C DMediaDriver61();
    void Read(TInt64& aPos,TInt* aLength,TLocalDriveMessageData& aTrg,DThread* aThread,TRequestStatus* aStatus);
    void Write(TInt64& aPos,TInt* aLength,TLocalDriveMessageData& aSrc,DThread* aThread,TRequestStatus* aStatus);
    void Format(TInt64& aPos,TInt* aLength,DThread* aThread,TRequestStatus* aStatus);
    void PartitionInfo(TPartitionInfo& anInfo);
    virtual TInt Enlarge(TInt aLength)=0;
    virtual TInt ReduceSize(TInt64& aPos,TInt aLength)=0;
    virtual void Close(void)=0;
    virtual void Caps(TDes8& aCapsBuf)=0;
    virtual void ReadPartitionInfo(TInt& aResult,const TCallBack& aCallBack)=0;
    IMPORT_C virtual void PasswordControl(TPBusPasswordFunc aFunc,TPBusPasswordData& aPasswordData,DThread* aThread,TRequestStatus* aStat);
    IMPORT_C void DeltaCurrentConsumptionInMilliAmps(TInt aValue);
  protected:
    IMPORT_C void Complete(TInt aReqNo,TInt aReason);
    IMPORT_C void Complete(TInt aReqNo,TInt aReason,TInt aLenWriteBack);
    IMPORT_C void CompleteAll(TInt aReason);
    inline TBool IsPending(TInt aReqNo) const;
    inline TBool AnyPending(void) const;
    IMPORT_C void SetTotalSizeInBytes(TInt64 aTotalSizeInBytes);
    IMPORT_C TInt64 TotalSizeInBytes(void);
    virtual void DoRead(TInt64& aPos,TInt aLength,TMediaDrvDescData& aTrg)=0;
    virtual void DoWrite(TInt64& aPos,TInt aLength,TMediaDrvDescData& aSrc)=0;
    virtual void DoFormat(TInt64& aPos,TInt aLength)=0;
    IMPORT_C virtual TInt DoSyncRead(TInt64& aPos,TInt& aLength,TMediaDrvDescData& aTrg);
    IMPORT_C virtual TInt DoSyncWrite(TInt64& aPos,TInt& aLength,TMediaDrvDescData& aSrc);
    IMPORT_C virtual TInt DoSyncFormat(TInt64& aPos,TInt& aLength);
  private:
    TInt RequestSetup(TInt aReqNo,TInt64 &aPos,TInt* aLength,DThread* aThread,TRequestStatus* aStatus);
  protected:
    TUint iFlags; //0x04
    TPartitionInfo iPartitionInfo; //0x08
    const TMediaDrvDescData* iMountInfo; //0x54
    TUint32 iUnused; //0x58
  private:
    DThread* iReqThread[KMediaDrvMaxReqs]; //0x5c
    TRequestStatus* iReqStat[KMediaDrvMaxReqs]; //0x68
    TInt* iReqLenAddr[KMediaDrvMaxReqs]; //0x74
    TInt64 iTotalSizeInBytes; //0x80
};

class KK
{
  public:
    enum TKernPanic
    {
      EPanicUnknown0
    };
    enum TKernFault
    {
      EFaultUnknown0
    };
  public:
    static void Panic(TKernPanic aValue);
    static void Fault(TKernFault aValue);
};

class K61
{
  public:
    static void BeepComplete(TAny* aPtr,TInt aValue);
    static void Beep(TInt aValue,TTimeIntervalMicroSeconds32 anInterval);
};

#include <k32std61.inl>

#endif
