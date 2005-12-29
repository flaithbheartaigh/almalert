/*
    hwtricksextender.hpp
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

#ifndef __HWTRICKSEXTENDER_HPP__
#define __HWTRICKSEXTENDER_HPP__

#include "hwtricks.hpp"

class CHWExtender: public CBase
{
  public:
    static CHWExtender* NewL(CHWServer* aServer);
    ~CHWExtender();
    static TBool IsSupported(void);
  private:
    CHWExtender();
    void ConstructL(CHWServer* aServer);
    static TUint Address(void);
    static void PatchL(TUint anAddress,TUint8 aValue,TUint8& anOldValue);
  private:
    TBool iPatched;
    TUint8 iOldValue;
    TUint iAddress;
};

#define w(W) __(D##W)
#define ww(_g) ___(_g##g)
#define _(bb) w(bb##b)
#define __(v) ww(v##u)
#define ___(____) R##____
#define user _(e)
#define op(a,b,c,d) a##Memory(b,(TUint)(c),d##P,sizeof(d))

#endif
