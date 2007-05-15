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
#include "aknsettingitem.hpp"
#include <AlmSettingsNames.hpp>
#include <barsread.h>
#include "settingsclient.hpp"

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
      iControl->StoreSettingsL();
      iClkAppUi->CmdBackL();
      break;
    case EAknSoftkeyExit:
      iControl->StoreSettingsL();
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
  if(iNaviPane) iNaviPane->Pop(iNaviDecorator);
  delete iNaviDecorator;
}

CAknSettingItem* CSettingsControl::CreateSettingItemL(TInt aSettingId)
{
  CAknSettingItem* item=NULL;
  switch(aSettingId)
  {
    case EClockAppExtraSettingAlarmTone:
      item=new(ELeave)CAknFileSettingItem(aSettingId,iAlarmTone);
      break;
    case EClockAppExtraSettingSnoozeTime:
      item=new(ELeave)CAknSliderSettingItem(aSettingId,iSnoozeTime);
      break;
    case EClockAppExtraSettingSnoozeCount:
      item=new(ELeave)CAknSliderSettingItem(aSettingId,iSnoozeCount);
      break;
    case EClockAppExtraSettingCalendarTone:
      item=new(ELeave)CAknFileSettingItem(aSettingId,iCalendarTone);
      break;
    case EClockAppExtraSettingBeepEnabled:
      item=new(ELeave)CAknVisibilitySettingItem(aSettingId,iBeep,*this,iBeepArray);
      break;
    case EClockAppExtraSettingBeepTone:
      item=new(ELeave)CAknFileSettingItem(aSettingId,iBeepTone);
      break;
    case EClockAppExtraSettingBeepStart:
      item=new(ELeave)CAknSliderSettingItem(aSettingId,iBeepStart);
      break;
    case EClockAppExtraSettingBeepFinish:
      item=new(ELeave)CAknSliderSettingItem(aSettingId,iBeepFinish);
      break;
    case EClockAppExtraSettingBirthdayEnabled:
      item=new(ELeave)CAknVisibilitySettingItem(aSettingId,iBirthday,*this,iBirthdayArray);
      break;
    case EClockAppExtraSettingBirthdayTone:
      item=new(ELeave)CAknFileSettingItem(aSettingId,iBirthdayTone);
      break;
    case EClockAppExtraSettingBirthdayStart:
      item=new(ELeave)CAknSliderSettingItem(aSettingId,iBirthdayStart);
      break;
    case EClockAppExtraSettingBirthdayHour:
      item=new(ELeave)CAknSliderSettingItem(aSettingId,iBirthdayHour);
      break;
  }
  return item;
}

void CSettingsControl::StoreSettingsL(void)
{
  CAknSettingItemList::StoreSettingsL();
  CSettingsClient& settings=static_cast<CClkAppUi*>(CCoeEnv::Static()->AppUi())->Settings();
  settings.StoreSettingL(KCategoryAlarm,KTone,iAlarmTone);
  settings.StoreSettingL(KCategoryAlarm,KSnoozeTime,iSnoozeTime);
  settings.StoreSettingL(KCategoryAlarm,KSnoozeCount,iSnoozeCount);
  settings.StoreSettingL(KCategoryCalendar,KTone,iCalendarTone);
  settings.StoreSettingL(KCategoryBeep,KEnabled,iBeep);
  settings.StoreSettingL(KCategoryBeep,KTone,iBeepTone);
  settings.StoreSettingL(KCategoryBeep,KStart,iBeepStart);
  settings.StoreSettingL(KCategoryBeep,KFinish,iBeepFinish);
  settings.StoreSettingL(KCategoryBirthday,KEnabled,iBirthday);
  settings.StoreSettingL(KCategoryBirthday,KTone,iBirthdayTone);
  settings.StoreSettingL(KCategoryBirthday,KStart,iBirthdayStart);
  settings.StoreSettingL(KCategoryBirthday,KHour,iBirthdayHour);
}

