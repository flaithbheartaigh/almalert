/*
    zg_vibra.cpp
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

#include "zg_vibra.hpp"

CVibraReq* CVibraReq::NewL(TUint8 aParam,TUint8 aState) //checked
{
  CVibraReq* self=new(ELeave)CVibraReq;
  CleanupStack::PushL(self);
  self->ConstructL(11);
  TUint8 var=0x1c;
  self->iPtr[3]=var;
  self->iPtr.Append(aParam);
  self->iPtr.Append(0xc);
  self->iPtr.Append(aState);
  CleanupStack::Pop();
  return self;
}

void CVibraReq::ConstructL(TInt aSize) //checked
{
  CPnMsg::ConstructL(aSize-6);
  iPtr.SetLength(8);
  TUint8 var=0;
  iPtr[1]=var;
  iPtr[6]=var;
}

TInt CVibraReq::SubBlockCountIndex(void) //checked
{
  return 0;
}

TInt CVibraReq::SubBlockStart(void) //checked
{
  return 0;
}

CPnMsg::~CPnMsg() //checked
{
  if(iBuf)
  {
    delete iBuf;
    iBuf=NULL;
  }
}
