/*
    AudioSvrClient.cpp
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

#include <AudioSvrClient.hpp>

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
{
  return KErrNone;
}

EXPORT_C void func_1(void) {}
EXPORT_C void func_3(void) {}
EXPORT_C void func_4(void) {}
EXPORT_C void func_5(void) {}
EXPORT_C void func_6(void) {}
EXPORT_C void func_7(void) {}
EXPORT_C void func_9(void) {}
EXPORT_C void func_10(void) {}
EXPORT_C void func_11(void) {}
EXPORT_C void func_12(void) {}
EXPORT_C void func_13(void) {}
EXPORT_C void func_14(void) {}
EXPORT_C void func_15(void) {}
EXPORT_C void func_16(void) {}
EXPORT_C void func_18(void) {}
EXPORT_C void func_19(void) {}
EXPORT_C void func_20(void) {}
EXPORT_C void func_21(void) {}
EXPORT_C void func_22(void) {}
EXPORT_C void func_23(void) {}

EXPORT_C CAudioClient* CAudioClient::NewL(void)
{
  return NULL;
}

EXPORT_C TInt CAudioClient::Connect(void)
{
  return KErrNone;
}

EXPORT_C TInt RAdspServerSession::AudioSendReceive(TInt aFunction,TAny* aPtr) const
{
  return KErrNone;
}

EXPORT_C void CAudioClient::Close(void)
{
}
