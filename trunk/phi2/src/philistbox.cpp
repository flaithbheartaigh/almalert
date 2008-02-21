/*
    philistbox.cpp
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

#include "philistbox.hpp"
#include "phientry.hpp"
#include "phipckg.hpp"
#include "phiconst.hpp"

CPhiListItemDrawer::CPhiListItemDrawer(MTextListBoxModel* aTextListBoxModel): CListItemDrawer(),iModel(aTextListBoxModel)
{
}

void CPhiListItemDrawer::DrawActualItem(TInt aItemIndex,const TRect& aActualItemRect,TBool aItemIsCurrent,TBool aViewIsEmphasized,TBool aViewIsDimmed,TBool aItemIsSelected) const
{
  TPckgC16<TPhiEntry> item(iModel->ItemText(aItemIndex));
  const CFont* font=item().IsDir()?LatinBold12():LatinPlain12();
  iGc->UseFont(font);
  iGc->SetBrushStyle(CGraphicsContext::ESolidBrush);
  iGc->SetPenStyle(CGraphicsContext::ESolidPen);
  iGc->SetBrushColor(iBackColor);
  iGc->Clear(aActualItemRect);
  TRect itemRect=aActualItemRect;
  itemRect.Shrink(1,1);
  if(aItemIsCurrent)
  {
    TRect shadowRect=itemRect;
    shadowRect.Move(2,2);
    shadowRect.Resize(-1,-1);
    iGc->SetBrushColor(CEikonEnv::AvkonColor(KPhiColorShadow));
    iGc->Clear(shadowRect);
    iGc->SetBrushColor(iHighlightedBackColor);
    iGc->SetPenColor(iHighlightedTextColor);
  }
  else
  {
    iGc->SetPenColor(iTextColor);
  }
  if(aItemIsSelected)
  {
    iGc->SetPenColor(CEikonEnv::AvkonColor(KPhiColorSelection));
  }
  else if(item().IsHidden())
  {
    iGc->SetPenColor(CEikonEnv::AvkonColor(KPhiColorHidden));
  }
  else if(item().IsExe()||item().IsApp())
  {
    iGc->SetPenColor(CEikonEnv::AvkonColor(KPhiColorExe));
  }
  else if(item().IsImage())
  {
    iGc->SetPenColor(CEikonEnv::AvkonColor(KPhiColorImage));
  }
  TFileName clip(item().iName);
  AknTextUtils::ClipToFit(clip,*font,itemRect.Width()-2*KPhiItemHMargin);
  iGc->DrawText(clip,itemRect,KPhiItemBaseLine+KPhiItemVMargin,CGraphicsContext::ELeft,KPhiItemHMargin);
  iGc->DiscardFont();
}


CPhiListBox* CPhiListBox::NewL(const CCoeControl* aParent,const TRect& aRect)
{
  CPhiListBox* self=new(ELeave)CPhiListBox;
  CleanupStack::PushL(self);
  self->ConstructL(aParent,aRect);
  CleanupStack::Pop(); //self
  return self;
}

CPhiSelection* CPhiListBox::SelectionLC(const TDesC& aFolder,TBool aCurrent,CPhiSelection::TType aType,TUint aSetMask,TUint aClearMask)
{
  CPhiSelection* selection=CPhiSelection::NewLC();
  selection->iType=aType;
  selection->iFolder=aFolder;
  const CListBoxView::CSelectionIndexArray& indexes=*SelectionIndexes();
  if(indexes.Count()&&!aCurrent)
  {
    for(TInt ii=0;ii<indexes.Count();ii++)
    {
      selection->iFiles->AppendL(static_cast<CTextListBoxModel*>(iModel)->ItemText(indexes[ii]));
    }
  }
  else
  {
    selection->iFiles->AppendL(static_cast<CTextListBoxModel*>(iModel)->ItemText(CurrentItemIndex()));
  }
  return selection;
}

void CPhiListBox::CreateItemDrawerL(void)
{
  iItemDrawer=new(ELeave)CPhiListItemDrawer((CTextListBoxModel*)iModel);
}

void CPhiListBox::ConstructL(const CCoeControl* aParent,const TRect& aRect)
{
  CEikTextListBox::ConstructL(aParent,EMultipleSelection|ELoopScrolling|EShiftEnterMarks);
  SetRect(aRect);
}
