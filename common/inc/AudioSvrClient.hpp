/*
    AudioSvrClient.hpp
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

#ifndef __AUDIOSVRCLIENT_HPP__
#define __AUDIOSVRCLIENT_HPP__

#include <e32base.h>

class RAdspServerSession: public RSessionBase
{
  public:
    IMPORT_C TInt AudioSendReceive(TInt aFunction,TAny* aPtr) const; //17
};

class CAudioClient: public CBase
{
  public:
    IMPORT_C static CAudioClient* NewL(void); //08
    IMPORT_C TInt Connect(void); //02
    inline RAdspServerSession& Session(void) {return iSession;};
    IMPORT_C void Close(void); //24
  public: //interface
    virtual void Unknown1(void); // 0x0c
    virtual TInt Unknown2(TInt anOp,TUint16 anId); // 0x10
    virtual void Unknown3(void);
    virtual void Unknown4(void);
    virtual void Unknown5(void);
    virtual void Unknown6(void); // 0x20
    virtual void Unknown7(void);
    virtual void Unknown8(void);
    virtual void Unknown9(void);
    virtual void UnknownA(void); // 0x30
    virtual void UnknownB(void);
    virtual void UnknownC(void);
    virtual void UnknownD(void);
    virtual TInt UnknownE(TUint16& anId,TDesC& aFileName); // 0x40
    virtual void UnknownF(void);
    virtual void UnknownG(void);
    virtual void UnknownH(void);
    virtual void UnknownI(void);
    virtual void UnknownJ(void);
    virtual void UnknownK(void);
    virtual void UnknownL(void);
  private:
    RAdspServerSession iSession;
};

class CMusicClient: private CAudioClient
{
};

class CRadioClient: private CAudioClient
{
};

#endif
