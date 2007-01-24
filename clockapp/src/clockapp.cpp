/*
    clockapp.cpp
    Copyright (C) 2005-2006 zg

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
#include <eikctl.rsg>
#include <barsread.h>
#include "clockapp.hrh"

static const TUid KUidClockApp={0x10005903};

CClkAlmViewContainer* CClkAlmViewContainer::NewL(const TRect& aRect,CClkAlmView& aView)
{
  CClkAlmViewContainer* self=new(ELeave)CClkAlmViewContainer;
  CleanupStack::PushL(self);
  self->ConstructL(aRect,aView);
  CleanupStack::Pop(); //self
  return self;
}

CClkAlmViewContainer::~CClkAlmViewContainer()
{
  delete iClock;
  delete iImage;
  delete iCurrentTimeStr;
  delete iLocalTimeStr;
  delete iNoAlarmsSetStr;
  delete iAlarmStr;
  delete iSnoozeActiveStr;
}

_LIT(KEmpty1," ");
_LIT(KFormatAmPm,"%B");
_LIT(KFormatHhMm,"%J%:1%T");
_LIT(KGmtPlus," +");
_LIT(KGmtMinus," ");
_LIT(KGmtZero,"  ");
_LIT(K0,"0");
const TInt KWeeks[]={R_QTN_WEEK_LONG_MONDAY,R_QTN_WEEK_LONG_TUESDAY,R_QTN_WEEK_LONG_WEDNESDAY,R_QTN_WEEK_LONG_THURSDAY,R_QTN_WEEK_LONG_FRIDAY,R_QTN_WEEK_LONG_SATURDAY,R_QTN_WEEK_LONG_SUNDAY};

//FIXME

inline TInt UniversalTimeOffset()
{
  TLocale locale;
  return locale.UniversalTimeOffset().Int();
}

inline TChar TimeSeparator()
{
  TLocale locale;
  return locale.TimeSeparator(1);
}

void CClkAlmViewContainer::SetNitzInfoL(void)
{
  TTime time;
  time.HomeTime();
  HBufC* str=iCoeEnv->AllocReadResourceLC(R_QTN_DATE_USUAL_WITH_ZERO);
  time.FormatL(iLocalDateStr,*str);
  CleanupStack::PopAndDestroy();
  iCoeEnv->ReadResource(iLocalWeekStr,KWeeks[time.DayNoInWeek()]);
  iCoeEnv->ReadResource(iString6,0x10931006); //FIXME
  //TLocale locale;
  TInt minOff1=UniversalTimeOffset()/60,minOff2=minOff1;
  TInt hourOff=minOff1/60;
  TInt diff=0;
  TBuf<6> buff;
  if(minOff1>0)
  {
    diff=minOff1-hourOff*60;
    iString6+=KGmtPlus;
  }
  else if(minOff2<0)
  {
    diff=hourOff*60-minOff1;
    iString6+=KGmtMinus;
  }
  else iString6+=KGmtZero;

  buff.Num(hourOff);
  iString6+=buff;
  //TLocale locale2;
  iString6.Append(TimeSeparator());
  if(diff>=0&&diff<10)
  {
    iString6+=K0;
  }
  buff.Num(diff);
  iString6+=buff;
}

void CClkAlmViewContainer::ShowActiveAlarmL(const TTime& anAlarm)
{
  iViewType=ETypeActive;
  SetAlarmTimeL(anAlarm);
  iCoeEnv->ReadResource(iAlarmWeekStr,KWeeks[anAlarm.DayNoInWeek()]);
  HBufC* str=iCoeEnv->AllocReadResourceLC(R_QTN_DATE_USUAL_WITH_ZERO);
  anAlarm.FormatL(iAlarmDateStr,*str);
  iImage->MakeVisible(ETrue);
  CleanupStack::PopAndDestroy();
}

void CClkAlmViewContainer::SetAlarmTimeL(const TTime& anAlarm)
{
  TBuf<5> buf;
  TAmPmName am;
  TAmPmName pm;
  am.Set(EAm);
  pm.Set(EPm);
  anAlarm.FormatL(buf,KFormatAmPm);
  buf.TrimAll();
  anAlarm.FormatL(iAlarmTimeStr,KFormatHhMm);
  if(buf.FindF(am)!=KErrNotFound)
  {
    iString2.Copy(am);
  }
  else
  {
    if(buf.FindF(pm)!=KErrNotFound)
    {
      iString2.Copy(pm);
    }
    else
    {
      iString2.Copy(KEmpty1);
    }
  }
}

void CClkAlmViewContainer::ShowInactiveAlarm(void)
{
  iViewType=ETypeInactive;
  iAlarmTimeStr.Copy(KEmpty1);
  iString2.Copy(KEmpty1);
  iImage->MakeVisible(EFalse);
}

void CClkAlmViewContainer::ShowSnoozedAlarmL(const TTime& anAlarm)
{
  iViewType=ETypeSnoozed;
  SetAlarmTimeL(anAlarm);
  iImage->MakeVisible(ETrue);
}

TKeyResponse CClkAlmViewContainer::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
{
  TKeyResponse res=EKeyWasNotConsumed;
  if(aType==EEventKey&&aKeyEvent.iCode==EKeyDevice3)
  {
    iView->SetOpenedByOk(ETrue);
    iView->MenuBar()->TryDisplayMenuBarL();
    res=EKeyWasConsumed;
  }
  else if(aType==EEventKey&&aKeyEvent.iCode=='#')
  {
    iView->HandleCommandL(EClockAppExtraStopWatch);
    res=EKeyWasConsumed;
  }
  return res;
}

void CClkAlmViewContainer::GetHelpContext(TCoeHelpContext& aContext) const
{
  _LIT(KHelp1,"CLK_HLP_REAL_TIME_CLOCK_AN");
  _LIT(KHelp2,"CLK_HLP_REAL_TIME_CLOCK_IN");
  aContext.iMajor=KUidClockApp;
  if(iView->NitzState())
    aContext.iContext.Copy(KHelp1);
  else
    aContext.iContext.Copy(KHelp2);
}

void CClkAlmViewContainer::SizeChanged(void)
{
  iClock->SetExtent(iRect1.Rect().iTl,iRect1.Rect().Size());
  iImage->SetExtent(iRect2.Rect().iTl,iRect2.Rect().Size());
}

TInt CClkAlmViewContainer::CountComponentControls() const
{
  return 2;
}

CCoeControl* CClkAlmViewContainer::ComponentControl(TInt aIndex) const
{
  switch(aIndex)
  {
    case 0:
      return iClock;
    case 1:
      return iImage;
    default:
      return NULL;
  }
}

void CClkAlmViewContainer::Draw(const TRect& aRect) const
{
  TRect rect=Rect();
  CWindowGc& gc=SystemGc();
  gc.Clear(aRect);
  gc.SetPenStyle(CGraphicsContext::ESolidPen);
  TAknLayoutRect layout;
  layout.LayoutRect(rect,0x10931026); //FIXME
  layout.DrawRect(gc);
  layout.LayoutRect(rect,0x10931027); //FIXME
  layout.DrawRect(gc);
  rect.Move(0,-1);
  TAknLayoutText text;
  if(iView->NitzState())
  {
    text.LayoutText(rect,0x1093102A,NULL); //FIXME
    text.DrawText(gc,*iLocalTimeStr);
    text.LayoutText(rect,0x10931028,NULL); //FIXME
    text.DrawText(gc,iString6);
  }
  else
  {
    text.LayoutText(rect,0x1093102A,NULL); //FIXME
    text.DrawText(gc,*iCurrentTimeStr);
    text.LayoutText(rect,0x10931028,NULL); //FIXME
    text.DrawText(gc,iLocalWeekStr);
    text.LayoutText(rect,0x10931029,NULL); //FIXME
    text.DrawText(gc,iLocalDateStr);
  }
  switch(iViewType)
  {
    case ETypeActive:
      text.LayoutText(rect,0x1093102E,NULL); //FIXME
      text.DrawText(gc,iAlarmTimeStr);
      text.LayoutText(rect,0x1093102B,NULL); //FIXME
      text.DrawText(gc,*iAlarmStr);
      text.LayoutText(rect,0x1093102F,NULL); //FIXME
      text.DrawText(gc,iString2);
      text.LayoutText(rect,0x1093102C,NULL); //FIXME
      text.DrawText(gc,iAlarmWeekStr);
      text.LayoutText(rect,0x1093102D,NULL); //FIXME
      text.DrawText(gc,iAlarmDateStr);
      break;
    case ETypeInactive:
      text.LayoutText(rect,0x1093102B,NULL); //FIXME
      text.DrawText(gc,*iNoAlarmsSetStr);
      break;
    case ETypeSnoozed:
      text.LayoutText(rect,0x1093102E,NULL); //FIXME
      text.DrawText(gc,iAlarmTimeStr);
      text.LayoutText(rect,0x1093102B,NULL); //FIXME
      text.DrawText(gc,*iAlarmStr);
      text.LayoutText(rect,0x1093102F,NULL); //FIXME
      text.DrawText(gc,iString2);
      text.LayoutText(rect,0x1093102C,NULL); //FIXME
      text.DrawText(gc,*iSnoozeActiveStr);
      break;
  }
}

void CClkAlmViewContainer::ConstructL(const TRect& aRect,CClkAlmView& aView)
{
  CreateWindowL();
  iView=&aView;
  TResourceReader reader;
  ControlEnv()->CreateResourceReaderLC(reader,R_EIK_CLOCK_WITH_AMPM);
  iClock=new(ELeave)CEikClock;
  iClock->SetContainerWindowL(*this);
  iClock->ConstructFromResourceL(reader);
  CleanupStack::PopAndDestroy();
  ControlEnv()->CreateResourceReaderLC(reader,0x10931025); //FIXME
  TRect rect1;
  iRect1.LayoutRect(rect1,reader);
  CleanupStack::PopAndDestroy();
  ControlEnv()->CreateResourceReaderLC(reader,0x1093100b); //FIXME
  iImage=new(ELeave)CEikImage;
  iImage->SetContainerWindowL(*this);
  iImage->ConstructFromResourceL(reader);
  iImage->MakeVisible(0);
  CleanupStack::PopAndDestroy();
  ControlEnv()->CreateResourceReaderLC(reader,0x10931030); //FIXME
  TRect rect2;
  iRect2.LayoutRect(rect2,reader);
  CleanupStack::PopAndDestroy();
  iCurrentTimeStr=ControlEnv()->AllocReadResourceAsDes16L(0x1093100a); //FIXME
  iLocalTimeStr=ControlEnv()->AllocReadResourceAsDes16L(0x10931005); //FIXME
  iNoAlarmsSetStr=ControlEnv()->AllocReadResourceAsDes16L(0x1093100d); //FIXME
  iAlarmStr=ControlEnv()->AllocReadResourceAsDes16L(0x1093100c); //FIXME
  iSnoozeActiveStr=ControlEnv()->AllocReadResourceAsDes16L(0x1093100e); //FIXME
  SetRect(aRect);
}

CApaDocument* CClkApplication::CreateDocumentL()
{
  CApaDocument* document=CClkDocument::NewL(*this);
  return document;
}

TUid CClkApplication::AppDllUid() const
{
  return KUidClockApp;
}

EXPORT_C CApaApplication* NewApplication()
{
  return new CClkApplication;
}

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
{
  return KErrNone;
}
