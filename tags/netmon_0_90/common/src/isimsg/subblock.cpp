/*
    subblock.cpp
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
#include <isi_units.hpp>

EXPORT_C CSubBlock::~CSubBlock()
{
  delete iBuf;
}

EXPORT_C CSubBlock::CSubBlock(): CBase(),iPtr(0,0),iBuf(NULL)
{
}

EXPORT_C void CSubBlock::Move(CSubBlock* aTarget)
{
  aTarget->iBuf=iBuf;
  iBuf=NULL;
  aTarget->iPtr.Set(iPtr);
  aTarget->iUnknown=iPtr[0];
}

void CSubBlock::ConstructL(TInt aSize)
{
  iBuf=HBufC8::NewL(aSize);
  iPtr.Set(iBuf->Des());
}

inline TUint16 Combine(TUint16 aParam1,TUint8 aParam2)
{
  return aParam1|aParam2;
}

EXPORT_C CSubBlock* CSubBlock::NewL(TDesC8& aData,TInt anOffset,TUint8 aUnit)
{
  TInt len;
  if(aUnit==KPhonePhoneBookUnit)
  {
    len=Combine(aData[anOffset+2]<<8,aData[anOffset+3]);
  }
  else
  {
    if(aData[anOffset]==4&&aUnit==KPhoneEpocTestUnit&&aData[anOffset+4]==0x10)
    {
      len=Combine(aData[anOffset+1]<<8,aData[anOffset+2]);
    }
    else
    {
      len=aData[anOffset+1];
    }
  }
  if((anOffset+len)>aData.Length()) User::Leave(KErrGeneral);
  TPtrC8 subBlock=aData.Mid(anOffset,len);
  CSubBlock* self=new(ELeave)CSubBlock;
  CleanupStack::PushL(self);
  self->ConstructL(subBlock.Length());
  self->iUnknown=aData[anOffset];
  self->iPtr=subBlock;
  CleanupStack::Pop(); //self
  return self;
}

EXPORT_C CSubBlock* CSubBlock::NewL1(TDesC8& aData,TInt anOffset)
{
  TPtrC8 subBlock=aData.Mid(anOffset,aData.Length()-anOffset);
  CSubBlock* self=new(ELeave)CSubBlock;
  CleanupStack::PushL(self);
  self->ConstructL(subBlock.Length());
  self->iUnknown=aData[anOffset];
  self->iPtr=subBlock;
  CleanupStack::Pop(); //self
  return self;
}

EXPORT_C TInt CSubBlock::SubBlockCountIndex()
{
  return 0;
}

EXPORT_C TInt CSubBlock::SubBlockStart()
{
  return 0;
}

EXPORT_C TPtrC8 CSubBlock::Data(void)
{
  return TPtrC8(iPtr);
}

EXPORT_C TInt CSubBlock::Length(void)
{
  return iPtr.Length();
}

EXPORT_C CSubBlock* CSubBlock::NewL0(TDesC8& aData,TInt anOffset)
{
  TUint8 length=aData[anOffset+2];
  if((anOffset+length)>aData.Length()) User::Leave(KErrGeneral);
  TPtrC8 ptr=aData.Mid(anOffset,length);
  CSubBlock* self=new(ELeave)CSubBlock;
  CleanupStack::PushL(self);
  self->ConstructL(ptr.Length());
  self->iUnknown=aData[anOffset];
  self->iPtr=ptr;
  CleanupStack::Pop(); //self
  return self;
}

EXPORT_C TUint16 CSubBlock::Address(void) //149
{
  return Combine(iPtr[0]<<8,iPtr[1]);
}
