/*
    clockappui.cpp
    Copyright (C) 2006-2007 zg

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

#include "clockapp.hpp"
#include "settingsex.hpp"
#include "stopwatch.hpp"
#include "locale.hpp"
#include <hlplch.h>
#include <clockapp.rsg>

CClkAppUi::~CClkAppUi()
{
  delete iEnvNotifier;
}

void CClkAppUi::ConstructL(void)
{
  BaseConstructL();
  //need here to load original resources
  CClkDateTimeView* view2=CClkDateTimeView::NewLC(KViewId,ETrue); //FIXME

  CClkAlmModel* alm=iClockDocument->Alm();
  CClkNitzModel* nitz=iClockDocument->Nitz();
  CClkAlmView* view1=CClkAlmView::NewLC(alm,nitz);
  AddViewL(view1);
  CleanupStack::Pop(); //view1
//FIXME
//  CClkDateTimeView* view2=CClkDateTimeView::NewLC(KViewId,ETrue);
  AddViewL(view2);
  CleanupStack::Pop(); //view2
  CSettingsView* view3=CSettingsView::NewLC();
  AddViewL(view3);
  CleanupStack::Pop(); //view3
  CStopWatchView* view4=CStopWatchView::NewLC();
  AddViewL(view4);
  CleanupStack::Pop(); //view4
  CLocaleView* view5=CLocaleView::NewLC();
  AddViewL(view5);
  CleanupStack::Pop(); //view5

  CEikStatusPane* sp=StatusPane();
  iTitlePane=(CAknTitlePane*)sp->ControlL(TUid::Uid(EEikStatusPaneUidTitle));
  iNaviPane=(CAknNavigationControlContainer*)sp->ControlL(TUid::Uid(EEikStatusPaneUidNavi));
  alm->Start();
  nitz->Start();
  iEnvNotifier=CEnvironmentChangeNotifier::NewL(CActive::EPriorityIdle,TCallBack(DoNotiferCallbackL,this));
  iEnvNotifier->Start();
}

void CClkAppUi::NotifyEnvChangedL(TClkEnvChanged anEvent)
{
  ((CClkAlmView*)View(KClkAlmViewId))->HandleSettingsChangeL(anEvent);
}

void CClkAppUi::CmdSettingsL(void)
{
  ShowTitlePaneL(0x10931015);
  ActivateLocalViewL(KSettingsId);
}

void CClkAppUi::CmdHelpL(void)
{
  CArrayFix<TCoeHelpContext>* context=AppHelpContextL();
  HlpLauncher::LaunchHelpApplicationL(iCoeEnv->WsSession(),context);
}

void CClkAppUi::CmdExit(void)
{
  Exit();
}

void CClkAppUi::ShowTitlePaneL(TInt aResId)
{
  TBuf<64> title;
  iCoeEnv->ReadResourceAsDes16(title,aResId);
  iTitlePane->SetTextL(title);
  iTitlePane->DrawNow();
}

void CClkAppUi::HandleCommandL(TInt aCommand)
{
  switch(aCommand)
  {
    case EEikCmdExit:
    case EAknCmdExit:
      Exit();
      break;
  }
}

TInt CClkAppUi::DoNotiferCallbackL(TAny* anAppUi)
{
  CClkAppUi* appUi=(CClkAppUi*)anAppUi;
  TInt change=appUi->iEnvNotifier->Change();
  if(change<=0x7e&&(change&(EChangesLocale|EChangesMidnightCrossover|EChangesSystemTime)))
  {
    appUi->NotifyEnvChangedL(EClkEnvChanged3);
  }
  return KErrNone;
}

void CClkAppUi::CmdBackL(void)
{
  ActivateLocalViewL(KClkAlmViewId);
}

void CClkAppUi::CmdSettingsExL(void)
{
  ShowTitlePaneL(R_CLOCKAPP_EXTRA_SETTING_TITLE);
  ActivateLocalViewL(KSettingsExViewId);
}

void CClkAppUi::CmdStopWatchL(void)
{
  ShowTitlePaneL(R_CLOCKAPP_STOP_WATCH_TITLE);
  ActivateLocalViewL(KStopWatchViewId);
}

void CClkAppUi::CmdLocaleL(void)
{
  ShowTitlePaneL(R_CLOCKAPP_LOCALE_TITLE);
  ActivateLocalViewL(KLocaleViewId);
}

void CClkAppUi::CmdCompactDBL(void)
{
  CSettingsControl::CompactL();
}
