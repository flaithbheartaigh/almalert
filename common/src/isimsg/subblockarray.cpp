/*
    subblockarray.cpp
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

EXPORT_C CSubBlockArray::~CSubBlockArray()
{
  for(TInt i=0;i<iAmount;i++)
    if(iSubBlocks[i]) delete iSubBlocks[i];
  delete[] iSubBlocks;
}

EXPORT_C CSubBlockArray* CSubBlockArray::NewL(TUint8 aNumber)
{
  CSubBlockArray* self=new(ELeave)CSubBlockArray;
  CleanupStack::PushL(self);
  self->ConstructL(aNumber);
  CleanupStack::Pop(); //self
  return self;
}

EXPORT_C CSubBlock*& CSubBlockArray::operator[](TUint8 aIndex)
{
  return iSubBlocks[aIndex];
}

void CSubBlockArray::ConstructL(TUint8 aAmount)
{
  iAmount=aAmount;
  iSubBlocks=new(ELeave)CSubBlock*[aAmount];
  for(TInt i=0;i<aAmount;i++) iSubBlocks[i]=NULL;
}

EXPORT_C TUint8 CSubBlockArray::Number(void)
{
  return iAmount;
}

EXPORT_C TInt CSubBlockArray::DataSize(void)
{
  TInt size=0;
  for(TInt i=0;i<iAmount;i++) size+=iSubBlocks[i]->Length();
  return size;
}
