/*
    isi_test.hpp
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

#ifndef __ISI_TEST_HPP__
#define __ISI_TEST_HPP__

#include <isimsg.hpp>
#include <isi_units.hpp>

const TUint8 KTestStateNet=2;
const TUint8 KTestStateOn=4;
const TUint8 KTestStateOff=8;

class CTestGetReq: public CIsiMsg
{
  public:
    IMPORT_C static CTestGetReq* NewL(TUint8 aTransactionId,TUint8 aUnit,TUint8 aParam,TUint8 aCount,const TDesC16& aData); //970
};

class CTestRemoveReq: public CIsiMsg
{
  public:
    IMPORT_C static CTestRemoveReq* NewL(TUint8 aTransactionId,TUint8 aUnit,TUint8 aParam,TUint8 aCount,const TDesC16& aData); //971
};

class CTestSetReq: public CIsiMsg
{
  public:
    IMPORT_C static CTestSetReq* NewL(TUint8 aTransactionId,TUint8 aUnit,TUint8 aParam1,TUint8 aParam2,TUint16 aParam3,TUint16 aParam4,const TDesC8& aData); //972
};

class CTestStateReq: public CIsiMsg
{
  public:
    IMPORT_C static CTestStateReq* NewL(TUint8 aTransactionId); //768
};

class CTestGetResp: public CIsiMsg
{
  public:
    IMPORT_C CSubBlockArray* SubBlocksL(void); //150
    IMPORT_C TInt SubBlockCountIndex(void); //1400
    IMPORT_C TInt SubBlockStart(void); //1499
};

class CTestInfo: public CSubBlock
{
  public:
    IMPORT_C void String(TDes16& aTarget); //1297
};

#endif
