/*
    pnmsg.hpp
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

#ifndef __PNMSG_HPP__
#define __PNMSG_HPP__

#include <e32base.h>

class CPnMsg: public CBase
{
  public:
    enum TPnMsgPanic
    {
      EPanicPnMsgUnderflow=KErrUnderflow,
      EPanicPnMsgNoMemory=KErrNoMemory
    };
  public:
    ~CPnMsg() {if(iBuf) {delete iBuf;iBuf=NULL;}};
  public:
    IMPORT_C TInt ConstructL(TInt aSize); //ordinal 4
    IMPORT_C static CPnMsg* NewL(CPnMsg &aMsg); //ordinal 2
    IMPORT_C static CPnMsg* NewL(TDes8 const& aStr); //ordinal 3
    IMPORT_C static CPnMsg* NewL(TUint8 aParam1,TDesC8 const& aStr); //ordinal 1
    IMPORT_C static CPnMsg* NewL(TUint8 aParam1,TUint8 aParam2,TDesC8 const& aStr); //ordinal 5
    IMPORT_C static CPnMsg* NewL(TInt aSize); //ordinal 6
    IMPORT_C CPnMsg& operator=(CPnMsg &aMsg); //ordinal 7
  public:
    inline TDes8& Ptr() {return iPtr;};
  protected:
    TPtr8 iPtr; //0x04
    HBufC8* iBuf; //0x10
  protected:
    CPnMsg(): CBase(),iPtr(0,0),iBuf(NULL) {};
  private:
    static void Panic(TPnMsgPanic aPanic);
};

#endif
