/*
    avkon_CAknCheckBoxSettingPage.cpp
    Copyright (C) 2008 zg

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

#include <akncheckboxsettingpage.h>
#include <akniconarray.h>

CAknListBoxSettingPage::CAknListBoxSettingPage(TInt aSettingPageResourceId): CAknSettingPage(aSettingPageResourceId) //FIXME
{
}

CAknListBoxSettingPage::CAknListBoxSettingPage(const TDesC* aSettingTitleText,TInt aSettingNumber,TInt aControlType,TInt aEditorResourceId,TInt aSettingPageResourceId): CAknSettingPage(aSettingTitleText,aSettingNumber,aControlType,aEditorResourceId,aSettingPageResourceId) //FIXME
{
}

EXPORT_C CSelectableItem::CSelectableItem(TDesC& aItemText,TBool aSelected): CBase(),iDesC(aItemText),iSelected(aSelected)
{
}

EXPORT_C void CSelectableItem::ConstructL(void)
{
  iItem=iDesC.AllocL();
}

CSelectableItem::~CSelectableItem()
{
  delete iItem;
}

EXPORT_C void CSelectableItem::SetSelectionStatus(TBool aSelected)
{
  iSelected=aSelected;
}

EXPORT_C TBool CSelectableItem::SelectionStatus(void)
{
  return iSelected;
}

EXPORT_C TPtrC CSelectableItem::ItemText(void)
{
  TPtrC ptr(*iItem);
  return ptr;
}

EXPORT_C CSelectionItemList::CSelectionItemList(TInt Granularity): CArrayPtrFlat<CSelectableItem>(Granularity)
{
}

EXPORT_C TInt CSelectionItemList::MdcaCount(void) const
{
  return Count();
}

EXPORT_C TPtrC CSelectionItemList::MdcaPoint(TInt aIndex) const
{
  return At(aIndex)->ItemText();
}

EXPORT_C CAknCheckBoxSettingPage::~CAknCheckBoxSettingPage()
{
  delete iInternalItemArray;
}

EXPORT_C CAknCheckBoxSettingPage::CAknCheckBoxSettingPage(TInt aResourceID,CSelectionItemList* aItemArray): CAknListBoxSettingPage(aResourceID),iItemArray(aItemArray)
{
}

EXPORT_C CAknCheckBoxSettingPage::CAknCheckBoxSettingPage(const TDesC* aSettingTitleText,TInt aSettingNumber,TInt aControlType,TInt aEditorResourceId,TInt aSettingPageResourceId,CSelectionItemList* aItemArray): CAknListBoxSettingPage(aSettingTitleText,aSettingNumber,aControlType,aEditorResourceId,aSettingPageResourceId),iItemArray(aItemArray)
{
}

EXPORT_C void CAknCheckBoxSettingPage::ConstructL(void)
{
  BaseConstructL();
  CAknSetStyleListBox* listbox=ListBoxControl();
  CAknIconArray* icons=new(ELeave)CAknIconArray(2);
  CleanupStack::PushL(icons);
  icons->ConstructFromResourceL(R_AVKON_SETTING_PAGE_CHECKBOX_ICONS);
  listbox->ItemDrawer()->FormattedCellData()->SetIconArrayL(icons);
  CleanupStack::Pop(); //icons
  GenerateInternalArrayAndGiveToListBoxL();
  CheckAndSetDataValidity();
  UpdateCbaL();
  listbox->SetListBoxObserver(this);
  SetAllSelectionsL();
  listbox->CreateScrollBarFrameL(ETrue);
  listbox->ScrollBarFrame()->SetScrollBarVisibilityL(CEikScrollBarFrame::EOff,CEikScrollBarFrame::EAuto);
  listbox->UpdateScrollBarsL();
  SizeChanged();
}

EXPORT_C void CAknCheckBoxSettingPage::HandleListBoxEventL(CEikListBox* aListBox,MEikListBoxObserver::TListBoxEvent aEventType)
{
  switch(aEventType)
  {
    case EEventEnterKeyPressed:
    case EEventItemDoubleClicked:
    case EEventItemActioned:
      ToggleSelectionL();
      UpdateSettingL();
      break;
    default:
      break;
  }
}

void CAknCheckBoxSettingPage::ToggleSelectionL(void) //FIXME
{
  CAknSetStyleListBox* listbox=ListBoxControl();
  CListBoxView* view=listbox->View();
  TInt index=listbox->CurrentItemIndex();
  if(iItemArray->At(index)->SelectionStatus())
  {
    view->DeselectItem(index);
    iItemArray->At(index)->SetSelectionStatus(EFalse);
  }
  else
  {
    view->SelectItemL(index);
    iItemArray->At(index)->SetSelectionStatus(ETrue);
  }
  if(iSettingPageObserver) iSettingPageObserver->HandleSettingPageEventL(this,MAknSettingPageObserver::EEventSettingChanged);
}

EXPORT_C void CAknCheckBoxSettingPage::UpdateSettingL(void)
{
}

EXPORT_C CAknSetStyleListBox* CAknCheckBoxSettingPage::ListBoxControl(void) const
{
  return (CAknSetStyleListBox*)EditorControl();
}

EXPORT_C void CAknCheckBoxSettingPage::DynamicInitL(void)
{
}

void CAknCheckBoxSettingPage::UpdateAllSelections(void)
{
  TInt count=iItemArray->Count();
  CListBoxView* view=ListBoxControl()->View();
  for(TInt ii=0;ii<count;ii++)
  {
    iItemArray->At(ii)->SetSelectionStatus(view->ItemIsSelected(ii));
  }
}

void CAknCheckBoxSettingPage::SetAllSelectionsL(void)
{
  TInt count=iItemArray->Count();
  CListBoxView* view=ListBoxControl()->View();
  for(TInt ii=0;ii<count;ii++)
  {
    if(iItemArray->At(ii)->SelectionStatus())
    {
      view->SelectItemL(ii);
    }
    else
    {
      view->DeselectItem(ii);
    }
  }
}

EXPORT_C void CAknCheckBoxSettingPage::ProcessCommandL(TInt aCommandId)
{
  HideMenu();
  switch(aCommandId)
  {
    case EEikBidSelect:
      SelectCurrentItemL();
      break;
    case EEikBidOk:
    case EAknSoftkeyDone:
      AttemptExitL(ETrue);
      break;
    case EEikBidCancel:
    case EAknSoftkeyBack:
      AttemptExitL(EFalse);
      break;
    case EAknSoftkeyOptions:
      DisplayMenuL();
      break;
  }
}

EXPORT_C void CAknCheckBoxSettingPage::SelectCurrentItemL(void)
{
  ToggleSelectionL();
  UpdateSettingL();
}

void CAknCheckBoxSettingPage::GenerateInternalArrayAndGiveToListBoxL(void)
{
  _LIT(KIconStr,"1\t");
  if(iInternalItemArray) delete iInternalItemArray;
  iInternalItemArray=NULL;
  iInternalItemArray=new(ELeave)CDesCArrayFlat(1);
  TInt count=iItemArray->Count();
  for(TInt ii=0;ii<count;ii++)
  {
    TPtrC ptr=iItemArray->At(ii)->ItemText();
    TPtr des=HBufC16::NewMaxLC(ptr.Length()+KIconStr().Length())->Des();
    des.Copy(KIconStr);
    des.Append(iItemArray->At(ii)->ItemText());
    iInternalItemArray->AppendL(des);
    CleanupStack::PopAndDestroy(); //des
  }
  CTextListBoxModel* model=ListBoxControl()->Model();
  model->SetItemTextArray(iInternalItemArray);
  model->SetOwnershipType(ELbmOwnsItemArray);
  SetAllSelectionsL();
}

EXPORT_C void CAknCheckBoxSettingPage::SetSelectionItemArrayL(CSelectionItemList* aItemArray)
{
  iItemArray=aItemArray;
  GenerateInternalArrayAndGiveToListBoxL();
  CheckAndSetDataValidity();
  UpdateCbaL();
  SizeChanged();
  DrawDeferred();
}

EXPORT_C void CAknCheckBoxSettingPage::WriteInternalStateL(RWriteStream& aWriteStream) const
{
}

EXPORT_C void CAknCheckBoxSettingPage::Reserved_2(void)
{
}

EXPORT_C void CAknCheckBoxSettingPage::CAknSettingPage_Reserved_1(void)
{
}

EXPORT_C void CAknCheckBoxSettingPage::CAknSettingPage_Reserved_2(void)
{
}

EXPORT_C void CAknCheckBoxSettingPage::CAknListBoxSettingPage_Reserved_1(void)
{
}
