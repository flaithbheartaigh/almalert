/*
    isi_light.cpp
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

#include <isi_light.hpp>

EXPORT_C CLightBrightnessGetReq* CLightBrightnessGetReq::NewL(TUint8 aParam,TDesC8& aData)
{
  CLightBrightnessGetReq* self=new(ELeave)CLightBrightnessGetReq;
  CleanupStack::PushL(self);
  self->ConstructL(16);
  TUint8 param=0x3a;
  self->iPtr[3]=param;
  self->iPtr.Append(aParam);
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

EXPORT_C CLightBrightnessSetReq* CLightBrightnessSetReq::NewL(TUint8 aParam,CSubBlock* aSubBlock)
{
  CLightBrightnessSetReq* self=new(ELeave)CLightBrightnessSetReq;
  CleanupStack::PushL(self);
  TInt len;
  if(aSubBlock)
    len=aSubBlock->Length()+12;
  else
    len=12;
  self->ConstructL(len);
  TUint8 param=0x3a;
  self->iPtr[3]=param;
  self->iPtr.Append(aParam);
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

EXPORT_C TUint8 CLightBrightnessInfo::Brightness1(void)
{
  return iPtr[2];
}

EXPORT_C TUint8 CLightBrightnessInfo::Brightness2(void)
{
  return iPtr[3];
}
