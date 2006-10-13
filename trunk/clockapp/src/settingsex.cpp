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
      item=new(ELeave)CAknTextSettingItem(aSettingId,iAlarmTone);
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
}

void CSettingsControl::ConstructL(const TRect& aRect)
{
  ConstructFromResourceL(R_CLOCKAPP_EXTRA_SETTING);
  SetRect(aRect);
}
