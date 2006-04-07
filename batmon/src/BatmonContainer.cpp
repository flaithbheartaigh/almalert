/*
    BatmonContainer.cpp
    Copyright (C) 2005 zg

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

#include "BatmonContainer.hpp"
#include <eikenv.h> //CEikonEnv
#include <eikspane.h> //CEikStatusPane
#include <aknutils.h> //fonts
#include <isi_units.hpp>
#include <hwtricks.hpp>

#include <f32file.h>
static void Log(const TDesC8& aBuffer,TInt aParam)
{
  RFs fs;
  RFile file;
  fs.Connect();
  TInt err=file.Open(fs,_L("e:\\zg_netmon_cl_log.txt"),EFileWrite|EFileShareAny);
  if(err==KErrNotFound) err=file.Create(fs,_L("e:\\zg_netmon_cl_log.txt"),EFileWrite|EFileShareAny);
  TInt pos=0;
  err=file.Seek(ESeekEnd,pos);
  TBuf8<1024> logBuf;
  TTime localTime; RThread thread;
  localTime.HomeTime();
  logBuf.Format(_L8("0x%08.8d|%02.2d:%02.2d:%02.2d.%06.6d: %S: 0x%x\n"),thread.Id(),localTime.DateTime().Hour(),localTime.DateTime().Minute(),localTime.DateTime().Second(),localTime.DateTime().MicroSecond(),&aBuffer,aParam);
  file.Write(logBuf);
  file.Close();
  fs.Close();
}

CBatmonContainer* CBatmonContainer::NewL(void)
{
  CBatmonContainer* self=new(ELeave)CBatmonContainer;
  self->SetMopParent(CEikonEnv::Static()->EikAppUi());
  CleanupStack::PushL(self);
  self->ConstructL();
  CleanupStack::Pop(); //self
  return self;
}

CBatmonContainer::~CBatmonContainer()
{
  CCoeEnv::Static()->RemoveForegroundObserver(*this);
  delete iRefresh;
}

CBatmonContainer::CBatmonContainer()
{
}

void CBatmonContainer::ConstructL(void)
{
  CreateWindowL();
  SetExtentToWholeScreen();
  CEikStatusPane* statusP=iEikonEnv->AppUiFactory()->StatusPane();
  if(statusP) statusP->MakeVisible(EFalse);
  DrawNow();
  ActivateL();
  iRefresh=CPeriodic::NewL(CActive::EPriorityStandard);
  InitRefresh();
  CCoeEnv::Static()->AddForegroundObserverL(*this);
}

void CBatmonContainer::HandleGainingForeground(void)
{
  InitRefresh();
}

void CBatmonContainer::HandleLosingForeground(void)
{
  iRefresh->Cancel();
}

void CBatmonContainer::InitRefresh(void)
{
  iRefresh->Cancel();
  iRefresh->Start(1000000,1000000,TCallBack(RefreshTimeout,this));
}

TKeyResponse CBatmonContainer::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
{
  return EKeyWasNotConsumed;
}

TInt CBatmonContainer::RefreshTimeout(TAny* aContainer)
{
  STATIC_CAST(CBatmonContainer*,aContainer)->DrawNow();
  return 0;
}

void CBatmonContainer::Value(TUint8 aUnit,TUint16 aAddress,TDes16& aValue) const
{
  TRAPD(err,HWNetmon::ValueL(aUnit,aAddress,aValue,HWNetmon::EExt));
  if(err!=KErrNone)
  {
    aValue.Zero();
    aValue.Append('?');
  }
}

void CBatmonContainer::Temperature(TDes16& aValue,TUint16 aAddress) const
{
  TRAPD(err,HWNetmon::ValueL(KPhoneEmUnit,aAddress,aValue,HWNetmon::EExt|HWNetmon::ERaw));
  if(err==KErrNone&&aValue.Length()>2)
  {
    TInt value=aValue[1]*256+aValue[2]-273;
    aValue.Num(value);
    aValue.Append(0xb0);
    aValue.Append('C');
  }
  else
  {
    aValue.Zero();
    aValue.Append('?');
  }
}

void CBatmonContainer::Voltage(TDes16& aValue,TUint16 aAddress) const
{
  Value(KPhoneEmUnit,aAddress,aValue);
  aValue.Append(_L(" mV"));
}

void CBatmonContainer::Current(TDes16& aValue,TUint16 aAddress) const
{
  Value(KPhoneEmUnit,aAddress,aValue);
  aValue.Append(_L(" mA"));
}

void CBatmonContainer::Capacity(TDes16& aValue,TUint16 aAddress) const
{
  Value(KPhoneEmUnit,aAddress,aValue);
  aValue.Append(_L(" mAh"));
}

void CBatmonContainer::Impedance(TDes16& aValue,TUint16 aAddress) const
{
  Value(KPhoneEmUnit,aAddress,aValue);
  aValue.Append(_L(" m"));
  aValue.Append(0x3a9);
}

void CBatmonContainer::Hour(TDes16& aValue,TUint16 aAddress) const
{
  Value(KPhoneEmUnit,aAddress,aValue);
  aValue.Append(_L(" h"));
}

void CBatmonContainer::Battery(TDes16& aValue,TUint16 aAddress) const
{
  TInt value=0;
  TRAPD(err,HWNetmon::ValueL(KPhoneEmUnit,aAddress,aValue,HWNetmon::EExt|HWNetmon::ERaw));
  if(err==KErrNone&&aValue.Length()>0) value=aValue[0];
  aValue.Zero();
  switch(value)
  {
    case 11:
      aValue.Append(_L("BL-4C"));
      break;
    case 12:
      aValue.Append(_L("BL-5C"));
      break;
    case 17:
      aValue.Append(_L("BL-6C"));
      break;
    default:
      aValue.Append('?');
      break;
  }
}

void CBatmonContainer::Common(TDes16& aValue,TUint16 aAddress) const
{
  Value(KPhoneEmUnit,aAddress,aValue);
}

void CBatmonContainer::Draw(const TRect& aRect) const
{
  CWindowGc& gc=SystemGc();
  gc.SetPenColor(TRgb::Color4K(0x048));
  gc.SetPenStyle(CGraphicsContext::ESolidPen);
  gc.SetBrushColor(TRgb::Color4K(0x000));
  gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
  TRect rect=Rect();
  gc.DrawRect(rect);

  DrawTitle();
  DrawStatus();
  DrawExitMark();
  DrawPage();


/*  gc.SetPenStyle(CGraphicsContext::ESolidPen);
  gc.SetPenColor(TRgb::Color4K(0xf44));
  gc.SetBrushStyle(CGraphicsContext::ENullBrush);
  gc.UseFont(LatinPlain12());

  TBuf<32> data;
  TRect label(2,12,173,24); //52,53
  TRect value(label);

  gc.DrawText(_L("0x0125"),label,10);
  Value(KPhoneEmUnit,0x0125,data);
  gc.DrawText(data,value,10,CGraphicsContext::ERight);
  label.Move(0,13);
  value.Move(0,13);
  gc.DrawText(_L("0x0119"),label,10);
  Value(KPhoneEmUnit,0x0119,data);
  gc.DrawText(data,value,10,CGraphicsContext::ERight);
  label.Move(0,13);
  value.Move(0,13);
  gc.DrawText(_L("0x011a"),label,10);
  Value(KPhoneEmUnit,0x011a,data);
  gc.DrawText(data,value,10,CGraphicsContext::ERight);
  label.Move(0,13);
  value.Move(0,13);


  gc.DrawText(_L("Battery current"),label,10);
  Value(KPhoneEmUnit,0x0112,data);
  gc.DrawText(data,value,10,CGraphicsContext::ERight);
  label.Move(0,13);
  value.Move(0,13);
  gc.DrawText(_L("Phone current"),label,10);
  Value(KPhoneEmUnit,0x0113,data);
  gc.DrawText(data,value,10,CGraphicsContext::ERight);
  label.Move(0,13);
  value.Move(0,13);
  gc.DrawText(_L("Feature current"),label,10);
  Value(KPhoneEmUnit,0x0114,data);
  gc.DrawText(data,value,10,CGraphicsContext::ERight);
  label.Move(0,13);
  value.Move(0,13);
  gc.DrawText(_L("Discharge mAs"),label,10);
  Value(KPhoneEmUnit,0x0115,data);
  gc.DrawText(data,value,10,CGraphicsContext::ERight);
  label.Move(0,13);
  value.Move(0,13);
  gc.DrawText(_L("Voltage"),label,10);
  Value(KPhoneEmUnit,0x0103,data);
  gc.DrawText(data,value,10,CGraphicsContext::ERight);
  label.Move(0,13);
  value.Move(0,13);
  gc.DrawText(_L("Temperature"),label,10);
  Value(KPhoneEmUnit,0x0102,data);
  gc.DrawText(data,value,10,CGraphicsContext::ERight);
  label.Move(0,13);
  value.Move(0,13);
  gc.DrawText(_L("Capacity"),label,10);
  Value(KPhoneEmUnit,0x0104,data);
  gc.DrawText(data,value,10,CGraphicsContext::ERight);
  label.Move(0,13);
  value.Move(0,13);

  gc.DiscardFont();*/
}

