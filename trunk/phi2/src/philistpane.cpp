/*
    philistpane.cpp
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

#include "philistpane.hpp"
#include "philistbox.hpp"
#include "phientry.hpp"
#include "phipckg.hpp"
#include "phiconst.hpp"
#include "phifs.hpp"
#include <aknutils.h>
#include <phi.rsg>
#include "phi.hrh"
#include <eikmenup.h>

CPhiListPane* CPhiListPane::NewL(const TRect& aRect,MObjectProvider* aObjectProvider)
{
  CPhiListPane* self=new(ELeave)CPhiListPane;
  CleanupStack::PushL(self);
  self->ConstructL(aRect,aObjectProvider);
  CleanupStack::Pop(); //self
  return self;
}

CPhiListPane::~CPhiListPane()
{
  delete iFs;
  delete iListBox;
}

TInt CPhiListPane::CountComponentControls() const
{
  return 1;
}

CCoeControl* CPhiListPane::ComponentControl(TInt aIndex) const
{
  switch(aIndex)
  {
    case 0:
      return iListBox;
    default:
      return NULL;
  }
}

TKeyResponse CPhiListPane::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
{
  if(aType==EEventKey)
  {
    if((aKeyEvent.iModifiers&EModifierShift)==0)
    {
      switch(aKeyEvent.iCode)
      {
        case EKeyLeftArrow:
          iFs->FolderUpL();
          return EKeyWasConsumed;
        case EKeyRightArrow:
          iFs->FolderDownL();
          return EKeyWasConsumed;
        case EKeyDevice3:
          iFs->StartItemL();
          return EKeyWasConsumed;
        case EKeyBackspace:
          iFs->DeleteL();
          return EKeyWasConsumed;
        case '*':
          iFs->CopyL();
          return EKeyWasConsumed;
        case '#':
          iFs->CutL();
          return EKeyWasConsumed;
        case '0':
          iFs->PasteL();
          return EKeyWasConsumed;
        case '1':
          iFs->SendL();
          return EKeyWasConsumed;
        case '2':
          iFs->AddToPinboardL();
          return EKeyWasConsumed;
        case '5':
          iFs->ShowInfoL();
          return EKeyWasConsumed;
        case '6':
          iFs->RenameL();
          return EKeyWasConsumed;
        case '7':
          iFs->NewFolderL();
          return EKeyWasConsumed;
        case '9':
          iFs->RefreshL();
          return EKeyWasConsumed;
      }
    }
    else
    {
      switch(aKeyEvent.iCode)
      {
        case '1':
          iFs->SetSortModeL(ESortByName);
          return EKeyWasConsumed;
        case '2':
          iFs->SetSortModeL(ESortByExt);
          return EKeyWasConsumed;
        case '3':
          iFs->SetSortModeL(ESortBySize);
          return EKeyWasConsumed;
        case '4':
          iFs->SetSortModeL(ESortByDate);
          return EKeyWasConsumed;
        case '5':
          iFs->SetAttrL();
          return EKeyWasConsumed;
      }
    }
  }
  TInt index=iListBox->CurrentItemIndex();
  TKeyResponse resp=iListBox->OfferKeyEventL(aKeyEvent,aType);
  if(index!=iListBox->CurrentItemIndex()) iFs->UpdateFileL();
  return resp;
}

void CPhiListPane::Draw(const TRect& aRect) const
{
  CWindowGc& gc=SystemGc();
  const CFont* font=LatinBold12();
  gc.UseFont(font);
  gc.SetBrushColor(CEikonEnv::AvkonColor(0xdf));
  gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
  gc.SetPenStyle(CGraphicsContext::ESolidPen);
  gc.SetPenColor(CEikonEnv::AvkonColor(0));
  gc.Clear(aRect);
  TFileName clip(iFs->CurrentFolder());
  AknTextUtils::ClipToFit(clip,*font,Rect().Width()-2*KPhiItemHMargin-KPhiSortAreaWidth);
  gc.DrawText(clip,TPoint(KPhiItemHMargin,KPhiItemBaseLine));
  TBuf<1> symbol;
  SortSymbol(symbol);
  gc.DrawText(symbol,TRect(TPoint(Rect().Width()-KPhiSortAreaWidth,0),TSize(KPhiSortAreaWidth,KPhiStatusHeight)),KPhiItemBaseLine,CGraphicsContext::ECenter);
  gc.DiscardFont();
}

void CPhiListPane::ConstructL(const TRect& aRect,MObjectProvider* aObjectProvider)
{
  SetMopParent(aObjectProvider);
  CreateWindowL();
  SetRect(aRect);

  TRect rect=aRect;
  rect.iTl.iY=KPhiStatusHeight;
  iListBox=CPhiListBox::NewL(this,rect);

  CDesCArrayFlat* files=new(ELeave)CDesCArrayFlat(6);
  CTextListBoxModel* model=iListBox->Model();
  model->SetItemTextArray(files);
  model->SetOwnershipType(ELbmOwnsItemArray);

  iListBox->CreateScrollBarFrameL(ETrue);
  iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(CEikScrollBarFrame::EOff,CEikScrollBarFrame::EOn);

  iListBox->SetItemHeightL(KPhiItemHeight);

  iFs=CPhiFs::NewL(this,iListBox,files);
}

CPhiListPane::CPhiListPane(): CCoeControl()
{
}

void CPhiListPane::NotifyFsEvent(TFsEvent aEvent)
{
  switch(aEvent)
  {
    case EFsEventFolderChanged:
      DrawDeferred(); //redraw title
      break;
    case EFsEventFileChanged:
      break;
    case EFsEventSortModeChanged:
      DrawDeferred(); //redraw title
    default:
      break;
  }
}

void CPhiListPane::SortSymbol(TDes& aSymbol) const
{
  TInt mode=iFs->SortMode(),label=R_PHI_SORT_NAME;
  switch(mode)
  {
    case ESortByExt:
      label=R_PHI_SORT_EXT;
      break;
    case ESortBySize:
      label=R_PHI_SORT_SIZE;
      break;
    case ESortByDate:
      label=R_PHI_SORT_DATE;
      break;
  }
  ControlEnv()->ReadResourceAsDes16(aSymbol,label);
}

void CPhiListPane::DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane)
{
  _LIT(KAsterisk,"* ");
  switch(aResourceId)
  {
    case R_PHI_SORT_MENU:
      {
        CEikMenuPaneItem::SData& data=aMenuPane->ItemData(EPhiSortName-ESortByName+iFs->SortMode());
        data.iText.Insert(0,KAsterisk);
      }
      break;
  }
}