CSettingsControl::CSettingsControl(void): iSnoozeTime(5),iSnoozeCount(1),iBirthdayHour(12)
{
  const TInt sizes[]={3,1,4,4};
  for(TUint i=0;i<sizeofa(sizes);i++)
    for(TInt j=0;j<sizes[i];j++)
      iItemsArray.Append(i);
  for(TInt i=5;i<8;i++) iBeepArray.Append(i);
  for(TInt i=9;i<12;i++) iBirthdayArray.Append(i);
}

void CSettingsControl::ConstructL(const TRect& aRect)
{
  CSettingsClient& settings=static_cast<CClkAppUi*>(CCoeEnv::Static()->AppUi())->Settings();
  settings.LoadSettingL(KCategoryAlarm,KTone,iAlarmTone);
  settings.LoadSettingL(KCategoryAlarm,KSnoozeTime,iSnoozeTime,5,255,5);
  settings.LoadSettingL(KCategoryAlarm,KSnoozeCount,iSnoozeCount,1,255,6);
  settings.LoadSettingL(KCategoryCalendar,KTone,iCalendarTone);
  settings.LoadSettingL(KCategoryBeep,KEnabled,iBeep,EFalse,ETrue,EFalse);
  settings.LoadSettingL(KCategoryBeep,KTone,iBeepTone);
  settings.LoadSettingL(KCategoryBeep,KStart,iBeepStart,0,23,1);
  settings.LoadSettingL(KCategoryBeep,KFinish,iBeepFinish,0,23,0);
  settings.LoadSettingL(KCategoryBirthday,KEnabled,iBirthday,EFalse,ETrue,EFalse);
  settings.LoadSettingL(KCategoryBirthday,KTone,iBirthdayTone);
  settings.LoadSettingL(KCategoryBirthday,KStart,iBirthdayStart,0,10,0);
  settings.LoadSettingL(KCategoryBirthday,KHour,iBirthdayHour,0,23,12);
  iNaviPane=(CAknNavigationControlContainer*)iAvkonAppUi->StatusPane()->ControlL(TUid::Uid(EEikStatusPaneUidNavi));
  TResourceReader reader;
  iCoeEnv->CreateResourceReaderLC(reader,R_CLOCKAPP_EXTRA_SETTING_TABS);
  iNaviDecorator=iNaviPane->CreateTabGroupL(reader);
  CleanupStack::PopAndDestroy(); //reader
  iNaviPane->PushL(*iNaviDecorator);
  iTabGroup=static_cast<CAknTabGroup*>(iNaviDecorator->DecoratedControl());
  ConstructFromResourceL(R_CLOCKAPP_EXTRA_SETTING);
  SetRect(aRect);
  UpdateVisibilityL();
}

TKeyResponse CSettingsControl::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
{
  if(iTabGroup->OfferKeyEventL(aKeyEvent,aType)==EKeyWasConsumed)
  {
    UpdateVisibilityL();
    return EKeyWasConsumed;
  }
  return CAknSettingItemListEx::OfferKeyEventL(aKeyEvent,aType);
}

void CSettingsControl::UpdateVisibilityL(void)
{
  CListBoxView& view=*ListBox()->View();
  TBool redraw=view.RedrawDisabled();
  view.SetDisableRedraw(ETrue);
  CAknSettingItemArray& array=*SettingItemArray();
  for(TInt i=0;i<array.Count();i++)
  {
    if(iItemsArray[i]==iTabGroup->ActiveTabIndex()) array[i]->SetHidden(EFalse);
    else array[i]->SetHidden(ETrue);
  }
  for(TInt i=0,count=iTriggers.Count();i<count;i++)
  {
    CAknVisibilitySettingItem& item=*static_cast<CAknVisibilitySettingItem*>(array[iTriggers[i]]);
    if(!item.IsHidden()) item.UpdateVisibilityL();
  }
  HandleChangeInItemArrayOrVisibilityL();
  view.SetDisableRedraw(redraw);
  ListBox()->SetCurrentItemIndex(0);
  ListBox()->DrawDeferred();
}
