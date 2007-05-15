/*
    avkon_CAknTimeOrDateSettingItem.cpp
    Copyright (C) 2007 zg

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

#include <aknsettingitemlist.h>
#include <aknsettingitemlist.pan>
#include <aknmfnesettingpage.h>

EXPORT_C CAknTimeOrDateSettingItem::CAknTimeOrDateSettingItem(TInt aIdentifier,TAknTimeOrDateSettingItemMode aMode,TTime& aTime): CAknSettingItem(aIdentifier),iExternalTime(aTime),iInternalTextPtr(NULL,0),iTimeFormatPtr(NULL,0),iTimeOrDateMode(aMode)
{
}

EXPORT_C CAknTimeOrDateSettingItem::~CAknTimeOrDateSettingItem()
{
  delete iTimeFormat;
  delete iInternalText;
}

EXPORT_C void CAknTimeOrDateSettingItem::CompleteConstructionL(void)
{
  iInternalText=HBufC16::NewL(30);
  delete iTimeFormat;
  switch(iTimeOrDateMode)
  {
    case ETime:
      iTimeFormat=CCoeEnv::Static()->AllocReadResourceAsDes16L(R_QTN_TIME_USUAL_WITH_ZERO);
      break;
    case EDate:
      iTimeFormat=CCoeEnv::Static()->AllocReadResourceAsDes16L(R_QTN_DATE_USUAL_WITH_ZERO);
      break;
    default:
      Panic(EAknSettingItemListInvatidDateOrTimeMode);
      break;
  }
  iTimeFormatPtr.Set(iTimeFormat->Des());
}

EXPORT_C void CAknTimeOrDateSettingItem::StoreL(void)
{
  iExternalTime=iInternalTime;
}

EXPORT_C void CAknTimeOrDateSettingItem::LoadL(void)
{
  iInternalTime=iExternalTime;
}

EXPORT_C const TDesC& CAknTimeOrDateSettingItem::SettingTextL(void)
{
  iInternalTextPtr.Set(iInternalText->Des());
  iInternalTime.FormatL(iInternalTextPtr,TimeFormatString());
  return iInternalTextPtr;
}

EXPORT_C void CAknTimeOrDateSettingItem::SetTimeFormatStringL(const TDesC& aTimeFormat)
{
  delete iTimeFormat;
  iTimeFormat=NULL;
  iTimeFormat=aTimeFormat.AllocL();
  iTimeFormatPtr.Set(iTimeFormat->Des());
}

EXPORT_C const TPtrC CAknTimeOrDateSettingItem::TimeFormatString(void)
{
  return iTimeFormatPtr;
}

EXPORT_C void CAknTimeOrDateSettingItem::EditItemL(TBool aCalledFromMenu)
{
  TPtrC name=SettingName();
  CAknSettingPage* page=NULL;
  switch(iTimeOrDateMode)
  {
    case ETime:
      page=new(ELeave)CAknTimeSettingPage(&name,SettingNumber(),EditorControlType(),SettingEditorResourceId(),SettingPageResourceId(),iInternalTime);
      break;
    case EDate:
      page=new(ELeave)CAknDateSettingPage(&name,SettingNumber(),EditorControlType(),SettingEditorResourceId(),SettingPageResourceId(),iInternalTime);
      break;
    default:
      Panic(EAknSettingItemListInvatidDateOrTimeMode);
      break;
  }
  SetSettingPage(page);
  SettingPage()->SetSettingPageObserver(this);
  SettingPage()->ExecuteLD(CAknSettingPage::EUpdateWhenChanged);
  SetSettingPage(NULL);
}

EXPORT_C void CAknTimeOrDateSettingItem::CAknSettingItem_Reserved(void)
{
}
