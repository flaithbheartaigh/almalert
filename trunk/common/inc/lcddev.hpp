/*
    lcddev.hpp
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

#ifndef __LCDDEV_HPP__
#define __LCDDEV_HPP__

#include <e32std.h>

class RLcdDev: public RBusLogicalChannel
{
  public:
    IMPORT_C static RLcdDev* NewL(void);
  public:
    virtual ~RLcdDev(); //0x08
    virtual TInt Load(TBool aFull); //0x0c
    virtual void Unload(void); //0x10
    virtual TBool IsLoaded(void); //0x14
    virtual TInt Func1(TInt aParam1,TInt aParam2,TInt aParam3); //0x18
    virtual TInt Func2(TInt aParam); //0x1c
    virtual TInt Func3(void); //0x20
    virtual TInt Func4(void); //0x24
    virtual TInt Func5(TInt aParam); //0x28
    virtual TInt Func6(TAny* aData); //0x2c
    virtual TInt Func7(TAny* aData); //0x30
    virtual TInt Func8(TAny* aData); //0x34
    virtual TInt Func9(TInt aParam1,TInt aParam2,TInt& aParam3); //0x38
    virtual TInt FuncA(TInt aParam1,TInt aParam2); //0x3c
    virtual TInt FuncB(TInt aParam1,TInt aParam2,TInt aParam3); //0x40
    virtual TInt FuncC(TInt aParam1,TInt aParam2); //0x44
    virtual TInt FuncD(void); //0x48
  private:
    RLcdDev();
    TVersion VersionRequired(void) const;
    TInt Open(const TVersion& aVersion);
  private:
    TBool iLoaded;
};

#endif
