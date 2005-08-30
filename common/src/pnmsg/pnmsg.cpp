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

#include "pnmsg.hpp"

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
{
  return KErrNone;
}

EXPORT_C TInt CPnMsg::ConstructL(TInt aSize)
{
  return 0;
}

EXPORT_C CPnMsg* CPnMsg::NewL(TInt aSize)
{
  return NULL;
}

EXPORT_C CPnMsg* CPnMsg::NewL(TUint8 aParam1,TDesC8 const& aStr)
{
  return NULL;
}

EXPORT_C CPnMsg* CPnMsg::NewL(TUint8 aParam1,TUint8 aParam2,TDesC8 const& aStr)
{
  return NULL;
}

EXPORT_C CPnMsg* CPnMsg::NewL(CPnMsg &aMsg)
{
  return NULL;
}

EXPORT_C CPnMsg* CPnMsg::NewL(TDes8 const& aStr)
{
  return NULL;
}

EXPORT_C CPnMsg& CPnMsg::operator=(CPnMsg &aMsg)
{
  return *this;
}
