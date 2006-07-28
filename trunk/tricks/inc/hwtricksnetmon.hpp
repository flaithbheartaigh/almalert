/*
    hwtricksnetmon.hpp
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

#ifndef __HWTRICKSNETMON_HPP__
#define __HWTRICKSNETMON_HPP__

#include "hwtricks.hpp"
#include <isi_test.hpp>

class CNetmonValue: public CBase
{
  public:
    ~CNetmonValue();
    static CNetmonValue* NewLC(TUint8 aUnit,TUint16 aAddress,TBool anExtended);
    void ValueL(TDes16& aTarget,TBool aRaw,TBool aSigned);
  private:
    CNetmonValue(TUint8 aUnit,TUint16 aAddress);
    void ConstructL(TBool anExtended);
    void CleanupL(void);
  private:
    TUint8 iUnit;
    TUint16 iAddress;
    CHWServer* iServer;
    CIsiMsg* iSendMsg;
    CIsiMsg* iRecvMsg;
};

#endif
