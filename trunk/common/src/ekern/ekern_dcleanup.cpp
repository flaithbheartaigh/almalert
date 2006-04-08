/*
    ekern_dcleanup.cpp
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

DCleanup::DCleanup(TCleanupType aType): CBase(),iType(aType)
{
}

void DCleanupObject::NewL(DThread& aThread,CObject* anObject)
{
  DCleanupObject* self=new(ELeave)DCleanupObject(anObject);
  aThread.AddToCleanup(*self);
}

DCleanupObject::DCleanupObject(CObject* anObject): DCleanup(EObject),iObject(anObject)
{
}

DCleanupObject::~DCleanupObject()
{
  S::CheckedClose(iObject);
}

TBool DCleanupObject::Remove(CObject* anObject)
{
  if(iObject==anObject)
  {
    iLink.Deque();
    iObject=NULL;
    delete this;
    return ETrue;
  }
  return EFalse;
}

DLogon::DLogon(TRequestStatus* aStatus,DThread* aThread,DThread* aLoggedThread): DCleanup(ELogon),iStatus(aStatus),iThread(aThread),iLoggedThread(aLoggedThread)
{
}

DLogon::~DLogon()
{
  if(iStatus) iThread->RequestComplete(iStatus,iLoggedThread->iExitReason);
}

TBool DLogon::Cancel(TRequestStatus* aStatus,DThread* aThread)
{
  if(aThread==iThread&&(aStatus==NULL||aStatus==iStatus))
  {
    iLink.Deque();
    delete this;
    return ETrue;
  }
  return EFalse;
}
