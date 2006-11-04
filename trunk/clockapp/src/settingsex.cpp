/*
    settingsex.cpp
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

#include "settingsex.hpp"
#include "clockapp.hpp"
#include "clockapp.hrh"
#include <clockapp.rsg>
#include <akntextsettingpage.h>
#include <akniconarray.h>

CSettingsView* CSettingsView::NewLC(void)
{
  CSettingsView* self=new(ELeave)CSettingsView;
  CleanupStack::PushL(self);
  self->ConstructL();
  return self;
}

CSettingsView::~CSettingsView()
{
  delete iControl;
}

void CSettingsView::ConstructL(void)
{
  BaseConstructL(R_CLOCKAPP_EXTRA_SETTING_VIEW);
}

TUid CSettingsView::Id(void) const
{
  return KSettingsExViewId;
}

void CSettingsView::HandleCommandL(TInt aCommand)
{
  switch(aCommand)
  {
    case EAknSoftkeyBack:
      iClkAppUi->CmdBackL();
      break;
    case EAknSoftkeyExit:
      iClkAppUi->CmdExit();
      break;
    case EClockAppExtraChange:
      iControl->EditItemL(iControl->ListBox()->CurrentItemIndex(),ETrue);
      break;
  }
}

void CSettingsView::HandleClientRectChange(void)
{
  if(iControl) iControl->SetRect(ClientRect());
}

void CSettingsView::DoActivateL(const TVwsViewId& aPrevViewId,TUid aCustomMessageId,const TDesC8& aCustomMessage)
{
  if(!iControl)
  {
    iControl=CSettingsControl::NewL(ClientRect());
    iControl->SetMopParent(this);
    iClkAppUi->AddToStackL(*this,iControl);
  }
  iControl->ActivateL();
}

void CSettingsView::DoDeactivate(void)
{
  if(iControl)
  {
    iClkAppUi->RemoveFromStack(iControl);
    delete iControl;
    iControl=NULL;
  }
}

CSettingsControl* CSettingsControl::NewL(const TRect& aRect)
{
  CSettingsControl* self=new(ELeave)CSettingsControl;
  CleanupStack::PushL(self);
  self->ConstructL(aRect);
  CleanupStack::Pop(); //self
  return(self);
}

CSettingsControl::~CSettingsControl()
{
}

CAknSettingItem* CSettingsControl::CreateSettingItemL(TInt aSettingId)
{
  CAknSettingItem* item=NULL;
  switch(aSettingId)
  {
    case EClockAppExtraSettingAlarmTone:
      item=new(ELeave)CAknFileSettingItem(aSettingId,iAlarmTone);
      break;
  }
  return item;
}

void CSettingsControl::HandleListBoxEventL(CEikListBox* aListBox,TListBoxEvent aEventType)
{
  CAknSettingItemList::HandleListBoxEventL(aListBox,aEventType);
}

CSettingsControl::CSettingsControl(void)
{
  iAlarmTone.Copy(_L("z:\\Nokia\\Sounds\\Digital\\"));
}

void CSettingsControl::ConstructL(const TRect& aRect)
{
  ConstructFromResourceL(R_CLOCKAPP_EXTRA_SETTING);
  SetRect(aRect);
}

inline TInt CAknSettingItem::EditorControlType() const
{
  return iEditorControlType;
}

CAknFileSettingItem::CAknFileSettingItem(TInt aIdentifier,TFileName& aText): CAknSettingItem(aIdentifier),iExternalText(aText),iInternalText(aText)
{
}

CAknFileSettingItem::~CAknFileSettingItem()
{
}

void CAknFileSettingItem::StoreL(void)
{
  iExternalText=iInternalText;
}

void CAknFileSettingItem::LoadL(void)
{
  iInternalText=iExternalText;
}

const TDesC& CAknFileSettingItem::SettingTextL(void)
{
  if(iInternalText.Length()) return iInternalText;
  return EmptyItemText();
}

void CAknFileSettingItem::EditItemL(TBool aCalledFromMenu)
{
  TPtrC name=SettingName();
  SetSettingPage(new(ELeave)CAknFileSettingPage(&name,SettingNumber(),EditorControlType(),SettingEditorResourceId(),SettingPageResourceId(),iInternalText));
  SettingPage()->SetSettingPageObserver(this);
  SettingPage()->ExecuteLD(CAknSettingPage::EUpdateWhenChanged);
  SetSettingPage(NULL);
}

CAknFileSettingPage::CAknFileSettingPage(TInt aResourceID,TFileName& aFileValue): CAknSettingPage(aResourceID),iFileValue(aFileValue),iBackupFileValue(aFileValue)
{
}

CAknFileSettingPage::CAknFileSettingPage(const TDesC* aSettingTitleText,TInt aSettingNumber,TInt aControlType,TInt aEditorResourceId,TInt aSettingPageResourceId,TFileName& aFileValue): CAknSettingPage(aSettingTitleText,aSettingNumber,aControlType,aEditorResourceId,aSettingPageResourceId),iFileValue(aFileValue),iBackupFileValue(aFileValue)
{
}

CAknFileSettingPage::~CAknFileSettingPage()
{
}

void CAknFileSettingPage::ConstructL(void)
{
  BaseConstructL();
  //listbox icons
  CAknIconArray* icons=new(ELeave)CAknIconArray(2);
  CleanupStack::PushL(icons);
  icons->ConstructFromResourceL(R_CLOCKAPP_EXTRA_SETTING_ICONS);
  FileControl()->ItemDrawer()->ColumnData()->SetIconArray(icons);
  CleanupStack::Pop(); //icons
  //listbox items
  iFiles=new(ELeave)CDesCArrayFlat(32);
  CTextListBoxModel* model=FileControl()->Model();
  model->SetItemTextArray(iFiles); //old text array deleted by owner
  model->SetOwnershipType(ELbmOwnsItemArray); //transfer ownership
  ReadFilesL();
  /*iFiles->AppendL(_L("1\taaaaa"));
  iFiles->AppendL(_L("0\taaaaa"));
  iFiles->AppendL(_L("1\tbbbbb"));
  FileControl()->HandleItemAdditionL();*/
  //SliderControl()->SetValueL(iSliderValue);
}

