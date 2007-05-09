/*
    ntpview.cpp
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

#include "ntpview.hpp"
#include <clockapp.rsg>
#include "clockapp.hpp"
#include "clockapp.hrh"
#include "ntp.hpp"
#include "aknsettingitem.hpp"
#include "settingsclient.hpp"

_LIT(KDefaultNtpServer,"time.windows.com");
//_LIT(KDefaultNtpServer,"time-a.nist.gov");
_LIT(KCategorySynchro,"Synchronization");
_LIT(KServer,"Server");
_LIT(KPort,"Port");
_LIT(KCorrection,"Correction");

CNtpView* CNtpView::NewLC(void)
{
  CNtpView* self=new(ELeave)CNtpView;
  CleanupStack::PushL(self);
  self->ConstructL();
  return self;
}

CNtpView::~CNtpView()
{
  delete iControl;
}

CNtpView::CNtpView(): CAknView(),iServer(KDefaultNtpServer),iPort(123),iCorrection(0)
{
}

void CNtpView::ConstructL(void)
{
  BaseConstructL(R_CLOCKAPP_INTERNET_TIME_VIEW);
}

TUid CNtpView::Id(void) const
{
  return KInternetTimeViewId;
}

void CNtpView::HandleCommandL(TInt aCommand)
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
    case EClockAppExtraSynchronize:
      iControl->StoreSettingsL();
      CNtp::NewLD(iServer,iPort,iCorrection);
      break;
  }
}

void CNtpView::HandleClientRectChange(void)
{
  if(iControl) iControl->SetRect(ClientRect());
}

void CNtpView::DoActivateL(const TVwsViewId& aPrevViewId,TUid aCustomMessageId,const TDesC8& aCustomMessage)
{
  if(!iControl)
  {
    iControl=CNtpControl::NewL(ClientRect(),iServer,iPort,iCorrection);
    iControl->SetMopParent(this);
    iClkAppUi->AddToStackL(*this,iControl);
  }
  iControl->ActivateL();
}

void CNtpView::DoDeactivate(void)
{
  if(iControl)
  {
    iClkAppUi->RemoveFromStack(iControl);
    delete iControl;
    iControl=NULL;
  }
}

CNtpControl* CNtpControl::NewL(const TRect& aRect,TFileName& aServer,TInt& aPort,TTimeIntervalSeconds& aCorrection)
{
  CNtpControl* self=new(ELeave)CNtpControl(aServer,aPort,aCorrection);
  CleanupStack::PushL(self);
  self->ConstructL(aRect);
  CleanupStack::Pop(); //self
  return(self);
}

CNtpControl::~CNtpControl()
{
}

CAknSettingItem* CNtpControl::CreateSettingItemL(TInt aSettingId)
{
  CAknSettingItem* item=NULL;
  switch(aSettingId)
  {
    case EClockAppInternetTimeServer:
      item=new(ELeave)CAknTextSettingItem(aSettingId,iServer);
      break;
    case EClockAppInternetTimePort:
      item=new(ELeave)CAknTextSettingItem(aSettingId,iPortStr);
      break;
    case EClockAppInternetTimeCorrection:
      item=new(ELeave)CAknTimeOffsetSettingItem(aSettingId,iCorrection);
      break;
  }
  return item;
}

void CNtpControl::StoreSettingsL(void)
{
  CAknSettingItemList::StoreSettingsL();
  TLex value(iPortStr);
  value.Val(iPort);
  CSettingsClient& settings=static_cast<CClkAppUi*>(CCoeEnv::Static()->AppUi())->Settings();
  settings.StoreSettingL(KCategorySynchro,KServer,iServer);
  settings.StoreSettingL(KCategorySynchro,KPort,iPort);
  settings.StoreSettingL(KCategorySynchro,KCorrection,iCorrection.Int());
}

CNtpControl::CNtpControl(TFileName& aServer,TInt& aPort,TTimeIntervalSeconds& aCorrection): iServer(aServer),iPort(aPort),iCorrection(aCorrection)
{
}

void CNtpControl::ConstructL(const TRect& aRect)
{
  CSettingsClient& settings=static_cast<CClkAppUi*>(CCoeEnv::Static()->AppUi())->Settings();
  settings.LoadSettingL(KCategorySynchro,KServer,iServer);
  settings.LoadSettingL(KCategorySynchro,KPort,iPort,0,65536,iPort);
  iPortStr.Format(_L("%d"),iPort);
  TInt correction;
  settings.LoadSettingL(KCategorySynchro,KCorrection,correction,-43200,43200,0);
  iCorrection=correction;
  ConstructFromResourceL(R_CLOCKAPP_INTERNET_TIME);
  SetRect(aRect);
}
