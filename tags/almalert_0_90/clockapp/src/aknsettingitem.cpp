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
#include "akntimeoffsetsettingpage.hpp"

inline TInt CAknSettingItem::EditorControlType() const
{
  return iEditorControlType;
}

CAknFileSettingItem::CAknFileSettingItem(TInt aIdentifier,TFileName& aText): CAknSettingItem(aIdentifier),iExternalText(aText)
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

CAknTimeOffsetSettingItem::CAknTimeOffsetSettingItem(TInt aIdentifier,TTimeIntervalSeconds& aInterval): CAknSettingItem(aIdentifier),iExternalInterval(aInterval),iInternalTextPtr(NULL,0),iTimeFormatPtr(NULL,0)
{
}

CAknTimeOffsetSettingItem::~CAknTimeOffsetSettingItem()
{
  delete iTimeFormat;
  delete iInternalText;
}

void CAknTimeOffsetSettingItem::CompleteConstructionL(void)
{
  iInternalText=HBufC16::NewL(30);
  delete iTimeFormat;
  iTimeFormat=CCoeEnv::Static()->AllocReadResourceAsDes16L(R_QTN_TIME_LONG);
  iTimeFormatPtr.Set(iTimeFormat->Des());
}

void CAknTimeOffsetSettingItem::StoreL(void)
{
  iExternalInterval=iInternalInterval;
}

void CAknTimeOffsetSettingItem::LoadL(void)
{
  iInternalInterval=iExternalInterval;
}

const TDesC& CAknTimeOffsetSettingItem::SettingTextL(void)
{
  iInternalTextPtr.Set(iInternalText->Des());
  iInternalTextPtr.Zero();
  TInt interval=iInternalInterval.Int();
  if(interval<0)
  {
    interval=-interval;
    iInternalTextPtr.Append('-');
  }
  else if(interval>0)
    iInternalTextPtr.Append('+');
  else
    iInternalTextPtr.Append(' ');
  TTime time=Convert(interval);
  TBuf<29> buffer;
  time.FormatL(buffer,TimeFormatString());
  iInternalTextPtr.Append(buffer);
  return iInternalTextPtr;
}

void CAknTimeOffsetSettingItem::SetTimeFormatStringL(const TDesC& aTimeFormat)
{
  delete iTimeFormat;
  iTimeFormat=NULL;
  iTimeFormat=aTimeFormat.AllocL();
  iTimeFormatPtr.Set(iTimeFormat->Des());
}

const TPtrC CAknTimeOffsetSettingItem::TimeFormatString(void)
{
  return iTimeFormatPtr;
}

void CAknTimeOffsetSettingItem::EditItemL(TBool aCalledFromMenu)
{
  TPtrC name=SettingName();
  SetSettingPage(new(ELeave)CAknTimeOffsetSettingPage(&name,SettingNumber(),EditorControlType(),SettingEditorResourceId(),SettingPageResourceId(),iInternalInterval));
  SettingPage()->SetSettingPageObserver(this);
  SettingPage()->ExecuteLD(CAknSettingPage::EUpdateWhenChanged);
  SetSettingPage(NULL);
}

TTime CAknTimeOffsetSettingItem::Convert(TInt aInterval)
{
  TInt second=aInterval%60;
  aInterval/=60;
  TInt minute=aInterval%60;
  aInterval/=60;
  TInt hour=aInterval%24;
  return TTime(TDateTime(0,EJanuary,0,hour,minute,second,0));
}

CAknWorkDaysSettingItem::CAknWorkDaysSettingItem(TInt aIdentifier,TUint& aWorkDays): CAknSettingItem(aIdentifier),iExternalWorkDays(aWorkDays)
{
  iCoeEnv=CCoeEnv::Static();
}

CAknWorkDaysSettingItem::~CAknWorkDaysSettingItem()
{
  if(iList)
  {
    iList->ResetAndDestroy();
    delete iList;
  }
}

void CAknWorkDaysSettingItem::CompleteConstructionL(void)
{
  iList=new(ELeave)CSelectionItemList(7);
  for(TInt i=0;i<7;i++)
  {
    HBufC* str=iCoeEnv->AllocReadResourceAsDes16LC(R_QTN_WEEK_LONG_MONDAY+i);
    CSelectableItem* item=new(ELeave)CSelectableItem(*str,EFalse);
    CleanupStack::PushL(item);
    item->ConstructL();
    iList->AppendL(item);
    CleanupStack::Pop(); //item
    CleanupStack::PopAndDestroy(); //str
  }
}

void CAknWorkDaysSettingItem::StoreL(void)
{
  iExternalWorkDays=0;
  for(TInt i=0;i<iList->Count();i++)
  {
    if((*iList)[i]->SelectionStatus()) iExternalWorkDays|=(1<<i);
  }
}

void CAknWorkDaysSettingItem::LoadL(void)
{
  for(TInt i=0;i<iList->Count();i++)
  {
    (*iList)[i]->SetSelectionStatus(iExternalWorkDays&(1<<i));
  }
}

const TDesC& CAknWorkDaysSettingItem::SettingTextL(void)
{
  TBuf<16> name;
  iInternalText.Zero();
  for(TInt i=0;i<7;i++)
  {
    if(iExternalWorkDays&(1<<i))
    {
      if(iInternalText.Length()) iInternalText.Append(' ');
      iCoeEnv->ReadResourceAsDes16(name,R_QTN_WEEK_TWO_CHARS_MO+i);
      iInternalText.Append(name);
    }
  }
  return iInternalText;
}

void CAknWorkDaysSettingItem::EditItemL(TBool aCalledFromMenu)
{
  TPtrC name=SettingName();
  SetSettingPage(new(ELeave)CAknCheckBoxSettingPage(&name,SettingNumber(),EditorControlType(),SettingEditorResourceId(),SettingPageResourceId(),iList));
  SettingPage()->SetSettingPageObserver(this);
  if(SettingPage()->ExecuteLD(CAknSettingPage::EUpdateWhenChanged)) StoreL();
  else LoadL();
  UpdateListBoxTextL();
  SetSettingPage(NULL);
}
