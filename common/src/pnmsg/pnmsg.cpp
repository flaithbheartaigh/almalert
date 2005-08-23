/*
    pnmsg.cpp
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

#include <pnmsg.hpp>

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
{
  return KErrNone;
}

void CPnMsg::Panic(TPnMsgPanic aPanic)
{
  _LIT(KPnMsgPanic,"PnMsg Panic");
  User::Panic(KPnMsgPanic,aPanic);
}

EXPORT_C TInt CPnMsg::ConstructL(TInt aSize)
{
  if(aSize<0) Panic(EPanicPnMsgUnderflow);
  iBuf=HBufC8::NewL(aSize+6);
  if(!iBuf) Panic(EPanicPnMsgNoMemory);
  iPtr.Set(iBuf->Des());
  iPtr.FillZ(6);
  iPtr[0]=0;
  iPtr[5]=aSize>>8;
  iPtr[4]=aSize;
  if(aSize>4) iPtr.SetLength(10);
  else if(aSize>2) iPtr.SetLength(8);
  return 0;
}

EXPORT_C CPnMsg* CPnMsg::NewL(TInt aSize)
{
  CPnMsg* self=new(ELeave)CPnMsg;
  CleanupStack::PushL(self);
  self->ConstructL(aSize);
  self->iPtr.SetLength(aSize+6);
  CleanupStack::Pop(); //self
  return self;
}

EXPORT_C CPnMsg* CPnMsg::NewL(TUint8 aParam1,TDesC8 const& aStr)
{
  TInt len=aStr.Length()+2;
  CPnMsg* self=new(ELeave)CPnMsg;
  CleanupStack::PushL(self);
  self->ConstructL(len);
  self->iPtr.SetLength(6);
  self->iPtr[3]=aParam1;
  self->iPtr.Append(0);
  self->iPtr.Append(0);
  self->iPtr.Append(aStr);
  CleanupStack::Pop(); //self
  return self;
}

EXPORT_C CPnMsg* CPnMsg::NewL(TUint8 aParam1,TUint8 aParam2,TDesC8 const& aStr)
{
  TInt len=aStr.Length()+2;
  CPnMsg* self=new(ELeave)CPnMsg;
  CleanupStack::PushL(self);
  self->ConstructL(len);
  self->iPtr.SetLength(6);
  self->iPtr[1]=aParam1;
  self->iPtr.Append(aParam2);
  self->iPtr.Append(0);
  self->iPtr.Append(aStr);
  CleanupStack::Pop(); //self
  return self;
}

inline TUint16 Combine(TUint16 aParam1,TUint8 aParam2)
{
  return aParam1|aParam2;
}

EXPORT_C CPnMsg* CPnMsg::NewL(CPnMsg &aMsg)
{
  TInt len;
  len=Combine(aMsg.iPtr[5]<<8,aMsg.iPtr[4]);
  CPnMsg* self=new(ELeave)CPnMsg;
  CleanupStack::PushL(self);
  self->ConstructL(len);
  *self=aMsg;
  CleanupStack::Pop(); //self
  return self;
}

EXPORT_C CPnMsg* CPnMsg::NewL(TDes8 const& aStr)
{
  TInt len=aStr.Length()-6;
  CPnMsg* self=new(ELeave)CPnMsg;
  CleanupStack::PushL(self);
  self->ConstructL(len);
  self->iPtr.SetLength(aStr.Length());
  self->iPtr=aStr;
  CleanupStack::Pop(); //self
  return self;
}

EXPORT_C CPnMsg& CPnMsg::operator=(CPnMsg &aMsg)
{
  TInt len=aMsg.iPtr.Length();
  iPtr.SetLength(len);
  TUint8* src=&aMsg.iPtr[0];
  TUint8* srcEnd=src+len;
  TUint8* dest=&iPtr[0];
  while(src<srcEnd)
  {
    *dest++=*src++;
  }
  return *this;
}
