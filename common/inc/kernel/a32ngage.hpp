/*
    a32ngage.hpp
    Copyright (C) 2006 zg

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

#ifndef __A32NGAGE_HPP__
#define __A32NGAGE_HPP__

#include <m32std61.hpp>

class TNgage
{
  public:
    //common
    IMPORT_C static TUint32 Register32(TUint anAddr);
    IMPORT_C static void SetRegister32(TUint32 aValue,TUint anAddr);
    IMPORT_C static void ModifyRegister32(TUint anAddr,TUint32 aSetMask,TUint32 aClearMask);
    IMPORT_C static TUint16 Register16(TUint anAddr);
    IMPORT_C static void SetRegister16(TUint16 aValue,TUint anAddr);
    IMPORT_C static void ModifyRegister16(TUint anAddr,TUint16 aSetMask,TUint16 aClearMask);
    //interrupts
    IMPORT_C static void EnableFiq(TInt anId);
    IMPORT_C static void DisableFiq(TInt anId);
    IMPORT_C static void EnableIrq(TInt anId);
    IMPORT_C static void DisableIrq(TInt anId);
};

#endif
