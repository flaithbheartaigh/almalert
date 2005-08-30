/*
    phonet.hpp
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

#ifndef __PHONET_HPP__
#define __PHONET_HPP__

#include <e32std.h>
typedef TPckgBuf<TUint16> TPnReceiveAllocationLengthPckg;

class CPnMsg;

class RPhoNet: public RBusLogicalChannel
{
  public:
    IMPORT_C RPhoNet(); //ordinal 1
    IMPORT_C ~RPhoNet(); //ordinal 38
    IMPORT_C TInt Open(const TUint8 aObjectNumber); //ordinal 11
    IMPORT_C TInt Close(void); //ordinal 02
    IMPORT_C TInt ReceiveCancel(void) const; //ordinal 32
    IMPORT_C TInt Receive(TRequestStatus& aStatus,CPnMsg& aMsg,TPnReceiveAllocationLengthPckg& aLen) const; //ordinal 33
    IMPORT_C TInt Send(CPnMsg& aMsg) const; //ordinal 36
    IMPORT_C TInt SendIsiEvent(CPnMsg& aMsg) const; //ordinal 35
  private:
    IMPORT_C TInt Unknown_03(void);
    IMPORT_C TInt Unknown_04(void);
    IMPORT_C TInt Unknown_05(void);
    IMPORT_C TInt Unknown_06(void);
    IMPORT_C TInt Unknown_07(void);
    IMPORT_C TInt Unknown_08(void);
    IMPORT_C TInt Unknown_09(void);
    IMPORT_C TInt Unknown_10(void);
    IMPORT_C TInt Unknown_12(void);
    IMPORT_C TInt Unknown_13(void);
    IMPORT_C TInt Unknown_14(void);
    IMPORT_C TInt Unknown_15(void);
    IMPORT_C TInt Unknown_16(void);
    IMPORT_C TInt Unknown_17(void);
    IMPORT_C TInt Unknown_18(void);
    IMPORT_C TInt Unknown_19(void);
    IMPORT_C TInt Unknown_20(void);
    IMPORT_C TInt Unknown_21(void);
    IMPORT_C TInt Unknown_22(void);
    IMPORT_C TInt Unknown_23(void);
    IMPORT_C TInt Unknown_24(void);
    IMPORT_C TInt Unknown_25(void);
    IMPORT_C TInt Unknown_26(void);
    IMPORT_C TInt Unknown_27(void);
    IMPORT_C TInt Unknown_28(void);
    IMPORT_C TInt Unknown_29(void);
    IMPORT_C TInt Unknown_30(void);
    IMPORT_C TInt Unknown_31(void);
    IMPORT_C TInt Unknown_34(void);
    IMPORT_C TInt Unknown_37(void);
    IMPORT_C TInt Unknown_39(void);
  private:
    TInt iValue1;
    TInt iValue2;
    TInt iValue3;
};

#endif
