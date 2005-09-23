/*
    gendll.hpp
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

#ifndef __GENDLL_HPP__
#define __GENDLL_HPP__

#include <e32base.h>
#include "phonet.hpp"
#include "isimsg.hpp"

class CPnMsg;

class CGeneral: public CActive
{
  protected:
    IMPORT_C void DoCancel(void); //ordinal 3
    IMPORT_C void RunL(void); //ordinal 8
  protected:
    //0x18
    virtual void DoResponse(TUint8 aFunction,TUint8 aTransactionId,TUint8 aUnit)=0;
  protected:
    IMPORT_C void DoReceiveL(void); //ordinal 4
    IMPORT_C TInt DoSendL(CPnMsg &aMsg); //ordinal 5
    IMPORT_C TInt DoSendL(TUint8 aFunction,TDesC8* aData,TUint8 aUnit,TUint8 aParam4,TUint8 aParam5,TUint8 aTransactionId); //ordinal 6
    IMPORT_C TInt SendIsiEvent(CPnMsg &aMsg) const; //ordinal 9
    IMPORT_C TInt Unknown_07(void);
    IMPORT_C TInt Unknown_10(void);
    IMPORT_C TInt Unknown_12(void);
  public:
    IMPORT_C CGeneral(); //ordinal 1
    IMPORT_C ~CGeneral(); //ordinal 11
    IMPORT_C void ConstructL(const TUint8 aObjectNumber); //ordinal 2
  private:
    CIsiMsg* iRecvMsg; //0x18
    TPtrC8 iResponseData; //0x1c
    RPhoNet* iPhoNet; //0x24
    CPnMsg* iSendMsg; //0x28
    HBufC8* iBuf; //0x2c
    TPnReceiveAllocationLengthPckg iPckgLength; //0x30
  private:
    void ErrorResponse(void);
    void Response(void);
    TInt CreateCustomMessage(TUint8 aFunction,TDesC8* aData,TUint8 aUnit,TUint8 aParam4,TUint8 aParam5,TUint8 aTransactionId);
    TInt SendCustomMessage(void);
};

#endif
