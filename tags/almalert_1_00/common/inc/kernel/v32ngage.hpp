/*
    v32ngage.hpp
    Copyright (C) 2006 zg

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

#ifndef __V32NGAGE_HPP__
#define __V32NGAGE_HPP__

#include <m32std61.hpp>

class Custom
{
  public:
    enum TInterfaces
    {
      ECBus,
      ERtc,
      EKeyboard,
      EUnknown3, //gsm stack
      ELcd,
      ESit, //infrared
      EUnknown6, //gsm stack
      EUnused,
      EProductInfo, //dont' exist on n-gage 3.30
      ECount
    };
  public:
    virtual void Init1(void)=0; //0x008
    virtual void Init2(void)=0; //0x00c
    virtual void Init3(void)=0; //0x010
    virtual void Init4(void)=0; //0x014
    virtual void Init5(void)=0; //0x018
    virtual TBool TestBootSequence(void)=0; //0x01c
    virtual void Unknown_020(void)=0;
    virtual void Unknown_024(void)=0;
    virtual void Unknown_028(void)=0;
    virtual void Unknown_02c(void)=0;
    virtual void Unknown_030(void)=0;
    virtual void Unknown_034(void)=0;
    virtual void Unknown_038(void)=0;
    virtual void Unknown_03c(void)=0;
    virtual void Unknown_040(void)=0;
    virtual void Unknown_044(void)=0;
    virtual void Unknown_048(void)=0;
    virtual void Unknown_04c(void)=0;
    virtual void Unknown_050(void)=0;
    virtual void Unknown_054(void)=0;
    virtual void Unknown_058(void)=0;
    virtual void Unknown_05c(void)=0;
    virtual void Unknown_060(void)=0;
    virtual void Unknown_064(void)=0;
    virtual void Unknown_068(void)=0;
    virtual void Unknown_06c(void)=0;
    virtual void Unknown_070(void)=0;
    virtual void Unknown_074(void)=0;
    virtual void Unknown_078(void)=0;
    virtual void Unknown_07c(void)=0;
    virtual void Unknown_080(void)=0;
    virtual void Unknown_084(void)=0;
    virtual void Unknown_088(void)=0;
    virtual void Unknown_08c(void)=0;
    virtual void Unknown_090(void)=0;
    virtual void Unknown_094(void)=0;
    virtual void Unknown_098(void)=0;
    virtual void Unknown_09c(void)=0;
    virtual void Unknown_0a0(void)=0;
    virtual void Unknown_0a4(void)=0;
    virtual void Unknown_0a8(void)=0;
    virtual void Unknown_0ac(void)=0;
    virtual void Unknown_0b0(void)=0;
    virtual void Unknown_0b4(void)=0;
    virtual void Unknown_0b8(void)=0;
    virtual void Unknown_0bc(void)=0;
    virtual void Unknown_0c0(void)=0;
    virtual void Unknown_0c4(void)=0;
    virtual void Unknown_0c8(void)=0;
    virtual void Unknown_0cc(void)=0;
    virtual void Unknown_0d0(void)=0;
    virtual void Unknown_0d4(void)=0;
    virtual void Unknown_0d8(void)=0;
    virtual void Unknown_0dc(void)=0;
    virtual void Unknown_0e0(void)=0;
    virtual void Unknown_0e4(void)=0;
    virtual void Unknown_0e8(void)=0;
    virtual void Unknown_0ec(void)=0;
    virtual void Unknown_0f0(void)=0;
    virtual void Unknown_0f4(void)=0;
    virtual void Unknown_0f8(void)=0;
    virtual TBool SetInterface(TInterfaces anId,TAny* anInterface)=0;
    virtual TAny* Interface(TInterfaces anId)=0;
    virtual void Unknown_104(void)=0;
    virtual void Unknown_108(void)=0;
    virtual void Unknown_10c(void)=0;
    virtual void Unknown_110(void)=0;
    virtual void Unknown_114(void)=0;
    virtual void Unknown_118(void)=0;
    virtual void Unknown_11c(void)=0;
    virtual void Unknown_120(void)=0;
    virtual void Unknown_124(void)=0;
    virtual void Unknown_128(void)=0;
    virtual void Unknown_12c(void)=0;
    virtual void Unknown_130(void)=0;
    virtual void Unknown_134(void)=0;
    virtual void Unknown_138(void)=0;
    virtual void Unknown_13c(void)=0;
    virtual void Unknown_140(void)=0;
    virtual void Unknown_144(void)=0;
    virtual void Unknown_148(void)=0;
    virtual void Unknown_14c(void)=0;
    virtual void Unknown_150(void)=0;
    virtual void Unknown_154(void)=0;
    virtual void Unknown_158(void)=0;
    virtual void Unknown_15c(void)=0;
    virtual void Unknown_160(void)=0;
    virtual void Unknown_164(void)=0;
    virtual void Unknown_168(void)=0;
    virtual void Unknown_16c(void)=0;
    virtual void Unknown_170(void)=0;
    virtual void Unknown_174(void)=0;
};

class Variant: public Custom
{
  public:
    void Init1(void); //0x008
    void Init2(void); //0x00c
    void Init3(void); //0x010
    void Init4(void); //0x014
    void Init5(void); //0x018
    TBool TestBootSequence(void); //0x01c
    TBool SetInterface(TInterfaces anId,TAny* anInterface); //0x0fc
    TAny* Interface(TInterfaces anId); //0x100
  private:
    TInt iUnknown01; //0x04
    TInt iUnknown02; //0x08
    TInt iUnknown03; //0x0c
    TInt iUnknown04; //0x10
    TInt iUnknown05; //0x14
    TInt iUnknown06; //0x18
    TAny* iInterfaces[ECount]; //0x1c
};

#endif
