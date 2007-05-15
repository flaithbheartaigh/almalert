/*
    BatmonContainer.hpp
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

#ifndef __BATMONCONTAINER_HPP__
#define __BATMONCONTAINER_HPP__

#include <coecntrl.h>
#include <coemain.h>

class CBatmonContainer: public CCoeControl,public MCoeForegroundObserver
{
  public:
    static CBatmonContainer* NewL(void);
    ~CBatmonContainer();
  public:
    TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
    void Draw(const TRect& aRect) const;
  public: //MCoeForegroundObserver
    void HandleGainingForeground(void);
    void HandleLosingForeground(void);
  private:
    CBatmonContainer();
    void ConstructL(void);
    void Value(TUint8 aUnit,TUint16 aAddress,TDes16& aValue) const;
    void Abr(TDes16& aString) const;
    void InitRefresh(void);
    static TInt RefreshTimeout(TAny* aContainer);
  private:
    void Temperature(TDes16& aValue,TUint16 aAddress) const;
    void Voltage(TDes16& aValue,TUint16 aAddress) const;
    void Current(TDes16& aValue,TUint16 aAddress) const;
    void Capacity(TDes16& aValue,TUint16 aAddress) const;
    void Impedance(TDes16& aValue,TUint16 aAddress) const;
    void Hour(TDes16& aValue,TUint16 aAddress) const;
    void Battery(TDes16& aValue,TUint16 aAddress) const;
    void Common(TDes16& aValue,TUint16 aAddress) const;
  private:
    void DrawTitle(void) const;
    void DrawStatus(void) const;
    void DrawExitMark(void) const;
    void DrawPage(void) const;
    void DrawRight(const TRect& aRect) const;
  private:
    CPeriodic* iRefresh;
};

struct SValue
{
  TInt iWidth;
  wchar_t* iTitle;
  TUint16 iAddress;
  void (CBatmonContainer::*iFunc)(TDes16& aValue,TUint16 aAddress) const;
};

#define sizeofa(array) (sizeof(array)/sizeof(array[0]))

#endif
