/*
    philistbox.hpp
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

#ifndef __PHILISTBOX_HPP__
#define __PHILISTBOX_HPP__

#include <eiklbi.h> //CListItemDrawer
#include <eiktxlbx.h> //CEikTextListBox
#include <eiktxlbm.h> //CTextListBoxModel
#include "phiselection.hpp"

#include <aknutils.h>

class CPhiListItemDrawer: public CListItemDrawer
{
  public:
    CPhiListItemDrawer(MTextListBoxModel* aTextListBoxModel);
  protected:
    void DrawActualItem(TInt aItemIndex,const TRect& aActualItemRect,TBool aItemIsCurrent,TBool aViewIsEmphasized,TBool aViewIsDimmed,TBool aItemIsSelected) const;
  protected:
    MTextListBoxModel* iModel;
    CFont* iBold;
    CFont* iPlain;
  private:
    CPhiListItemDrawer();
};

class CPhiSelection;
class CPhiListBox: public CEikTextListBox
{
  public:
    static CPhiListBox* NewL(const CCoeControl* aParent,const TRect& aRect);
    CPhiSelection* SelectionLC(const TDesC& aFolder,TBool aCurrent=EFalse,CPhiSelection::TType aType=CPhiSelection::ECopy,TUint aSetMask=0,TUint aClearMask=0);
  protected:
    void CreateItemDrawerL(void);
    void ConstructL(const CCoeControl* aParent,const TRect& aRect);
};

#endif
