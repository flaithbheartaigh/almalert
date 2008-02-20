/*
    philistpane.hpp
    Copyright (C) 2007-2008 zg

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __PHILISTPANE_HPP__
#define __PHILISTPANE_HPP__

#include <coecntrl.h>
#include <eiklbo.h>
#include "phipaneinterface.hpp"

class CPhiListBox;
class CPhiFs;
class CEikMenuPane;
class CPhiListPane: public CCoeControl,public MPhiPaneInterface
{
  public:
    static CPhiListPane* NewL(const TRect& aRect,MObjectProvider* aObjectProvider);
    ~CPhiListPane();
  public:
    TInt CountComponentControls() const;
    CCoeControl* ComponentControl(TInt aIndex) const;
    TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
    void DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane);
  public: //MPhiPaneInterface
    void NotifyFsEvent(TFsEvent aEvent);
  private:
    void Draw(const TRect& aRect) const;
  private:
    void ConstructL(const TRect& aRect,MObjectProvider* aObjectProvider);
    CPhiListPane();
    void SortSymbol(TDes& aSymbol) const;
  private:
    CPhiListBox* iListBox;
    CPhiFs* iFs;
};

#endif
