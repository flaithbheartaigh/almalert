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

#include <isimsg.hpp>

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

EXPORT_C void CIsiMsg::Move(CIsiMsg* aTarget)
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

EXPORT_C CSubBlock* CIsiMsg::SubBlock(void)
{
  TInt index=SubBlockCountIndex();
  if(index==0||(index>0&&!iPtr[index])) return NULL;
  TInt offset=SubBlockStart();
  return CSubBlock::NewL(iPtr,offset,Unit());
}

EXPORT_C void CIsiMsg::Append(CSubBlockArray* aBlocks)
{
  TUint8 number=aBlocks->Number();
  for(TUint8 i=0;i<number;i++)
  {
    Append((*aBlocks)[i]);
  }
}

EXPORT_C void CIsiMsg::Append(CSubBlock* aBlock)
{
  iPtr.Append(aBlock->Data());
}

void CIsiMsg::ConstructL(TInt aSize)
{
  CPnMsg::ConstructL(aSize-6);
  iPtr.SetLength(8);
  TUint8 var=0;
  iPtr[1]=var;
  iPtr[6]=var;
}

EXPORT_C CSubBlockArray* CIsiMsg::SubBlocksL(void)
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
