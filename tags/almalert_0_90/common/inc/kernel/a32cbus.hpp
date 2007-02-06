/*
    a32cbus.hpp
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

#ifndef __A32CBUS_HPP__
#define __A32CBUS_HPP__

#include <e32base.h>

class CCBus: public CBase
{
  public:
    CCBus() {};
  public:
    virtual void Unknown1(void)=0;
    virtual TInt Read(TInt aParam1,TInt aParam2,TInt8 aParam3,TUint32& aResult)=0;
    virtual TInt Write(TInt aParam1,TInt aParam2,TInt8 aParam3,TInt8 aParam4,TUint32 aClearMask,TUint32 aSetMask)=0;
    virtual TInt Unknown4(TInt aParam1,TInt aParam2)=0;
    virtual void Unknown5(TInt aParam1)=0;
    virtual TInt Unknown6(void)=0;
};

#endif
