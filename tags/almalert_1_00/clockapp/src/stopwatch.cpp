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
      iControl->ProcessSignal(CStopWatchControl::ESignalStop);
      break;
    case EClockAppExtraStopWatchLapReset:
      iControl->ProcessSignal(CStopWatchControl::ESignalReset);
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
  delete iRefresh;
}

CStopWatchControl::CStopWatchControl(void)
{
}

void CStopWatchControl::ConstructL(const TRect& aRect)
{
  iCoeEnv->ReadResourceAsDes16(iLapLabel,R_CLOCKAPP_STOP_WATCH_LAP);
  CreateWindowL();
  SetRect(aRect);
  DrawNow(); //FIXME: needed?
  ActivateL();
  iRefresh=CPeriodic::NewL(CActive::EPriorityStandard);
  InitRefresh();
}

void CStopWatchControl::HandleGainingForeground(void)
{
  InitRefresh();
}

void CStopWatchControl::HandleLosingForeground(void)
{
  iRefresh->Cancel();
}

void CStopWatchControl::InitRefresh(void)
{
  iRefresh->Cancel();
  iRefresh->Start(100000,100000,TCallBack(RefreshTimeout,this));
}

TInt CStopWatchControl::RefreshTimeout(TAny* aContainer)
{
  STATIC_CAST(CStopWatchControl*,aContainer)->DrawNow();
  return 0;
}

void CStopWatchControl::Draw(const TRect& aRect) const
{
  CWindowGc& gc=SystemGc();
  gc.SetPenStyle(CGraphicsContext::ESolidPen);
  gc.SetPenColor(AKN_LAF_COLOR_STATIC(KBlackColor));
  gc.SetBrushColor(AKN_LAF_COLOR_STATIC(KWhiteColor));
  gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
  TRect rect=Rect();
  gc.Clear(rect);
  TBuf<11> time; TBuf<32> lap;
  CurrentTime(time,lap);
  gc.UseFont(ClockBold30());
  TRect rectTime(0,40,176,70);
  gc.DrawText(time,rectTime,30,CGraphicsContext::ECenter);
  gc.DiscardFont();
  gc.UseFont(LatinPlain12());
  TRect rectLap(0,72,176,84);
  gc.DrawText(lap,rectLap,10,CGraphicsContext::ECenter);
  gc.DiscardFont();
}

TKeyResponse CStopWatchControl::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
{
  if(aType==EEventKey)
  {
    switch(aKeyEvent.iCode)
    {
      case '5':
        ProcessSignal(ESignalStop);
        break;
      case '7':
        ProcessSignal(ESignalReset);
        break;
    }
  }
  return EKeyWasNotConsumed;
}


void CStopWatchControl::ProcessSignal(TSignal aSignal)
{
  typedef void (CStopWatchControl::*TStateFunction)(void);
  const struct SStates
  {
    TStates iNew[ESignalLast];
    TStateFunction iFunc[ESignalLast];
  } states[EStateLast]=
  {
    {{EStateStart,EStateClear},{&ProcessStart,&ProcessNothing}},//EStateClear
    {{EStateStop,EStateLap1},{&ProcessStop,&ProcessLap}},//EStateStart
    {{EStateStart,EStateClear},{&ProcessRestart,&ProcessNothing}},//EStateStop
    {{EStateLap2,EStateStart},{&ProcessStop,&ProcessNothing}},//EStateLap1
    {{EStateStart,EStateLap2Visible},{&ProcessRestart,&ProcessNothing}},//EStateLap2
    {{EStateStart,EStateClear},{&ProcessRestart,&ProcessNothing}}//EStateLap2Visible
  };
  (this->*states[iState].iFunc[aSignal])();
  iState=states[iState].iNew[aSignal];
}

void CStopWatchControl::ProcessNothing(void)
{
}

void CStopWatchControl::ProcessStart(void)
{
  iStart.HomeTime();
  iPause=0;
}

void CStopWatchControl::ProcessStop(void)
{
  iStop.HomeTime();
}

void CStopWatchControl::ProcessRestart(void)
{
  TTime current;
  current.HomeTime();
  iPause+=current.MicroSecondsFrom(iStop).Int64();
}

void CStopWatchControl::ProcessLap(void)
{
  iLap.HomeTime();
}

_LIT(KZero,"00:00:00.00");
_LIT(KFormat,"%02.2d:%02.2d:%02.2d.%02.2d");

void CStopWatchControl::CurrentTime(TDes& aTime,TDes& aLap) const
{
  TTime current;
  current.HomeTime();
  TInt64 diff=0;
  switch(iState)
  {
    case EStateStart:
      diff=current.MicroSecondsFrom(iStart).Int64()-iPause;
      break;
    case EStateStop:
    case EStateLap2Visible:
      diff=iStop.MicroSecondsFrom(iStart).Int64()-iPause;
      break;
    case EStateLap1:
    case EStateLap2:
      diff=iLap.MicroSecondsFrom(iStart).Int64()-iPause;
      break;
    default:
      aTime.Copy(KZero);
      return;
  }
  diff=(diff+5000)/10000;
  TInt msec=(diff%100).GetTInt();
  diff/=100;
  TInt sec=(diff%60).GetTInt();
  diff/=60;
  TInt min=(diff%60).GetTInt();
  diff/=60;
  TInt hour=(diff%60).GetTInt();
  aTime.Format(KFormat,hour,min,sec,msec);
  aLap.Zero();
  if((iState==EStateLap1&&current.DateTime().MicroSecond()>500000)||iState==EStateLap2)
  {
    aLap.Copy(iLapLabel);
  }
}
