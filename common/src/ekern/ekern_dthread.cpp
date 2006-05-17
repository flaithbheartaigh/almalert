/*
    ekern_dthread.cpp
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

class TProcessMemoryInfo; //don't exists in symbian 6.1
#include <k32std.h>
#include <e32std.h>

TMessageBase::TMessageBase() //FIXME:!!!
{
}

TMessageSlot::TMessageSlot(TMessagePool* aPool) //FIXME:!!!
{
}

DThread::DThread() //FIXME: NOT IMPLEMENTED
{
}

DThread::~DThread() //FIXME: NOT IMPLEMENTED
{
}

void DThread::CloseCreatedHeap(void) //FIXME: NOT IMPLEMENTED
{
}

void DThread::DoClose(void) //FIXME: NOT IMPLEMENTED
{
}

void DThread::Create(CObject* anOwner,DProcess* anOwningProcess,TThreadCreateType aType,const TDesC& aName,TThreadFunction aFunction,TInt aStackSize,RHeap* aHeap,TInt aHeapMinSize,TInt aHeapMaxSize,TAny* aPtr,TOwnerType anOwnerType)
{
}

void SendReceiveFault(TInt aFunction,TAny* aPtr) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void Kern::SvSendReceive(TInt aFunction,TAny* aPtr,TRequestStatus& aStatus) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C DThread* Kern::ThreadFromHandle(TInt aHandle)
{
  return (DThread*)K::ObjectFromHandle(aHandle,NULL,K::Threads);
}

EXPORT_C DThread* Kern::ThreadFromId(TThreadId aId)
{
  for(TInt i=0;i<K::Threads->Count();i++)
  {
    DThread* thread=static_cast<DThread*>((*K::Threads)[i]);
    if(thread->iId==aId) return thread;
  }
  return NULL;
}

DThread& S::ThreadFromHandleL(TInt aHandle)
{
  return *(DThread*)S::ObjectFromHandleL(aHandle,K::SvThread,K::Threads);
}

DThread* S::ThreadFromHandle(TInt aHandle) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
  //return (DThread*)K::ObjectFromHandleNull(aHandle,NULL,K::Threads);
}

void S::ThreadCreateL(TInt& aHandle,DProcess* anOwningProcess,DThread* anOwningThread,TInt aLibraryHandle,const TDesC& aName,TThreadFunction aFunction,TInt aStackSize,RHeap* aHeap,TInt aHeapMinSize,TInt aHeapMaxSize,TAny* aPtr,DThread*& aThread,TOwnerType aType) //FIXME: NOT IMPLEMENTED
{
}

TInt S::ThreadCreate(TInt& aHandle,DProcess* anOwningProcess,DThread* anOwningThread,TInt aLibraryHandle,const TDesC& aName,TThreadFunction aFunction,TInt aStackSize,RHeap* aHeap,TInt aHeapMinSize,TInt aHeapMaxSize,TAny* aPtr,TOwnerType aType)
{
  DThread* thread=NULL;
  TRAPD(err,ThreadCreateL(aHandle,anOwningProcess,anOwningThread,aLibraryHandle,aName,aFunction,aStackSize,aHeap,aHeapMinSize,aHeapMaxSize,aPtr,thread,aType));
  if(err!=KErrNone) CheckedClose(thread);
  return err;
}

TInt S::ThreadOpen(TInt& aHandle,DProcess* anOwningProcess,DThread* anOwningThread,const TDesC& aFullName,TOwnerType aType)
{
  TFullName name;
  TInt handle=0;
  TInt err=K::Threads->FindByFullName(handle,aFullName,name);
  if(err==KErrNone)
  {
    err=S::OpenFindHandle(aHandle,anOwningProcess,anOwningThread,handle,aType);
  }
  return err;
}

TInt S::ThreadOpen(TInt& aHandle,DProcess* anOwningProcess,DThread* anOwningThread,TThreadId aId,TOwnerType aType)
{
  DThread* thread=Kern::ThreadFromId(aId);
  if(!thread) return KErrNotFound;
  TInt err=thread->Open();
  if(err==KErrNone)
  {
    if(aType==EOwnerThread)
    {
      TRAP(err,aHandle=MakeHandle(thread,anOwningThread));
    }
    else
    {
      TRAP(err,aHandle=MakeHandle(thread,anOwningProcess));
    }
    if(err!=KErrNone)
    {
      thread->Close();
    }
  }
  return err;
}

EXPORT_C TInt Kern::ThreadFind(TInt &aFindHandle,const TDesC &aMatch,TFullName &aName)
{
  return K::Threads->FindByFullName(aFindHandle,aMatch,aName);
}

EXPORT_C TInt DThread::Rename(const TDesC &aName) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TThreadPriority DThread::Priority() const
{
  return iPriority;
}

EXPORT_C void DThread::SetPriority(TThreadPriority aPriority) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C TBool DThread::System() const
{
  return iFlags&4;
}

EXPORT_C void DThread::SetSystem(TBool aState)
{
  iFlags&=~4;
  if(aState) iFlags|=4;
}

EXPORT_C TBool DThread::Protected() const
{
  return iFlags&8;
}

EXPORT_C void DThread::SetProtected(TBool aState)
{
  iFlags&=~8;
  if(aState) iFlags|=8;
}

EXPORT_C TInt DThread::RequestCount() const
{
  return iRequestSemaphore->Count();
}

EXPORT_C TInt DThread::GetDesInfo(const TAny *aDes,TInt &aLength,TInt &aMaxLength,TUint8 * &aPtr,TBool aWriteable) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt DThread::GetDesLength(const TAny *aPtr) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt DThread::GetDesMaxLength(const TAny *aPtr) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt DThread::Read(const TAny * aPtr,TDes8 &aDes) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt DThread::Read(const TAny * aPtr,TDes16 &aDes) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt DThread::Read(const TAny * aPtr,TDes8 * aDes,TInt anOffset,TInt aMode) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt DThread::Write(const TAny * aPtr,const TDesC8 &aDes) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt DThread::Write(const TAny * aPtr,const TDesC16 &aDes) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt DThread::Write(const TAny * aPtr,const TDesC8 * aDes,TInt anOffset,TInt aMode) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt DThread::WriteNoCheck(const TAny * aPtr,const TDesC8 * aDes,TInt anOffset,TInt aMode) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C void DThread::Logon(TRequestStatus *aStatus,DThread *anOwningThread) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C TInt DThread::LogonCancel(TRequestStatus *aStatus,DThread *anOwningThread) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C void DThread::After(TTimeIntervalMicroSeconds32 aTime,TRequestStatus &aStatus)
{
  iTimer->After(aStatus,aTime);
}

EXPORT_C void DThread::At(const TTime &aTime,TRequestStatus &aStatus)
{
  iTimer->At(aStatus,aTime);
}

EXPORT_C TInt DThread::SetTls(TInt aHandle,TAny *aPtr) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TAny* DThread::Tls(TInt aHandle) //FIXME: NOT IMPLEMENTED
{
  return NULL;
}

EXPORT_C void DThread::FreeTls(TInt aHandle) //FIXME: NOT IMPLEMENTED
{
}

TBool IsInList(TUint aValue,TUint* aList,TInt aCount)
{
  for(TInt i=0;i<aCount;i++)
  {
    if(aList[i]==aValue) return ETrue;
  }
  return EFalse;
}

TInt TraverseDllRefTable(TUint,TInt,TInt&,TUint*,TUint*&,TDllRefTable*) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

TInt DThread::StaticCallList(TInt& aCount,TUint*& aCallList) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

TInt DThread::DynamicCallList(DLibrary* aLibrary,TInt& aCount,TUint*& aCallList) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

TInt DThread::DisjointCallList(DLibrary* aLibrary,TInt& aCount,TUint*& aCallList) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

void DThread::AddToCleanup(DCleanup &aCleanup)
{
  iCleanupQ.AddLast(aCleanup);
}

void DThread::RemoveFromCleanup(CObject *anObject) //FIXME: NOT IMPLEMENTED
{
}

void DThread::DoDie(void) //FIXME: NOT IMPLEMENTED
{
}

void DThread::Cleanup(void) //FIXME: NOT IMPLEMENTED
{
}

TBool DThread::ProtectThreadP(void) //FIXME: NOT IMPLEMENTED
{
  return ETrue;
}

void DThread::ProtectThreadL(void) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C TExceptionHandler* DThread::ExceptionHandler(void)
{
  return iExceptionHandler;
}

EXPORT_C TInt DThread::SetExceptionHandler(TExceptionHandler *aHandler,TUint aMask)
{
  iExceptionHandler=aHandler;
  iExceptionMask=aMask;
  return KErrNone;
}

EXPORT_C void DThread::ModifyExceptionMask(TUint aClearMask, TUint aSetMask)
{
  iExceptionMask=aSetMask|(iExceptionMask&~aClearMask);
}

EXPORT_C TInt DThread::RaiseException(TExcType aType) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

TBool DThread::IsExceptionHandled(TExcType aType) //FIXME: NOT IMPLEMENTED
{
  return ETrue;
}

TInt DThread::GetRamSizes(TInt& aHeapSize,TInt& aStackSize) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

void DThread::AddThread(DThread& aThread) //FIXME: NOT IMPLEMENTED
{
}

void DThread::MakeThisMainThread(void) //FIXME: NOT IMPLEMENTED
{
}
