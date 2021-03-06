/*
    zg_vibra.hpp
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

#ifndef __ZG_VIBRA_HPP__
#define __ZG_VIBRA_HPP__

#include <isimsg.hpp>

class CIsiMsgCustom: public CIsiMsg
{
  protected:
    void ConstructL(TInt aSize);
};

class CVibraReq: public CIsiMsgCustom
{
  public:
    static CVibraReq* NewL(TUint8 aTransactionId,TUint8 aState); //848
};

class CVibraIntensityReq: public CIsiMsgCustom
{
  public:
    static CVibraIntensityReq* NewL(TUint8 aTransactionId,TUint8 anIntensity); //1623
};

#endif