void CAknFileSettingPage::UpdateSettingL()
{
  //iFileValue=SliderControl()->Value();
  if(iSettingPageObserver) iSettingPageObserver->HandleSettingPageEventL(this,MAknSettingPageObserver::EEventSettingChanged);
}

void CAknFileSettingPage::AcceptSettingL(void)
{
  //iFileValue=SliderControl()->Value();
  if(iSettingPageObserver) iSettingPageObserver->HandleSettingPageEventL(this,MAknSettingPageObserver::EEventSettingChanged);
}

void CAknFileSettingPage::RestoreOriginalSettingL(void)
{
  iFileValue=iBackupFileValue;
}

CAknSingleGraphicPopupMenuStyleListBox* CAknFileSettingPage::FileControl(void)
{
  return (CAknSingleGraphicPopupMenuStyleListBox*)EditorControl();
}

void CAknFileSettingPage::SizeChanged(void)
{
  StandardSettingPageLayout();
  AknLayoutUtils::LayoutControl(EditorControl(),Rect(),ELayoutEmpty,4,29,ELayoutEmpty,ELayoutEmpty,168,106);
}

void CAknFileSettingPage::Draw(const TRect &aRect) const
{
  BaseDraw(aRect);
}

void CAknFileSettingPage::CheckAndSetDataValidity(void)
{
  User::Panic(_L("zg"),666);
  SetDataValidity(EFalse);
}

_LIT(KFolderIcon,"0\t");
_LIT(KMelodyIcon,"1\t");
_LIT(KFolderUp,"..");

TKeyResponse CAknFileSettingPage::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
{
  if(aType==EEventKey)
  {
    switch(aKeyEvent.iCode)
    {
      case EKeyLeftArrow:
        SetFolderL(KFolderUp);
        ReadFilesL();
        return EKeyWasConsumed;
      case '5':
      case EKeyRightArrow:
      case EKeyDevice3:
        {
          TPtrC item=(*iFiles)[FileControl()->CurrentItemIndex()],name=item.Mid(2);
          if(item[0]=='0')
          {
            SetFolderL(name);
            ReadFilesL();
          }
        }
        return EKeyWasConsumed;
    }
  }
  return CAknSettingPage::OfferKeyEventL(aKeyEvent,aType);
}

void CAknFileSettingPage::ReadFilesL(void)
{
  iFiles->Reset();
  FileControl()->Reset();
  TFileName buffer;
  RFs fs;
  User::LeaveIfError(fs.Connect());
  CleanupClosePushL(fs);
  if(iFileValue.Length())
  {
    buffer.Copy(KFolderIcon);
    buffer.Append(KFolderUp);
    iFiles->AppendL(buffer);
    TParse parse;
    parse.SetNoWild(iFileValue,NULL,NULL);
    CDir* list;
    User::LeaveIfError(fs.GetDir(parse.DriveAndPath(),KEntryAttMaskSupported,ESortByName|EDirsFirst,list));
    CleanupStack::PushL(list);
    for(TInt i=0;i<list->Count();i++)
    {
      if((*list)[i].IsDir())
      {
        buffer.Copy(KFolderIcon);
      }
      else
      {
        buffer.Copy(KMelodyIcon);
      }
      buffer.Append((*list)[i].iName);
      iFiles->AppendL(buffer);
    }
    CleanupStack::PopAndDestroy(); //list
  }
  else
  {
    TDriveList drives;
    User::LeaveIfError(fs.DriveList(drives));
    for(TInt drive=EDriveA;drive<=EDriveZ;drive++)
    {
      if(drives[drive])
      {
        buffer.Copy(KFolderIcon);
        buffer.Append(drive+'a');
        buffer.Append(':');
        iFiles->AppendL(buffer);
      }
    }
  }
  CleanupStack::PopAndDestroy(); //fs
  FileControl()->HandleItemAdditionL();
}

void CAknFileSettingPage::SetFolderL(const TDesC& aFolder)
{
  TParse parse;
  parse.SetNoWild(iFileValue,NULL,NULL);
  if(aFolder.CompareF(KFolderUp)==0)
  {
    if(parse.PopDir()==KErrNone)
      iFileValue=parse.DriveAndPath();
    else
      iFileValue.Zero();
  }
  else
  {
    iFileValue=parse.DriveAndPath();
    iFileValue.Append(aFolder);
    iFileValue.Append('\\');
  }
}

void CAknFileSettingPage::UpdateFileL(void)
{
  TPtrC item=(*iFiles)[FileControl()->CurrentItemIndex()],name=item.Mid(2);
  TParse parse;
  parse.SetNoWild(iFileValue,NULL,NULL);
  iFileValue=parse.DriveAndPath();
  iFileValue.Append(name);
}
