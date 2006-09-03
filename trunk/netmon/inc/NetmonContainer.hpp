/*
    NetmonContainer.hpp
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

#ifndef __NETMONCONTAINER_HPP__
#define __NETMONCONTAINER_HPP__

#include <coecntrl.h>
#include <coemain.h>

class CNetmonRefresh;
class CNetmonFlashSms;
class CNetmonContainer: public CCoeControl,public MCoeForegroundObserver
{
  public:
    static CNetmonContainer* NewL(void);
    ~CNetmonContainer();
  public:
    TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
    void Draw(const TRect& aRect) const;
  public: //MCoeForegroundObserver
    void HandleGainingForeground(void);
    void HandleLosingForeground(void);
  private:
    CNetmonContainer();
    void ConstructL(void);
    void DrawTab(TBool anActive,TInt anIndex) const;
    void DrawExitMark(void) const;
    void DrawCellId(void) const;
    void Value(TUint8 aUnit,TUint16 aAddress,TDes16& aValue) const;
    void Abr(TDes16& aString) const;
    void DrawPage0(void) const;
    void DrawPage1(void) const;
    void DrawPage2(void) const;
    void InitRefresh(void);
    static TInt RefreshTimeout(TAny* aContainer);
  private:
    TInt iTab;
    CPeriodic* iRefresh;
    CNetmonRefresh* iNetmonRefresh;
    CNetmonFlashSms* iNetmonFlashSms;
};

const TInt KTabCount=3;

#define sizeofa(array) (sizeof(array)/sizeof(array[0]))

#endif
