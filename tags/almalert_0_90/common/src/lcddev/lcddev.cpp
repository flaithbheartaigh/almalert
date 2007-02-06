/*
    lcddev.cpp
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

#include <lcddev.hpp>

_LIT(KRDevLcd,"Lcd");

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
{
  return KErrNone;
}

EXPORT_C RLcdDev* RLcdDev::NewL(void)
{
  return new RLcdDev;
}

RLcdDev::~RLcdDev()
{
  if(iLoaded) Unload();
}

void RLcdDev::Unload(void)
{
  if(iLoaded)
  {
    Close();
    User::FreeLogicalDevice(KRDevLcd);
    iLoaded=EFalse;
  }
}

TInt RLcdDev::Load(TBool aFull)
{
  if(iLoaded) return KErrAlreadyExists;
  TInt err;
  if(aFull)
  {
    err=User::LoadLogicalDevice(KRDevLcd);
    if(err!=KErrNone&&err!=KErrAlreadyExists) return err;
  }
  err=Open(VersionRequired());
  if(err==KErrNone) iLoaded=ETrue;
  return err;
}

TBool RLcdDev::IsLoaded(void)
{
  return iLoaded;
}

TInt RLcdDev::Func1(TInt aParam1,TInt aParam2,TInt aParam3)
{
  TInt data[3]={aParam1,aParam2,aParam3};
  return DoControl(0x1001,data);
}

TInt RLcdDev::Func2(TInt aParam)
{
  TInt data=aParam;
  TInt result=-1;
  DoControl(0x1002,&data,&result);
  return result;
}

TInt RLcdDev::Func3(void)
{
  TInt result;
  DoControl(0x1003,&result);
  return result;
}

TInt RLcdDev::Func4(void)
{
  return DoControl(0x1004);
}

TInt RLcdDev::Func5(TInt aParam)
{
  TInt data=aParam;
  return DoControl(0x1005,&data);
}

TInt RLcdDev::Func6(TAny* aData)
{
  return DoControl(0x1006,aData);
}

TInt RLcdDev::Func7(TAny* aData)
{
  return DoControl(0x1007,aData);
}

TInt RLcdDev::Func8(TAny* aData)
{
  return DoControl(0x1008,aData);
}

TInt RLcdDev::Func9(TInt aParam1,TInt aParam2,TInt& aParam3)
{
  TInt data[3];
  TInt tmp;
  data[0]=aParam1;
  data[1]=aParam2;
  data[2]=(TInt)&tmp;
  TInt err=DoControl(0x1009,data);
  if(err==KErrNone) aParam3=tmp;
  return err;
}

TInt RLcdDev::FuncA(TInt aParam1,TInt aParam2)
{
  TInt data[3]={aParam1,1,aParam2};
  return DoControl(0x100a,data);
}

TInt RLcdDev::FuncB(TInt aParam1,TInt aParam2,TInt aParam3)
{
  TInt data[3];
  TInt tmp=aParam3;
  data[0]=aParam1;
  data[1]=aParam2;
  data[2]=(TInt)&tmp;
  return DoControl(0x100b,data);
}

TInt RLcdDev::FuncC(TInt aParam1,TInt aParam2)
{
  TInt data[3]={aParam1,1,aParam2};
  return DoControl(0x100c,data);
}

TInt RLcdDev::FuncD(void)
{
  TInt data=0;
  DoControl(0x100d,&data);
  return data;
}

RLcdDev::RLcdDev(): iLoaded(EFalse)
{
}

TInt RLcdDev::Open(const TVersion& aVersion)
{
  return DoCreate(KRDevLcd,aVersion,NULL,-1,NULL,NULL);
}

TVersion RLcdDev::VersionRequired(void) const
{
  return(TVersion(3,3,19));
}
