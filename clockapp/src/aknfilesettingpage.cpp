/*
    aknfilesettingpage.cpp
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

#include "aknfilesettingpage.hpp"
#include <akniconarray.h>
#include <clockapp.rsg>
#include <bautils.h> //BaflUtils

CAknFileSettingPage::CAknFileSettingPage(TInt aResourceID,TFileName& aFileValue): CAknSettingPage(aResourceID),iFs(CCoeEnv::Static()->FsSession()),iFileValue(aFileValue),iBackupFileValue(aFileValue)
{
}

CAknFileSettingPage::CAknFileSettingPage(const TDesC* aSettingTitleText,TInt aSettingNumber,TInt aControlType,TInt aEditorResourceId,TInt aSettingPageResourceId,TFileName& aFileValue): CAknSettingPage(aSettingTitleText,aSettingNumber,aControlType,aEditorResourceId,aSettingPageResourceId),iFs(CCoeEnv::Static()->FsSession()),iFileValue(aFileValue),iBackupFileValue(aFileValue)
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
  TParse parse;
  parse.SetNoWild(iFileValue,NULL,NULL);
  TPtrC name=parse.NameAndExt(),path=parse.DriveAndPath();
  TDesC* pos=&name;
  if(!BaflUtils::PathExists(iFs,path))
  {
    pos=NULL;
    iFileValue.Zero();
  }
  ReadFilesL(pos);
}

void CAknFileSettingPage::DynamicInitL(void)
{
  UpdateCbaL();
}

void CAknFileSettingPage::UpdateSettingL()
{
  if(iSettingPageObserver) iSettingPageObserver->HandleSettingPageEventL(this,MAknSettingPageObserver::EEventSettingChanged);
}

void CAknFileSettingPage::AcceptSettingL(void)
{
  if(iSettingPageObserver) iSettingPageObserver->HandleSettingPageEventL(this,MAknSettingPageObserver::EEventSettingOked);
}

void CAknFileSettingPage::RestoreOriginalSettingL(void)
{
  iFileValue=iBackupFileValue;
  if(iSettingPageObserver) iSettingPageObserver->HandleSettingPageEventL(this,MAknSettingPageObserver::EEventSettingCancelled);
}

void CAknFileSettingPage::UpdateCbaL(void)
{
  if(!DataValidity())
  {
    Cba()->SetCommandSetL(InvalidDataCbaResourceId());
  }
  else
  {
    Cba()->SetCommandSetL(DefaultCbaResourceId());
  }
  Cba()->DrawDeferred();
}

CAknSingleGraphicPopupMenuStyleListBox* CAknFileSettingPage::FileControl(void)
{
  return (CAknSingleGraphicPopupMenuStyleListBox*)EditorControl();
}

void CAknFileSettingPage::SizeChanged(void)
{
  StandardSettingPageLayout();
  AknLayoutUtils::LayoutControl(EditorControl(),Rect(),ELayoutEmpty,4,30,ELayoutEmpty,ELayoutEmpty,168,104);
  iOutlineFrame.LayoutRect(Rect(),0xd7,3,29,ELayoutEmpty,ELayoutEmpty,170,106);
}

void CAknFileSettingPage::Draw(const TRect &aRect) const
{
  BaseDraw(aRect);
  CWindowGc& gc=SystemGc();
  iOutlineFrame.DrawOutLineRect(gc);
}

void CAknFileSettingPage::CheckAndSetDataValidity(void)
{
  SetDataValidity(EFalse);
  TUint att;
  if(iFs.Att(iFileValue,att)==KErrNone&&(att&KEntryAttDir)==0) SetDataValidity(ETrue);
}

_LIT(KFolderIcon,"0\t");
_LIT(KMelodyIcon,"1\t");
_LIT(KFolderUp,"..");
_LIT(KMelodyMask1,"*.mid");
_LIT(KMelodyMask2,"*.wav");
_LIT(KMelodyMask3,"*.amr");
_LIT(KMelodyMask4,"*.rng");

TKeyResponse CAknFileSettingPage::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
{
  if(aType==EEventKey)
  {
    switch(aKeyEvent.iCode)
    {
      case EKeyLeftArrow:
        SetFolderL(KFolderUp);
        return EKeyWasConsumed;
      case '5':
      case EKeyRightArrow:
      case EKeyDevice3:
        {
          TPtrC item=(*iFiles)[FileControl()->CurrentItemIndex()],name=item.Mid(2);
          if(item[0]=='0')
          {
            SetFolderL(name);
          }
        }
        return EKeyWasConsumed;
    }
  }
  TInt index=FileControl()->CurrentItemIndex();
  TKeyResponse resp=CAknSettingPage::OfferKeyEventL(aKeyEvent,aType);
  if(index!=FileControl()->CurrentItemIndex()) UpdateFileL();
  return resp;
}

void CAknFileSettingPage::ReadFilesL(const TDesC* aPos)
{
  TInt pos=-1;
  iFiles->Reset();
  FileControl()->Reset();
  TFileName buffer;
  if(iFileValue.Length())
  {
    buffer.Copy(KFolderIcon);
    buffer.Append(KFolderUp);
    iFiles->AppendL(buffer);
    TParse parse;
    parse.SetNoWild(iFileValue,NULL,NULL);
    CDir* list;
    User::LeaveIfError(iFs.GetDir(parse.DriveAndPath(),KEntryAttMaskSupported,ESortByName|EDirsFirst,list));
    CleanupStack::PushL(list);
    for(TInt i=0;i<list->Count();i++)
    {
      const TDesC& name=(*list)[i].iName;
      if((*list)[i].IsDir())
      {
        buffer.Copy(KFolderIcon);
      }
      else
      {
        if(name.MatchF(KMelodyMask1)<0&&name.MatchF(KMelodyMask2)<0&&name.MatchF(KMelodyMask3)<0&&name.MatchF(KMelodyMask4)<0) continue;
        buffer.Copy(KMelodyIcon);
      }
      buffer.Append(name);
      if(aPos&&name.MatchF(*aPos)>=0) pos=iFiles->Count();
      iFiles->AppendL(buffer);
    }
    CleanupStack::PopAndDestroy(); //list
  }
  else
  {
    TDriveList drives;
    User::LeaveIfError(iFs.DriveList(drives));
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
  FileControl()->HandleItemAdditionL();
  if(pos!=-1) FileControl()->SetCurrentItemIndexAndDraw(pos);
  UpdateFileL();
}

void CAknFileSettingPage::SetFolderL(const TDesC& aFolder)
{
  TParse parse; TFileName file; TDesC* pos=NULL;
  parse.SetNoWild(iFileValue,NULL,NULL);
  if(aFolder.CompareF(KFolderUp)==0)
  {
    if(iFileValue.Length())
    {
      if(parse.PopDir()==KErrNone)
      {
        TParse parse2;
        parse2.SetNoWild(iFileValue,NULL,NULL);
        TPtrC ptr=parse2.DriveAndPath();
        file=ptr.Left(ptr.Length()-1);
        parse2.SetNoWild(file,NULL,NULL);
        file=parse2.NameAndExt();
        pos=&file;
        iFileValue=parse.DriveAndPath();
      }
      else
        iFileValue.Zero();
    }
  }
  else
  {
    iFileValue=parse.DriveAndPath();
    iFileValue.Append(aFolder);
    iFileValue.Append('\\');
  }
  ReadFilesL(pos);
}

void CAknFileSettingPage::UpdateFileL(void)
{
  if(iFileValue.Length())
  {
    TPtrC item=(*iFiles)[FileControl()->CurrentItemIndex()],name=item.Mid(2);
    TParse parse;
    parse.SetNoWild(iFileValue,NULL,NULL);
    iFileValue=parse.DriveAndPath();
    iFileValue.Append(name);
  }
  CheckAndSetDataValidity();
  UpdateCbaL();
}
