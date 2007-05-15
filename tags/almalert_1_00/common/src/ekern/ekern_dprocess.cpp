/*
    ekern_dprocess.cpp
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

DProcess::DProcess(): iCleanupQ(8) //FIXME: NOT IMPLEMENTED
{
}

DProcess::~DProcess() //FIXME: NOT IMPLEMENTED
{
}

TName DProcess::Name(void) const //FIXME: NOT IMPLEMENTED
{
  return TName();
}

void DProcess::DoClose(void) //FIXME: NOT IMPLEMENTED
{
}

void DProcess::Create(TBool aFirstProcess,TLoaderInfo& anInfo,HBufC* aCommand) //FIXME: NOT IMPLEMENTED
{
}


EXPORT_C DProcess* Kern::ProcessFromHandle(TInt aHandle) //FIXME: NOT IMPLEMENTED
{
  return NULL;
}

DProcess& S::ProcessFromHandleL(TInt aHandle,DThread *aThread) //FIXME: NOT IMPLEMENTED
{
  return *(DProcess*)0;
}

DProcess* S::ProcessFromHandle(TInt aHandle,DThread *aThread) //FIXME: NOT IMPLEMENTED
{
  return NULL;
}

void S::ProcessCreateL(DProcess* anOwningProcess,DThread* anOwningThread,TLoaderInfo& anInfo,HBufC* aCommand,DProcess*& aProcess,TOwnerType aType) //FIXME: NOT IMPLEMENTED
{
}

TInt S::ProcessCreate(DProcess* anOwningProcess,DThread* anOwningThread,TLoaderInfo& anInfo,HBufC* aCommand,TOwnerType aType) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

TInt S::ProcessOpen(TInt& aHandle,DProcess* anOwningProcess,DThread* anOwningThread,const TDesC& aName,TOwnerType aType) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

TInt S::ProcessOpen(TInt& aHandle,DProcess* anOwningProcess,DThread* anOwningThread,TProcessId aId,TOwnerType aType) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Kern::ProcessFind(TInt &aFindHandle,const TDesC &aMatch,TFullName &aName) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt DProcess::Rename(const TDesC &aName) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C void DProcess::FileName(TDes& aFileName) const
{
  aFileName.Copy(*iFileName);
}

EXPORT_C void DProcess::CommandLine(TDes &aCommand) const //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C TProcessPriority DProcess::Priority() const
{
  return iPriority;
}

EXPORT_C void DProcess::SetPriority(TProcessPriority aPriority) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C TBool DProcess::System() const
{
  return iFlags&4;
}

EXPORT_C void DProcess::SetSystem(TBool aState)
{
  iFlags&=~4;
  if(aState) iFlags|=4;
}

EXPORT_C TBool DProcess::Protected() const
{
  return iFlags&8;
}

EXPORT_C void DProcess::SetProtected(TBool aState)
{
  iFlags&=~8;
  if(aState) iFlags|=8;
}

EXPORT_C TInt DProcess::MakeHandleToOwningProcess(TInt& aHandle,DProcess* anOwningProcess) const //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C void DProcess::SetOwningProcess(TInt aHandle)
{
  if(!aHandle) iOwningProcess=NULL;
  else iOwningProcess=Kern::ProcessFromHandle(aHandle);
}

void DProcess::Cleanup(void) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void DProcess::Logon(TRequestStatus* aStatus,DThread* anOwningThread) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C TInt DProcess::LogonCancel(TRequestStatus* aStatus,DThread* anOwningThread) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

TBool DProcess::ProtectProcessP(void) //FIXME: NOT IMPLEMENTED
{
  return EFalse;
}

void DProcess::ProtectProcessL(void) //FIXME: NOT IMPLEMENTED
{
}

DLogonProcess::DLogonProcess(TRequestStatus* aStatus,DThread* aThread,DProcess* aLoggedProcess): DCleanup(ELogon),iStatus(aStatus),iThread(aThread),iLoggedProcess(aLoggedProcess)
{
}

DLogonProcess::~DLogonProcess()
{
  if(iStatus)
  {
    iThread->RequestComplete(iStatus,iLoggedProcess->iExitReason);
  }
}

TBool DLogonProcess::Cancel(TRequestStatus* aStatus,DThread* aThread)
{
  if(aThread==iThread&&(aStatus==NULL||aStatus==iStatus))
  {
    iLink.Deque();
    delete this;
    return ETrue;
  }
  return EFalse;
}
