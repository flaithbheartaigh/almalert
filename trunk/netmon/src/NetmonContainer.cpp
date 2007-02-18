/*
    NetmonContainer.cpp
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

#include "NetmonContainer.hpp"
#include <eikenv.h> //CEikonEnv
#include <eikspane.h> //CEikStatusPane
#include <aknutils.h> //fonts
#include <aknquerydialog.h> //CAknNumberQueryDialog
#include <aknlistquerydialog.h> //CAknListQueryDialog
#include <isi_units.hpp>
#include <hwtricks.hpp>
#include <netmon.rsg>
#include "NetmonRefresh.hpp"

CNetmonContainer* CNetmonContainer::NewL(void)
{
  CNetmonContainer* self=new(ELeave)CNetmonContainer;
  self->SetMopParent(CEikonEnv::Static()->EikAppUi());
  CleanupStack::PushL(self);
  self->ConstructL();
  CleanupStack::Pop(); //self
  return self;
}

CNetmonContainer::~CNetmonContainer()
{
  CCoeEnv::Static()->RemoveForegroundObserver(*this);
  delete iNetmonRefresh;
  delete iRefresh;
}

CNetmonContainer::CNetmonContainer()
{
}

void CNetmonContainer::ConstructL(void)
{
  CreateWindowL();
  SetExtentToWholeScreen();
  CEikStatusPane* statusP=iEikonEnv->AppUiFactory()->StatusPane();
  if(statusP) statusP->MakeVisible(EFalse);
  DrawNow();
  ActivateL();
  iRefresh=CPeriodic::NewL(CActive::EPriorityStandard);
  InitRefresh();
  iNetmonRefresh=CNetmonRefresh::NewL();
  CCoeEnv::Static()->AddForegroundObserverL(*this);
}

void CNetmonContainer::HandleGainingForeground(void)
{
  InitRefresh();
}

void CNetmonContainer::HandleLosingForeground(void)
{
  iRefresh->Cancel();
}

void CNetmonContainer::InitRefresh(void)
{
  iRefresh->Cancel();
  iRefresh->Start(1000000,1000000,TCallBack(RefreshTimeout,this));
}

TKeyResponse CNetmonContainer::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
{
  if(aType==EEventKey)
  {
    switch(aKeyEvent.iCode)
    {
      case '1':
      case '2':
      case '3':
//      case '4':
        iTab=aKeyEvent.iCode-'1';
        DrawNow();
        return EKeyWasConsumed;
      case '5':
        iNetmonRefresh->Refresh();
        return EKeyWasConsumed;
      case '6':
        {
          TInt channel=0;
          CAknNumberQueryDialog* dlg=new(ELeave)CAknNumberQueryDialog(channel);
          if(dlg->ExecuteLD(R_CHANEL_QUERY)==EAknSoftkeyOk)
          {
            HWNetmon::SetChannelL(channel);
          }
        }
        return EKeyWasConsumed;
      case '8':
        {
          TBuf<32> data;
          Value(KPhoneGssUnit,0x3287,data);
        }
        return EKeyWasConsumed;
      case '9':
        {
          TBuf<32> data;
          Value(KPhoneGssUnit,0x3282,data);
        }
        return EKeyWasConsumed;
      case '0':
        {
          TInt freq=0;
          CAknListQueryDialog* dlg=new(ELeave)CAknListQueryDialog(&freq);
          if(dlg->ExecuteLD(R_FREQ_QUERY)==EAknSoftkeyOk)
          {
            HWNetmon::SetFrequencyL((HWNetmon::TBandFrequency)freq);
          }
        }
        return EKeyWasConsumed;
      case EKeyLeftArrow:
        iTab--;
        if(iTab<0) iTab=KTabCount-1;
        DrawNow();
        return EKeyWasConsumed;
      case EKeyRightArrow:
        iTab++;
        if(iTab>=KTabCount) iTab=0;
        DrawNow();
        return EKeyWasConsumed;
    }
  }
  return EKeyWasNotConsumed;
}

TInt CNetmonContainer::RefreshTimeout(TAny* aContainer)
{
  STATIC_CAST(CNetmonContainer*,aContainer)->DrawNow();
  return 0;
}

const TInt KTabHeight=10;
const TInt KTabWidth=25;
const TInt KTabGap=10;

void CNetmonContainer::Draw(const TRect& aRect) const
{
  CWindowGc& gc=SystemGc();
  gc.SetPenStyle(CGraphicsContext::ENullPen);
  gc.SetBrushColor(TRgb::Color4K(0x000));
  gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
  TRect rect=Rect();
  gc.Clear(rect);

  for(TInt i=0;i<iTab;i++)
  {
    DrawTab(EFalse,i);
  }
  for(TInt i=KTabCount-1;i>iTab;i--)
  {
    DrawTab(EFalse,i);
  }
  gc.SetPenColor(TRgb::Color4K(0x0F0));
  gc.SetPenStyle(CGraphicsContext::ESolidPen);
  gc.SetBrushStyle(CGraphicsContext::ENullBrush);
  TRect clientRect(rect.iTl.iX,rect.iTl.iY+KTabHeight,rect.iBr.iX,rect.iBr.iY);
  gc.DrawRect(clientRect);
  DrawExitMark();
  DrawTab(ETrue,iTab);
  switch(iTab)
  {
    case 0:
      DrawPage0();
      DrawCellId();
      break;
    case 1:
      DrawPage1();
      DrawCellId();
      break;
    case 2:
      DrawPage2();
      DrawCellId();
      break;
  }
}

void CNetmonContainer::DrawExitMark(void) const
{
  CWindowGc& gc=SystemGc();
  gc.SetPenColor(TRgb::Color4K(0xFF0));
  gc.SetPenStyle(CGraphicsContext::ESolidPen);
  gc.MoveTo(TPoint(171,206));
  gc.DrawLineBy(TPoint(3,0));
  gc.DrawLineBy(TPoint(0,-4));
  gc.MoveBy(TPoint(0,4));
  gc.DrawLineBy(TPoint(-4,-4));
}

void CNetmonContainer::Abr(TDes16& aString) const
{
  aString.Trim();
  if(aString.Length()>3)
  {
    aString.SetLength(3);
    aString.Append('.');
  }
}

void CNetmonContainer::DrawCellId(void) const
{
  CWindowGc& gc=SystemGc();
  TBuf<32> data;
  gc.SetBrushStyle(CGraphicsContext::ENullBrush);
  gc.SetPenColor(TRgb::Color4K(0x00F));
  gc.SetPenStyle(CGraphicsContext::ESolidPen);
  gc.UseFont(LatinPlain12());
  TRect value(14,194,38,206);
  Value(KPhoneNetworkUnit,0x1304,data);
  if(data.Length()>4) data.Copy(data.Mid(4));
  gc.DrawText(data,value,10,CGraphicsContext::ELeft);
  value.SetRect(51,194,71,206);
  Value(KPhoneGssUnit,0x3286,data);
  Abr(data);
  gc.DrawText(data,value,10,CGraphicsContext::ELeft);
  value.SetRect(84,194,104,206);
  Value(KPhoneGssUnit,0x3244,data);
  Abr(data);
  gc.DrawText(data,value,10,CGraphicsContext::ELeft);
  value.SetRect(117,194,133,206);
  Value(KPhoneGssUnit,0x32fb,data);
  gc.DrawText(data,value,10,CGraphicsContext::ELeft);
  value.SetRect(134,194,169,206);
  Value(KPhoneGssUnit,0x3240,data);
  gc.DrawText(data,value,10,CGraphicsContext::ERight);
  gc.DiscardFont();
  gc.SetPenColor(TRgb::Color4K(0xFF0));
  gc.UseFont(NumberPlain5());
  value.SetRect(2,195,12,205);
  const TInt labels[]={6,8,9,0};
  const TInt shifts[]={37,33,33,0};
  for(TUint i=0;i<sizeofa(labels);i++)
  {
    gc.DrawEllipse(value);
    data.Num(labels[i]);
    gc.DrawText(data,value,7,CGraphicsContext::ECenter);
    value.Move(shifts[i],0);
  }
  gc.DiscardFont();
}

void CNetmonContainer::DrawTab(TBool anActive,TInt anIndex) const
{
  CWindowGc& gc=SystemGc();
  gc.SetPenColor(TRgb::Color4K(anActive?0x0F0:0x080));
  gc.SetPenStyle(CGraphicsContext::ESolidPen);
  gc.SetBrushColor(TRgb::Color4K(0x000));
  gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
  TPoint points[4];
  TInt xStart=KTabGap+anIndex*(KTabHeight+KTabWidth);
  points[0].SetXY(xStart,KTabHeight);
  TInt xPos=xStart+KTabHeight;
  points[1].iX=xPos;
  xPos+=KTabWidth;
  points[2].iX=xPos;
  xPos+=KTabHeight;
  points[3].SetXY(xPos,KTabHeight);
  gc.DrawPolygon(points,sizeofa(points));
  gc.SetBrushStyle(CGraphicsContext::ENullBrush);
  gc.UseFont(NumberPlain5());
  TBuf<1> label;
  label.Num(anIndex+1);
  gc.DrawText(label,TRect(xStart,0,xPos,KTabHeight),8,CGraphicsContext::ECenter);
  gc.DiscardFont();
  if(anActive)
  {
    gc.SetPenColor(TRgb::Color4K(0x000));
    gc.MoveTo(TPoint(xPos-1,KTabHeight));
    gc.DrawLineTo(TPoint(xStart,KTabHeight));
  }
}

void CNetmonContainer::Value(TUint8 aUnit,TUint16 aAddress,TDes16& aValue) const
{
  TRAPD(err,HWNetmon::ValueL(aUnit,aAddress,aValue));
  if(err!=KErrNone)
  {
    aValue.Zero();
    aValue.Append('?');
  }
}

const TInt KLines=14;
_LIT(KHoping," ");

void CNetmonContainer::DrawPage0(void) const
{
  CWindowGc& gc=SystemGc();
  TBuf<32> data;
  const wchar_t* labels[]=
  {
    L"Rx level",
    L"Chnl num",
    L"Tx pwr lvl",
    L"TS",
    L"TA",
    L"Rx quality",
    L"R time",
    L"C1",
    L"C2",
    L"Band",
    L"CHty",
    L"Pag. mode",
    L"max RACH",
    L"Roaming",
    L"BSIC",
    L"CC cause",
    L"CRO",
    L"Hopping",
    L"PenT",
    L"HCh",
    L"MAIO",
    L"HSN",
    L"BTS tst",
    L"Call b.",
    L"DTX",
    L"DTX def.",
    L"DTX BS",
    L"LU band"
  };
  const TUint16 addresses[]=
  {
    0x3200,
    0x3201,
    0x3203,
    0x3204,
    0x3205,
    0x3206,
    0x3207,
    0x3208,
    0x320a,
    0x32f1,
    0x3209,
    0x320b,
    0x320c,
    0x320d,
    0x320e,
    0x320f,
    0x3210,
    0x3211,
    0x3212,
    0x3213,
    0x3214,
    0x3215,
    0x1304,
    0x3286,
    0x3244,
    0x3245,
    0x3246,
    0x32fb
  };

  gc.SetPenStyle(CGraphicsContext::ESolidPen);
  gc.SetBrushStyle(CGraphicsContext::ENullBrush);

  gc.UseFont(LatinPlain12());

  for(TInt i=0;i<2;i++)
  {
    TRect label(2,12,86,24); //52,53
    TRect value(label);
    for(TInt j=0;j<i;j++)
    {
      label.Move(89,0);
      value.Move(89,0);
    }
    for(TInt j=0;j<KLines;j++)
    {
      TUint index=j+i*KLines;
      if(index>=sizeofa(addresses)) break;
      gc.SetPenColor(TRgb::Color4K(0x0F0));
      TPtrC ptr((TUint16*)labels[index]);
      gc.DrawText(ptr,label,10);
      TUint8 unit=KPhoneGssUnit;
      if(addresses[index]==0x1304) unit=KPhoneNetworkUnit;
      Value(unit,addresses[index],data);
      if(addresses[index]==0x3201)
      {
        TBuf<8> flag;
        Value(KPhoneGssUnit,0x3202,flag);
        if(flag.Compare(KHoping))
           gc.SetPenColor(TRgb::Color4K(0xF00));
      }
      gc.DrawText(data,value,10,CGraphicsContext::ERight);
      label.Move(0,13);
      value.Move(0,13);
    }
  }
  gc.DiscardFont();
}

const TInt KPage1Cols=7;
const TInt KLines2=3;

void CNetmonContainer::DrawPage1(void) const
{
  CWindowGc& gc=SystemGc();
  TBuf<32> data;
  TRect cols[KPage1Cols];
  cols[0].SetRect(2,12,14,24);
  cols[1].SetRect(15,12,50,24);
  cols[2].SetRect(51,12,86,24);
  cols[3].SetRect(87,12,122,24);
  cols[4].SetRect(123,12,158,24);
  cols[5].SetRect(159,12,166,24);
  cols[6].SetRect(167,12,174,24);

  wchar_t* labels[]=
  {
    L"SC",
    L"1.",
    L"2.",
    L"3.",
    L"4.",
    L"5.",
    L"6.",
    L"7.",
    L"8."
  };

  TUint16 addresses[][KPage1Cols-1]=
  {
    {0x3201,0x3200,0x3296,0x3297,0,0},
    {0x3216,0x3218,0x3217,0x3219,0x327f,0x3280},
    {0x321a,0x321c,0x321b,0x321d,0x3288,0x3289},
    {0x321e,0x3220,0x321f,0x3221,0x328a,0x328b},
    {0x3222,0x3224,0x3223,0x3225,0x328c,0x328d},
    {0x3226,0x3228,0x3227,0x3229,0x328e,0x328f},
    {0x322a,0x322c,0x322b,0x322d,0x3290,0x3291},
    {0x322e,0x3230,0x322f,0x3231,0x3292,0x3293},
    {0x3232,0x3234,0x3233,0x3235,0x3294,0x3295}
  };

  gc.SetPenColor(TRgb::Color4K(0x0F0));
  gc.SetPenStyle(CGraphicsContext::ESolidPen);

//  gc.SetBrushColor(TRgb::Color4K(0x800));
//  gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
  gc.SetBrushStyle(CGraphicsContext::ENullBrush);

  gc.UseFont(LatinPlain12());

  gc.DrawText(_L("Chnl"),cols[1],10,CGraphicsContext::ECenter);
  gc.DrawText(_L("Rx"),cols[2],10,CGraphicsContext::ECenter);
  gc.DrawText(_L("C1"),cols[3],10,CGraphicsContext::ECenter);
  gc.DrawText(_L("C2"),cols[4],10,CGraphicsContext::ECenter);

  for(TUint i=0;i<sizeofa(labels);i++)
  {
    for(int j=0;j<KPage1Cols;j++)
      cols[j].Move(0,13);
    TPtrC ptr((TUint16*)labels[i]);
    gc.DrawText(ptr,cols[0],10);
    for(int j=0;j<(KPage1Cols-1);j++)
    {
      TUint16 address=addresses[i][j];
      if(address)
      {
        Value(KPhoneGssUnit,address,data);
        gc.DrawText(data,cols[j+1],10,CGraphicsContext::ERight);
      }
    }
  }

  const wchar_t* labels2[]=
  {
    L"MCC",
    L"MNC",
    L"LAC",
    L"Chnl num",
    L"BSIC",
    L"Cell id"
  };
  const TUint16 addresses2[]=
  {
    0x3298,
    0x3299,
    0x323f,
    0x3201,
    0x320e,
    0x3240
  };

  for(TInt i=0;i<2;i++)
  {
    TRect label(2,155,86,167);
    TRect value(label);
    for(TInt j=0;j<i;j++)
    {
      label.Move(89,0);
      value.Move(89,0);
    }
    for(TInt j=0;j<KLines2;j++)
    {
      TUint index=j+i*KLines2;
      if(index>=sizeofa(addresses2)) break;
      gc.SetPenColor(TRgb::Color4K(0x0F0));
      TPtrC ptr((TUint16*)labels2[index]);
      gc.DrawText(ptr,label,10);
      Value(KPhoneGssUnit,addresses2[index],data);
      gc.DrawText(data,value,10,CGraphicsContext::ERight);
      label.Move(0,13);
      value.Move(0,13);
    }
  }

  gc.DiscardFont();
}

void CNetmonContainer::DrawPage2(void) const
{
  CWindowGc& gc=SystemGc();
  TBuf<32> data;
  const wchar_t* labels[]=
  {
    L"TMSI",
    L"T3212 cur.",
    L"T3212 init.",
    L"Pag. rep.",
    L"Last reg",
    L"1st pref",
    L"2nd pref",
    L"3rd pref",
    L"1st forb",
    L"2nd forb",
    L"3rd forb",
    L"4th forb",
    L"",
    L"",

    L"Emerg. call",
    L"Attach detach",
    L"Half rate",
    L"C2 broad",
    L"SI 7&8 broad",
    L"Cell broad",
    L"Call Re-est",

    L"ECSC",
    L"2-Ter",
    L"Mband",
    L"Ciphering v.",
    L"Hopping v.",
    L"DTX v.",
    L"IMSI attach"
  };
  const TUint16 addresses[]=
  {
    0x323b,
    0x323c,
    0x323d,
    0x323e,
    0x3236,
    0x3237,
    0x3238,
    0x3239,
    0x3269,
    0x326a,
    0x326b,
    0x326c,
    0,
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    11,
    12,
    13,
    0x3241,
    0x329a,
    0x3242,
    0x3243
  };
  gc.SetPenStyle(CGraphicsContext::ESolidPen);
  gc.SetBrushStyle(CGraphicsContext::ENullBrush);
  gc.UseFont(LatinPlain12());
  for(TInt i=0;i<2;i++)
  {
    TRect label(2,12,86,24);
    TRect value(label);
    for(TInt j=0;j<i;j++)
    {
      label.Move(89,0);
      value.Move(89,0);
    }
    for(TInt j=0;j<KLines;j++)
    {
      TUint index=j+i*KLines;
      if(index>=sizeofa(addresses)) break;
      TUint16 address=addresses[index];
      gc.SetPenColor(TRgb::Color4K(0x0F0));
      TPtrC ptr((TUint16*)labels[index]);
      gc.DrawText(ptr,label,10);
      if(address>20)
      {
        TUint8 unit=KPhoneGssUnit;
        Value(unit,address,data);
        gc.DrawText(data,value,10,CGraphicsContext::ERight);
      }
      else if(address>10)
      {
        TInt subindex=1;
        switch(address)
        {
          case 11:
            Value(KPhoneGssUnit,0x329b,data);
            break;
          case 12:
            subindex=4;
            break;
          case 13:
            subindex=8;
            break;
        }
        if(data.Length()>subindex) gc.DrawText(data.Mid(subindex,1),value,10,CGraphicsContext::ERight);
      }
      else if(address>0)
      {
        if(address==1) Value(KPhoneGssUnit,0x323a,data);
        TInt subindex=address*2-2;
        if(subindex==12) subindex=11;
        if(data.Length()>subindex) gc.DrawText(data.Mid(subindex,1),value,10,CGraphicsContext::ERight);
      }
      label.Move(0,13);
      value.Move(0,13);
    }
  }
  gc.DiscardFont();
}
