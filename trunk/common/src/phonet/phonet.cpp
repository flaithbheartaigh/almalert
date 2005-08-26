/*
    phonet.cpp
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

#include <phonet.hpp>

const TUint16 KDriver[]={L'N',L'O',L'S',L'D',L'R',L'I',L'V',L'E',L'R',0};

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
{
  return KErrNone;
}

EXPORT_C RPhoNet::RPhoNet(): iActive(0),iObjectNumber(0),iOpened(0)
{
}

EXPORT_C RPhoNet::~RPhoNet()
{
  if(iOpened) Close();
}

EXPORT_C TInt RPhoNet::Open(const TUint8 aObjectNumber)
{
  TInt err=KErrNone;
  iObjectNumber=aObjectNumber;
  switch(aObjectNumber)
  {
    //phonetsy.tsy
    case 0x37: //qd: 0x36
      {
        TVersion version(1,0,1);
        TPtrC ptr(KDriver);
        err=DoCreate(ptr,version,NULL,0x02,NULL,NULL);
        //FIXME
      }
      break;
    //TestServer.exe
    case 0x38://qd: 0x37
      {
        TVersion version(1,0,1);
        TPtrC ptr(KDriver);
        err=DoCreate(ptr,version,NULL,0x0c,NULL,NULL);
      }
      break;
    //Nokia.dsy
    case 0x39://qd: 0x38
      {
        TVersion version(1,0,1);
        TPtrC ptr(KDriver);
        err=DoCreate(ptr,version,NULL,0x0d,NULL,NULL);
      }
      break;
    //emon.exe
    case 0x3a://qd: 0x39
      {
        TVersion version(1,0,1);
        TPtrC ptr(KDriver);
        err=DoCreate(ptr,version,NULL,0x0f,NULL,NULL);
      }
      break;
    //EInfoServer.exe
    case 0x3b://qd: 0x3a
      {
        TVersion version(1,0,1);
        TPtrC ptr(KDriver);
        err=DoCreate(ptr,version,NULL,0x12,NULL,NULL);
      }
      break;
    //adspserver.exe
    case 0x3c://qd: 0x55 - GD1ENG.DLL
      {
        TVersion version(1,0,1);
        TPtrC ptr(KDriver);
        err=DoCreate(ptr,version,NULL,0x16,NULL,NULL);
      }
      break;
    //CRadioEngine.dll
    case 0x3d://qd: no
      {
        TVersion version(1,0,1);
        TPtrC ptr(KDriver);
        err=DoCreate(ptr,version,NULL,0x17,NULL,NULL);
      }
      break;
    //AudioServer.exe
    case 0x3e://qd: no
      {
        TVersion version(1,0,1);
        TPtrC ptr(KDriver);
        err=DoCreate(ptr,version,NULL,0x18,NULL,NULL);
      }
      break;
    //Nokia.dsy
    case 0x3f://qd: 0x3b
      {
        TVersion version(1,0,1);
        TPtrC ptr(KDriver);
        err=DoCreate(ptr,version,NULL,0x15,NULL,NULL);
      }
      break;
    //nowhere
    case 0x4a://qd: 0x46 - ppc.app
      {
        TVersion version(1,0,1);
        TPtrC ptr(KDriver);
        err=DoCreate(ptr,version,NULL,0x0e,NULL,NULL);
      }
      break;
    //dataport.csy
    case 0x4b://qd: 0x47 - dataport.csy
      {
        TVersion version(1,0,1);
        TPtrC ptr(KDriver);
        err=DoCreate(ptr,version,NULL,0x00,NULL,NULL);
        //FIXME
      }
      break;
    case 0x4c://qd: 0x48
      {
        TVersion version(1,0,1);
        TPtrC ptr(KDriver);
        err=DoCreate(ptr,version,NULL,0x14,NULL,NULL);
      }
      break;
    case 0x4d://qd: 0x49 - nowhere
      {
        TVersion version(1,0,1);
        TPtrC ptr(KDriver);
        err=DoCreate(ptr,version,NULL,0x04,NULL,NULL);
      }
      break;
    case 0x4e://qd: 0x4a
      {
        TVersion version(1,0,1);
        TPtrC ptr(KDriver);
        err=DoCreate(ptr,version,NULL,0x05,NULL,NULL);
      }
      break;
    case 0x4f://qd: 0x4b
      {
        TVersion version(1,0,1);
        TPtrC ptr(KDriver);
        err=DoCreate(ptr,version,NULL,0x06,NULL,NULL);
      }
      break;
    case 0x50://qd: 0x4c
      {
        TVersion version(1,0,1);
        TPtrC ptr(KDriver);
        err=DoCreate(ptr,version,NULL,0x07,NULL,NULL);
      }
      break;
    case 0x51://qd: 0x4d
      {
        TVersion version(1,0,1);
        TPtrC ptr(KDriver);
        err=DoCreate(ptr,version,NULL,0x08,NULL,NULL);
      }
      break;
    case 0x52://qd: 0x4e
      {
        TVersion version(1,0,1);
        TPtrC ptr(KDriver);
        err=DoCreate(ptr,version,NULL,0x09,NULL,NULL);
      }
      break;
    case 0x53://qd: 0x4f
      {
        TVersion version(1,0,1);
        TPtrC ptr(KDriver);
        err=DoCreate(ptr,version,NULL,0x0a,NULL,NULL);
      }
      break;
    case 0x54://qd: 0x50
      {
        TVersion version(1,0,1);
        TPtrC ptr(KDriver);
        err=DoCreate(ptr,version,NULL,0x0b,NULL,NULL);
      }
      break;
    //dataport.csy
    case 0x55://qd: 0x51 - dataport.csy
      {
        TVersion version(1,0,1);
        TPtrC ptr(KDriver);
        err=DoCreate(ptr,version,NULL,0x01,NULL,NULL);
        //FIXME
      }
      break;
    case 0x56://qd: 0x52
      {
        TVersion version(1,0,1);
        TPtrC ptr(KDriver);
        err=DoCreate(ptr,version,NULL,0x10,NULL,NULL);
      }
      break;
    case 0x57://qd: 0x53
      {
        TVersion version(1,0,1);
        TPtrC ptr(KDriver);
        err=DoCreate(ptr,version,NULL,0x11,NULL,NULL);
      }
      break;
    //nowhere
    case 0x58://qd: 0x54 - nowhere
      {
        TVersion version(1,0,1);
        TPtrC ptr(KDriver);
        err=DoCreate(ptr,version,NULL,0x13,NULL,NULL);
      }
      break;
    case 0x59://qd: no
      {
        TVersion version(1,0,1);
        TPtrC ptr(KDriver);
        err=DoCreate(ptr,version,NULL,0x19,NULL,NULL);
      }
      break;
    //dataport.csy
    case 0x5a://qd: 0x56 - dataport.csy
      {
        TVersion version(1,0,1);
        TPtrC ptr(KDriver);
        err=DoCreate(ptr,version,NULL,0x03,NULL,NULL);
        //FIXME
      }
      break;
    default:
      err=KErrNotSupported;
      break;
  }
  if(err==KErrNone) iOpened=ETrue;
  return err;
}

EXPORT_C TInt RPhoNet::Close(void)
{
  if(iOpened)
  {
    iOpened=EFalse;
    if(iActive)
    {
      iActive->Cancel();
      delete iActive;
    }
    RBusLogicalChannel::Close();
    return KErrNone;
  }
  else return KErrNotReady;
}


EXPORT_C TInt RPhoNet::Send(CPnMsg& aMsg)
{
  if(!iOpened) return KErrNotReady;
  TPtr8& ptr=aMsg.Ptr();
  TPtr8 buffer(ptr);
  TUint8 value=0;
  ptr[2]=value;
  value=iObjectNumber;
  ptr[7]=value;
  return DoControl(4,&buffer);
}

EXPORT_C TInt RPhoNet::Receive(TRequestStatus& aStatus,CPnMsg& aMsg,TPnReceiveAllocationLengthPckg& aLen)
{
  if(!iOpened) return KErrNotReady;
  DoRequest(0,aStatus,&aMsg.Ptr(),&aLen);
  return 0;
}

EXPORT_C TInt RPhoNet::ReceiveCancel(void)
{
  if(!iOpened) return KErrNotReady;
  DoCancel(1);
  return 0;
}


EXPORT_C TInt RPhoNet::Unknown_37(void)
{
  return 0;
}

EXPORT_C TInt RPhoNet::SendIsiEvent(CPnMsg& aMsg) const
{
  return 0;
}

EXPORT_C TInt RPhoNet::Unknown_06(void)
{
  return 0;
}

EXPORT_C TInt RPhoNet::Unknown_05(void)
{
  return 0;
}

EXPORT_C TInt RPhoNet::Unknown_10(void)
{
  return 0;
}

EXPORT_C TInt RPhoNet::Unknown_09(void)
{
  return 0;
}

EXPORT_C TInt RPhoNet::Unknown_08(void)
{
  return 0;
}

EXPORT_C TInt RPhoNet::Unknown_07(void)
{
  return 0;
}

EXPORT_C TInt RPhoNet::Unknown_18(void)
{
  return 0;
}

EXPORT_C TInt RPhoNet::Unknown_17(void)
{
  return 0;
}

EXPORT_C TInt RPhoNet::Unknown_21(void)
{
  return 0;
}

EXPORT_C TInt RPhoNet::Unknown_20(void)
{
  return 0;
}

EXPORT_C TInt RPhoNet::Unknown_29(void)
{
  return 0;
}

EXPORT_C TInt RPhoNet::Unknown_28(void)
{
  return 0;
}

EXPORT_C TInt RPhoNet::Unknown_27(void)
{
  return 0;
}

EXPORT_C TInt RPhoNet::Unknown_26(void)
{
  return 0;
}

EXPORT_C TInt RPhoNet::Unknown_31(void)
{
  return 0;
}

EXPORT_C TInt RPhoNet::Unknown_30(void)
{
  return 0;
}

EXPORT_C TInt RPhoNet::Unknown_25(void)
{
  return 0;
}

EXPORT_C TInt RPhoNet::Unknown_24(void)
{
  return 0;
}

EXPORT_C TInt RPhoNet::Unknown_12(void)
{
  return 0;
}

EXPORT_C TInt RPhoNet::Unknown_15(void)
{
  return 0;
}

EXPORT_C TInt RPhoNet::Unknown_16(void)
{
  return 0;
}

EXPORT_C TInt RPhoNet::Unknown_04(void)
{
  return 0;
}

EXPORT_C TInt RPhoNet::Unknown_03(void)
{
  return 0;
}

EXPORT_C TInt RPhoNet::Unknown_19(void)
{
  return 0;
}

EXPORT_C TInt RPhoNet::Unknown_23(void)
{
  return 0;
}

EXPORT_C TInt RPhoNet::Unknown_22(void)
{
  return 0;
}

EXPORT_C TInt RPhoNet::Unknown_39(void)
{
  return 0;
}

EXPORT_C TInt RPhoNet::Unknown_34(void)
{
  return 0;
}

EXPORT_C TInt RPhoNet::Unknown_13(void)
{
  return 0;
}

EXPORT_C TInt RPhoNet::Unknown_14(void)
{
  return 0;
}
