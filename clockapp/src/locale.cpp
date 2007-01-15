/*
    locale.cpp
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

#include "locale.hpp"
#include "clockapp.hpp"
#include "clockapp.hrh"
#include <clockapp.rsg>
#include "aknsettingitem.hpp"

CLocaleView* CLocaleView::NewLC(void)
{
  CLocaleView* self=new(ELeave)CLocaleView;
  CleanupStack::PushL(self);
  self->ConstructL();
  return self;
}

CLocaleView::~CLocaleView()
{
  delete iControl;
}

void CLocaleView::ConstructL(void)
{
  BaseConstructL(R_CLOCKAPP_EXTRA_SETTING_VIEW);
}

TUid CLocaleView::Id(void) const
{
  return KLocaleViewId;
}

void CLocaleView::HandleCommandL(TInt aCommand)
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

void CLocaleView::HandleClientRectChange(void)
{
  if(iControl) iControl->SetRect(ClientRect());
}

void CLocaleView::DoActivateL(const TVwsViewId& aPrevViewId,TUid aCustomMessageId,const TDesC8& aCustomMessage)
{
  if(!iControl)
  {
    iControl=CLocaleControl::NewL(ClientRect());
    iControl->SetMopParent(this);
    iClkAppUi->AddToStackL(*this,iControl);
  }
  iControl->ActivateL();
}

void CLocaleView::DoDeactivate(void)
{
  if(iControl)
  {
    iClkAppUi->RemoveFromStack(iControl);
    delete iControl;
    iControl=NULL;
  }
}

CLocaleControl* CLocaleControl::NewL(const TRect& aRect)
{
  CLocaleControl* self=new(ELeave)CLocaleControl;
  CleanupStack::PushL(self);
  self->ConstructL(aRect);
  CleanupStack::Pop(); //self
  return(self);
}

CLocaleControl::~CLocaleControl()
{
}

CAknSettingItem* CLocaleControl::CreateSettingItemL(TInt aSettingId)
{
  CAknSettingItem* item=NULL;
  switch(aSettingId)
  {
    case EClockAppLocaleTimeOffset:
      item=new(ELeave)CAknTimeOffsetSettingItem(aSettingId,iTimeOffset);
  }
  return item;
}

void CLocaleControl::StoreSettingsL(void)
{
  CAknSettingItemList::StoreSettingsL();
  TLocale locale;
  locale.SetUniversalTimeOffset(iTimeOffset);
  locale.Set();
}

CLocaleControl::CLocaleControl(void)
{
}

void CLocaleControl::ConstructL(const TRect& aRect)
{
  TLocale locale;
  iTimeOffset=locale.UniversalTimeOffset();
  ConstructFromResourceL(R_CLOCKAPP_LOCALE);
  SetRect(aRect);
}
