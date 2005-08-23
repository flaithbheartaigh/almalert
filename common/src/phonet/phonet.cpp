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

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
{
  return KErrNone;
}

EXPORT_C RPhoNet::RPhoNet()
{
}

EXPORT_C RPhoNet::~RPhoNet()
{
}

EXPORT_C TInt RPhoNet::Open(const TUint8 aObjectNumber)
{
  return 0;
}

EXPORT_C TInt RPhoNet::Close(void)
{
  return 0;
}

EXPORT_C TInt RPhoNet::Send(CPnMsg& aMsg) const
{
  return 0;
}

EXPORT_C TInt RPhoNet::Receive(TRequestStatus& aStatus,CPnMsg& aMsg,TPnReceiveAllocationLengthPckg& aLen) const
{
  return 0;
}

EXPORT_C TInt RPhoNet::ReceiveCancel(void) const
{
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
