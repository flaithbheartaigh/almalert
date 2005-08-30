/*
    isi_light.hpp
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

#ifndef __ISI_LIGHT_HPP__
#define __ISI_LIGHT_HPP__

#include <isimsg.hpp>

class CLightBrightnessGetReq: public CIsiMsg
{
  public:
    IMPORT_C static CLightBrightnessGetReq* NewL(TUint8 aParam,TDesC8& aData); //902
};

class CLightBrightnessGetResp: public CIsiMsg
{
  public:
    IMPORT_C TInt SubBlockCountIndex(void); //1388
    IMPORT_C TInt SubBlockStart(void); //1487
};

class CLightBrightnessSetReq: public CIsiMsg
{
  public:
    IMPORT_C static CLightBrightnessSetReq* NewL(TUint8 aParam,CSubBlock* aSubBlock); //928
};

class CLightSwitchReq: public CIsiMsg
{
  public:
    IMPORT_C static CLightSwitchReq* NewL(TUint8 aParam,TUint8 aType,TUint8 aState,CSubBlock* aSubBlock); //788
};

class CLightBrightnessInfo: public CSubBlock
{
  public:
    IMPORT_C TUint8 Brightness1(void); //30
    IMPORT_C TUint8 Brightness2(void); //31
};

#endif
