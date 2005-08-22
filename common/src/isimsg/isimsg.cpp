/*
    isimsg.cpp
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

#include "isimsg.hpp"

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
{
  return KErrNone;
}

EXPORT_C CIsiMsg::~CIsiMsg()
{
}

EXPORT_C CIsiMsg* CIsiMsg::NewL(TDesC8& aData)
{
  CIsiMsg* self=new(ELeave)CIsiMsg;
  CleanupStack::PushL(self);
  self->iBuf=aData.AllocL();
  self->iPtr.Set(self->iBuf->Des());
  CleanupStack::Pop(); //self
  return self;
}

EXPORT_C CIsiMsg* CIsiMsg::NewL(TInt aSize)
{
  CIsiMsg* self=new(ELeave)CIsiMsg;
  CleanupStack::PushL(self);
  self->ConstructL(aSize);
  CleanupStack::Pop(); //self
  return self;
}

EXPORT_C void CIsiMsg::Transfer(CIsiMsg* aTarget)
{
  aTarget->iBuf=iBuf;
  iBuf=NULL;
  aTarget->iPtr.Set(iPtr);
}

EXPORT_C TInt CIsiMsg::SubBlockCountIndex(void)
{
  return 0;
}

EXPORT_C TInt CIsiMsg::SubBlockStart(void)
{
  return 0;
}

EXPORT_C TUint8 CIsiMsg::SubBlockCount(void)
{
  TInt index=SubBlockCountIndex();
  if(index>0)
  {
    return iPtr[index];
  }
  else if(index==-1)
  {
    return 1;
  }
  return 0;
}

void CIsiMsg::ConstructL(TInt aSize)
{
  CPnMsg::ConstructL(aSize-6);
  iPtr.SetLength(8);
  TUint8 var=0;
  iPtr[1]=var;
  iPtr[6]=var;
}

EXPORT_C TUint8 CIsiMsg976::GetParam10(void)
{
  return iPtr[10];
}

EXPORT_C TUint8 CIsiMsg976::GetParam10A(void)
{
  return iPtr[10];
}

EXPORT_C TUint8 CIsiMsg976::GetParam10B(void)
{
  return iPtr[10];
}

EXPORT_C TUint8 CIsiMsg976::GetParam10C(void)
{
  return iPtr[10];
}

EXPORT_C CIsiMsg802* CIsiMsg802::NewL(TUint8 aParam1,TUint8 aParam2)
{
  return NULL;
}

EXPORT_C CIsiMsg848* CIsiMsg848::NewL(TUint8 aParam1,TUint8 aParam2)
{
  return NULL;
}

EXPORT_C CIsiMsg850* CIsiMsg850::NewL(TUint8 aParam1,TUint8 aParam2,TUint8 aParam3,TUint8 aParam4)
{
  return NULL;
}

EXPORT_C CIsiMsg649* CIsiMsg649::NewL(TUint8 aParam1,TUint8 aParam2,TUint8 aParam3)
{
  return NULL;
}

EXPORT_C CIsiMsg691* CIsiMsg691::NewL(TUint8 aParam1,TUint8 aParam2,TUint8 aParam3,CIsiMsg649* aMsg)
{
  return NULL;
}

EXPORT_C CIsiMsg984* CIsiMsg984::NewL(TDesC8& aData,TUint anOffset,TUint8 aParam)
{
  return NULL;
}

EXPORT_C CIsiMsg928* CIsiMsg928::NewL(TUint8 aParam,CIsiMsg984* aMsg)
{
  return NULL;
}

EXPORT_C CIsiMsg902* CIsiMsg902::NewL(TUint8 aParam,TDesC8& aData)
{
  return NULL;
}