void CBatmonContainer::DrawTitle(void) const
{
  CWindowGc& gc=SystemGc();
  gc.SetPenColor(TRgb::Color4K(0x048));
  gc.SetPenStyle(CGraphicsContext::ESolidPen);
  gc.SetBrushColor(TRgb::Color4K(0x006));
  gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
  TRect rect=Rect();
  rect.SetHeight(14);
  gc.DrawRect(rect);
  TRect value(rect);
  value.Shrink(1,1);
  gc.UseFont(LatinPlain12());
  SValue title[]=
  {
    {32,NULL,0x0102,&CBatmonContainer::Temperature},
    {50,NULL,0x0103,&CBatmonContainer::Voltage},
    {47,NULL,0x0113,&CBatmonContainer::Current},
    {47,NULL,0x0112,&CBatmonContainer::Current}
  };
  TBuf<32> data;
  for(TUint i=0;i<sizeofa(title);i++)
  {
    gc.SetPenStyle(CGraphicsContext::ESolidPen);
    gc.SetPenColor(TRgb::Color4K(0xff0));
    gc.SetBrushStyle(CGraphicsContext::ENullBrush);
    (this->*(title[i].iFunc))(data,title[i].iAddress);
    gc.DrawText(data,value,11,CGraphicsContext::ERight);
    DrawRight(value);
    value.Resize(-title[i].iWidth,0);
  }
  gc.DiscardFont();
}

