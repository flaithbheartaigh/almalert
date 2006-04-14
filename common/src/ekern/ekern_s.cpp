/*
    ekern_s.cpp
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

CObject* S::ObjectFromHandleL(TInt aHandle) //FIXME: NOT IMPLEMENTED
{
  return NULL;
}

CObject* S::ObjectFromHandleL(TInt aHandle,DThread* aThread) //FIXME: NOT IMPLEMENTED
{
  return NULL;
}

CObject* S::ObjectFromHandleL(TInt aHandle,DThread* aThread,CObjectCon* aCon) //FIXME: NOT IMPLEMENTED
{
  return NULL;
}

TInt S::OpenObject(TInt& aHandle,CObject*& anObject,DProcess* anOwningProcess,CObjectCon* aCon,const TDesC& aName) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

TInt S::OpenObject(TInt& aHandle,DProcess* anOwningProcess,CObjectCon* aCon,const TDesC& aName) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

TInt S::OpenObject(TInt& aHandle,CObject*& anObject,DThread* anOwningThread,CObjectCon* aCon,const TDesC& aName) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

TInt S::OpenObject(TInt& aHandle,DThread* anOwningThread,CObjectCon* aCon,const TDesC& aName) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

TInt S::OpenFindHandle(TInt& aHandle,DProcess* anOwningProcess,DThread* anOwningThread,TInt aFindHandle,TOwnerType aType) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

TInt S::MakeHandleAndOpen(CObject* anObj,DProcess* anOwningProcess) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

TInt S::MakeHandleAndOpen(CObject* anObj,DThread* anOwningThread) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

TInt S::MakeHandle(CObject* anObj,DProcess* anOwningProcess)
{
  if(anOwningProcess==NULL) anOwningProcess=K::SvProcess;
  return anOwningProcess->Handles().AddL(anObj);
}

TInt S::MakeHandle(CObject* anObj,DThread* anOwningThread)
{
  if(anOwningThread==NULL) anOwningThread=K::SvThread;
  return anOwningThread->Handles().AddL(anObj)|0x40000000;
}

TInt S::DuplicateHandle(DThread* aSrc,TInt aHandle,DThread* aTrg,TInt& aNewHandle,TOwnerType aType) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

void S::Close(TInt aHandle,DProcess* anOwningProcess,DThread* anOwningThread) //FIXME: NOT IMPLEMENTED
{
}

void S::CheckedClose(CObject* anObj)
{
  if(anObj) anObj->Close();
}

void S::CheckedClose(CObject* anObj,CObject* anOwner)
{
  if(anObj)
  {
    if(anObj->Owner()==anOwner) anObj->SetOwner(NULL);
    anObj->Close();
  }
}

void S::OrphanAndDeleteHandles(CObjectIx*& anIndex,CObject* anOwner)
{
  if(anIndex)
  {
    for(int i=anIndex->Count()-1;i>=0;i--)
    {
      CObject* obj=(*anIndex)[i];
      if(obj&&obj->Owner()==anOwner)
      {
        obj->SetOwner(NULL);
      }
    }
    delete anIndex;
    anIndex=NULL;
  }
}
