/*
    aknsettingitem.cpp
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

#include "aknsettingitem.hpp"
#include "aknfilesettingpage.hpp"

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

CAknVisibilitySettingItem::CAknVisibilitySettingItem(TInt aIdentifier,TBool& aBinaryValue,CAknSettingItemListEx& aList,RArray<TInt>& anItems): CAknBinaryPopupSettingItem(aIdentifier,aBinaryValue),iList(aList),iItems(anItems)
{
  iList.AddTrigger(iList.SettingItemArray()->Count());
}

void CAknVisibilitySettingItem::EditItemL(TBool aCalledFromMenu)
{
  CAknBinaryPopupSettingItem::EditItemL(EFalse);
  UpdateVisibilityL();
}

void CAknVisibilitySettingItem::UpdateVisibilityL(void)
{
  CAknSettingItemArray& array=*iList.SettingItemArray();
  for(TInt i=0,count=iItems.Count(),value=InternalValue();i<count;i++) array[iItems[i]]->SetHidden(!value);
  iList.HandleChangeInItemArrayOrVisibilityL();
}