void CBatmonContainer::DrawStatus(void) const
{
}

void CBatmonContainer::DrawExitMark(void) const
{
  CWindowGc& gc=SystemGc();
  gc.SetPenColor(TRgb::Color4K(0xf00));
  gc.SetPenStyle(CGraphicsContext::ESolidPen);
  gc.MoveTo(TPoint(171,206));
  gc.DrawLineBy(TPoint(3,0));
  gc.DrawLineBy(TPoint(0,-4));
  gc.MoveBy(TPoint(0,4));
  gc.DrawLineBy(TPoint(-4,-4));
}

void CBatmonContainer::DrawPage(void) const
{
  CWindowGc& gc=SystemGc();
  gc.SetPenStyle(CGraphicsContext::ESolidPen);
  gc.SetPenColor(TRgb::Color4K(0xff0));
  gc.SetBrushStyle(CGraphicsContext::ENullBrush);
  gc.UseFont(LatinPlain12());
  SValue page[]=
  {
    {0,L"Battery capacity",0x0104,&CBatmonContainer::Capacity},
    {4,L"Battery impedance",0x0146,&CBatmonContainer::Impedance},
    {14,L"Standby time",0x0106,&CBatmonContainer::Hour},
    {24,L"Battery type",0x0101,&CBatmonContainer::Common},
    {6,L"Charging voltage",0x0119,&CBatmonContainer::Voltage},
    {6,L"Charging current",0x011a,&CBatmonContainer::Current},
    {24,L"Battery",0x014d,&CBatmonContainer::Battery}
  };
  TRect value(2,18,174,30);
  TBuf<32> data;
  for(TUint i=0;i<sizeofa(page);i++)
  {
    TPtrC ptr((TUint16*)page[i].iTitle);
    gc.DrawText(ptr,value,10);
    (this->*(page[i].iFunc))(data,page[i].iAddress);
    gc.DrawText(data,value,10,CGraphicsContext::ERight,page[i].iWidth);
    value.Move(0,13);
  }
  gc.DiscardFont();
}

void CBatmonContainer::DrawRight(const TRect& aRect) const
{
  CWindowGc& gc=SystemGc();
  gc.SetPenColor(TRgb::Color4K(0x048));
  gc.SetPenStyle(CGraphicsContext::ESolidPen);
  gc.MoveTo(TPoint(aRect.iBr.iX+1,aRect.iBr.iY));
  gc.DrawLineBy(TPoint(0,-aRect.Height()-1));
}
