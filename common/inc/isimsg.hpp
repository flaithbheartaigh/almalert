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

#include "pnmsg.hpp"

class CIsiMsg: public CPnMsg
{
  public:
    virtual TInt SubBlockCountIndex(void)=0;
    virtual TInt SubBlockStart(void)=0;
//  private:
//    CIsiMsg();
};

class CIsiMsg976: public CIsiMsg //phonet init??
{
  public:
    IMPORT_C static CIsiMsg976* NewL(TInt aLength); //TUint8???
    TUint8 GetParam10(void);
    TUint8 GetParam10A(void);
    TUint8 GetParam10B(void);
    TUint8 GetParam10C(void);
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
