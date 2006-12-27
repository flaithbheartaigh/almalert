/*
    stopwatch.cpp
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

#include "stopwatch.hpp"
#include <clockapp.rsg>
#include "clockapp.hpp"
#include "clockapp.hrh"

CStopWatchView* CStopWatchView::NewLC(void)
{
  CStopWatchView* self=new(ELeave)CStopWatchView;
  CleanupStack::PushL(self);
  self->ConstructL();
  return self;
}

CStopWatchView::~CStopWatchView()
{
  delete iControl;
}

void CStopWatchView::ConstructL(void)
{
  BaseConstructL(R_CLOCKAPP_STOP_WATCH_VIEW);
}

TUid CStopWatchView::Id(void) const
{
  return KStopWatchViewId;
}

void CStopWatchView::HandleCommandL(TInt aCommand)
{
  switch(aCommand)
  {
    case EAknSoftkeyBack:
      iClkAppUi->CmdBackL();
      break;
    case EAknSoftkeyExit:
      iClkAppUi->CmdExit();
      break;
    case EClockAppExtraStopWatchStartStop:
      break;
    case EClockAppExtraStopWatchLapReset:
      break;
  }
}

void CStopWatchView::HandleClientRectChange(void)
{
  if(iControl) iControl->SetRect(ClientRect());
}

void CStopWatchView::DoActivateL(const TVwsViewId& aPrevViewId,TUid aCustomMessageId,const TDesC8& aCustomMessage)
{
  if(!iControl)
  {
    iControl=CStopWatchControl::NewL(ClientRect());
    iControl->SetMopParent(this);
    iClkAppUi->AddToStackL(*this,iControl);
  }
  iControl->ActivateL();
}

void CStopWatchView::DoDeactivate(void)
{
  if(iControl)
  {
    iClkAppUi->RemoveFromStack(iControl);
    delete iControl;
    iControl=NULL;
  }
}

CStopWatchControl* CStopWatchControl::NewL(const TRect& aRect)
{
  CStopWatchControl* self=new(ELeave)CStopWatchControl;
  CleanupStack::PushL(self);
  self->ConstructL(aRect);
  CleanupStack::Pop(); //self
  return(self);
}

CStopWatchControl::~CStopWatchControl()
{
}

CStopWatchControl::CStopWatchControl(void)
{
}

void CStopWatchControl::ConstructL(const TRect& aRect)
{
  CreateWindowL();
  SetRect(aRect);
  DrawNow(); //FIXME: needed?
  ActivateL();
}

void CStopWatchControl::HandleGainingForeground(void)
{
  //FIXME
}

void CStopWatchControl::HandleLosingForeground(void)
{
  //FIXME
}

void CStopWatchControl::Draw(const TRect& aRect) const
{
  CWindowGc& gc=SystemGc();
  //gc.SetPenStyle(CGraphicsContext::ENullPen);
  gc.SetBrushColor(AKN_LAF_COLOR_STATIC(KWhiteColor));
  gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
  TRect rect=Rect();
  gc.Clear(rect);
}


TKeyResponse CStopWatchControl::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
{
  return EKeyWasNotConsumed;
}
