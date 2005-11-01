/*
    isimsg.hpp
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

#ifndef __ISIMSG_HPP__
#define __ISIMSG_HPP__

#include <pnmsg.hpp>

class CSubBlock;

class CIsiMsg: public CPnMsg
{
  public:
    IMPORT_C ~CIsiMsg(); //1600
    IMPORT_C static CIsiMsg* NewL(TDesC8& aData); //975
    IMPORT_C static CIsiMsg* NewL(TInt aSize); //976
    IMPORT_C virtual TInt SubBlockCountIndex(void); //1410
    IMPORT_C virtual TInt SubBlockStart(void); //1509
    IMPORT_C void Move(CIsiMsg* aTarget); //611
    IMPORT_C TUint8 SubBlockCount(void); //990
    IMPORT_C CSubBlock* SubBlock(void); //1413
    IMPORT_C void Append(CSubBlock* aBlock); //34
  public:
    inline TUint8 Unit(void) {return iPtr[3];};
    inline TUint8 Transaction(void) {return iPtr[8];};
    inline TUint8 Function(void) {return iPtr[9];};
    inline void SetUnit(TUint8 aUnit) {iPtr[3]=aUnit;};
  protected:
    void ConstructL(TInt aSize);
};

class CSubBlock: public CBase
{
  public:
    IMPORT_C CSubBlock(); //1605
    IMPORT_C ~CSubBlock(); //1602
    IMPORT_C virtual TInt SubBlockCountIndex(); //1411
    IMPORT_C virtual TInt SubBlockStart(); //1510
    IMPORT_C void Move(CSubBlock* aTarget); //613
    IMPORT_C static CSubBlock* NewL(TDesC8& aData,TInt anOffset,TUint8 aUnit); //984
    IMPORT_C static CSubBlock* NewL0(TDesC8& aData,TInt anOffset); //983
    IMPORT_C static CSubBlock* NewL1(TDesC8& aData,TInt anOffset); //985
    IMPORT_C TInt Length(void); //1186
    IMPORT_C TPtrC8 Data(void); //200
    IMPORT_C TUint16 Address(void); //149
  public:
    inline TDes8& Ptr() {return iPtr;};
  protected:
    void ConstructL(TInt aSize);
  protected:
    TUint8 iUnknown; //0x04
    TPtr8 iPtr; //0x08
    HBufC8* iBuf; //0x14
};

class CSubBlockArray: public CBase
{
  public:
    IMPORT_C static CSubBlockArray* NewL(TUint8 aNumber); //762
    IMPORT_C ~CSubBlockArray(); //1599
  public:
    IMPORT_C CSubBlock*& operator[](TUint8 aIndex); //1
    IMPORT_C TUint8 Number(void); //1006
    IMPORT_C TInt DataSize(void); //192
  private:
    void ConstructL(TUint8 aAmount);
  private:
    TUint8 iAmount;
    CSubBlock** iSubBlocks;
};

//really not exist
class CIsiMsg976: public CIsiMsg //phonet init??
{
  public:
    //IMPORT_C static CIsiMsg976* NewL(TInt aLength); //TUint8??? 976
    TUint8 GetParam10(void); //259
    TUint8 GetParam10A(void); //516
    TUint8 GetParam10B(void); //524
    TUint8 GetParam10C(void); //242
};

class CIsiMsg802: public CIsiMsg
{
  public:
    IMPORT_C static CIsiMsg802* NewL(TUint8 aParam1,TUint8 aParam2);
};

class CIsiMsg848: public CIsiMsg //vibra. only on qd
{
  public:
    IMPORT_C static CIsiMsg848* NewL(TUint8 aParam1,TUint8 aParam2);
};

class CIsiMsg850: public CIsiMsg //
{
  public:
    IMPORT_C static CIsiMsg850* NewL(TUint8 aParam1,TUint8 aParam2,TUint8 aParam3,TUint8 aParam4);
};

class CIsiMsg649: public CIsiMsg //
{
  public:
    IMPORT_C static CIsiMsg649* NewL(TUint8 aParam1,TUint8 aParam2,TUint8 aParam3);
};

class CIsiMsg691: public CIsiMsg //
{
  public:
    IMPORT_C static CIsiMsg691* NewL(TUint8 aParam1,TUint8 aParam2,TUint8 aParam3,CIsiMsg649* aMsg);
};

#endif
