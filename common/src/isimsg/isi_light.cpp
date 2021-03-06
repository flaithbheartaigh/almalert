/*
    isi_light.cpp
    Copyright (C) 2005-2006 zg

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

#include <isi_light.hpp>
#include <isi_units.hpp>

EXPORT_C CLightBrightnessGetReq* CLightBrightnessGetReq::NewL(TUint8 aTransactionId,TDesC8& aData)
{
  CLightBrightnessGetReq* self=new(ELeave)CLightBrightnessGetReq;
  CleanupStack::PushL(self);
  self->ConstructL(16);
  self->SetUnit(KPhoneLightUnit);
  self->iPtr.Append(aTransactionId);
  self->iPtr.Append(1);
  self->iPtr.Append(0);
  self->iPtr.Append(0);
  if(aData.Length()!=4)
  {
    User::Leave(KErrGeneral);
  }
  else
  {
    self->iPtr.Append(aData);
  }
  CleanupStack::Pop(); //self
  return self;
}

EXPORT_C TInt CLightBrightnessGetResp::SubBlockCountIndex(void)
{
  return 11;
}

EXPORT_C TInt CLightBrightnessGetResp::SubBlockStart(void)
{
  return 12;
}

EXPORT_C CLightBrightnessSetReq* CLightBrightnessSetReq::NewL(TUint8 aTransactionId,CSubBlockArray* aSubBlocks)
{
  CLightBrightnessSetReq* self=new(ELeave)CLightBrightnessSetReq;
  CleanupStack::PushL(self);
  TInt len;
  if(aSubBlocks)
    len=aSubBlocks->DataSize()+12;
  else
    len=12;
  self->ConstructL(len);
  self->SetUnit(KPhoneLightUnit);
  self->iPtr.Append(aTransactionId);
  self->iPtr.Append(3);
  self->iPtr.Append(0);
  if(aSubBlocks)
  {
    self->iPtr.Append(aSubBlocks->Number());
    self->Append(aSubBlocks);
  }
  else
  {
    self->iPtr.Append(0);
  }
  CleanupStack::Pop(); //self
  return self;
}

EXPORT_C CLightBrightnessSetReq* CLightBrightnessSetReq::NewL(TUint8 aTransactionId,CSubBlock* aSubBlock)
{
  CLightBrightnessSetReq* self=new(ELeave)CLightBrightnessSetReq;
  CleanupStack::PushL(self);
  TInt len;
  if(aSubBlock)
    len=aSubBlock->Length()+12;
  else
    len=12;
  self->ConstructL(len);
  self->SetUnit(KPhoneLightUnit);
  self->iPtr.Append(aTransactionId);
  self->iPtr.Append(3);
  self->iPtr.Append(0);
  if(aSubBlock)
  {
    self->iPtr.Append(1);
    self->Append(aSubBlock);
  }
  else
  {
    self->iPtr.Append(0);
  }
  CleanupStack::Pop(); //self
  return self;
}


EXPORT_C CLightSwitchReq* CLightSwitchReq::NewL(TUint8 aTransactionId,TUint8 aType,TUint8 aState,CSubBlockArray* aSubBlocks)
{
  CLightSwitchReq* self=new(ELeave)CLightSwitchReq;
  CleanupStack::PushL(self);
  TInt len;
  if(aSubBlocks)
    len=aSubBlocks->DataSize()+16;
  else
    len=16;
  self->ConstructL(len);
  self->SetUnit(KPhoneLightUnit);
  self->iPtr.Append(aTransactionId);
  self->iPtr.Append(5);
  self->iPtr.Append(aType);
  self->iPtr.Append(aState);
  self->iPtr.Append(0);
  self->iPtr.Append(0);
  self->iPtr.Append(0);
  if(aSubBlocks)
  {
    self->iPtr.Append(aSubBlocks->Number());
    self->Append(aSubBlocks);
  }
  else
  {
    self->iPtr.Append(0);
  }
  CleanupStack::Pop(); //self
  return self;
}

EXPORT_C CLightSwitchReq* CLightSwitchReq::NewL(TUint8 aTransactionId,TUint8 aType,TUint8 aState,CSubBlock* aSubBlock)
{
  CLightSwitchReq* self=new(ELeave)CLightSwitchReq;
  CleanupStack::PushL(self);
  TInt len;
  if(aSubBlock)
    len=aSubBlock->Length()+16;
  else
    len=16;
  self->ConstructL(len);
  self->SetUnit(KPhoneLightUnit);
  self->iPtr.Append(aTransactionId);
  self->iPtr.Append(5);
  self->iPtr.Append(aType);
  self->iPtr.Append(aState);
  self->iPtr.Append(0);
  self->iPtr.Append(0);
  self->iPtr.Append(0);
  if(aSubBlock)
  {
    self->iPtr.Append(1);
    self->Append(aSubBlock);
  }
  else
  {
    self->iPtr.Append(0);
  }
  CleanupStack::Pop(); //self
  return self;
}

EXPORT_C TUint8 CLightBrightnessInfo::Brightness1(void)
{
  return iPtr[2];
}

EXPORT_C TUint8 CLightBrightnessInfo::Brightness2(void)
{
  return iPtr[3];
}
