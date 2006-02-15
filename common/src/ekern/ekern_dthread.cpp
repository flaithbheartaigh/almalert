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

EXPORT_C TExceptionHandler* DThread::ExceptionHandler()
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
