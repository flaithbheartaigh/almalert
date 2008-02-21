/*
    phiimagepane.cpp
    Copyright (C) 2008 zg

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "img/phiimagepane.hpp"
#include "phiutils.hpp"
#include <phien.rsg>

CPhiImagePane* CPhiImagePane::NewL(const TRect& aRect,MObjectProvider* aObjectProvider,const TDesC& aFileName)
{
  CPhiImagePane* self=new(ELeave)CPhiImagePane;
  CleanupStack::PushL(self);
  self->ConstructL(aRect,aObjectProvider,aFileName);
  CleanupStack::Pop(); //self
  return self;
}

CPhiImagePane::~CPhiImagePane()
{
  delete iConvert;
  delete iBitmap;
}

TKeyResponse CPhiImagePane::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
{
  if(aType==EEventKey)
  {
    switch(aKeyEvent.iCode)
    {
      case EKeyLeftArrow:
      case EKeyRightArrow:
      case EKeyUpArrow:
      case EKeyDownArrow:
        ScrollL(aKeyEvent.iCode,(aKeyEvent.iModifiers&EModifierShift)!=0);
        return EKeyWasConsumed;
    }
  }
  return EKeyWasNotConsumed;
}

void CPhiImagePane::Draw(const TRect& aRect) const
{
  CWindowGc& gc=SystemGc();
  if(iLoaded)
  {
    gc.SetBrushColor(CEikonEnv::AvkonColor(0));
    gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
    gc.Clear(aRect);
    TRect part(Rect());
    part.Move(iOffset);
    gc.BitBlt(iDrawOffset,iBitmap,part);
  }
}

void CPhiImagePane::ConstructL(const TRect& aRect,MObjectProvider* aObjectProvider,const TDesC& aFileName)
{
  SetMopParent(aObjectProvider);
  CreateWindowL();
  SetRect(aRect);
  iBitmap=new(ELeave)CFbsBitmap();
  iConvert=CMdaImageFileToBitmapUtility::NewL(*this);
  iConvert->OpenL(aFileName);
}

CPhiImagePane::CPhiImagePane(): CCoeControl()
{
}

void CPhiImagePane::MiuoCreateComplete(TInt aError)
{
  if(aError<KErrNone) {TRAPD(err,PhiUtils::ShowErrorL(R_PHI_ERROR,aError));}
}

void CPhiImagePane::MiuoOpenComplete(TInt aError)
{
  if(aError<KErrNone) {TRAPD(err,PhiUtils::ShowErrorL(R_PHI_ERROR,aError));}
  else if(iConvert->FrameCount()>0)
  {
    TFrameInfo info;
    iConvert->FrameInfo(0,info);
    TInt err=iBitmap->Create(info.iOverallSizeInPixels,CCoeEnv::Static()->ScreenDevice()->DisplayMode());
    if(err==KErrNone) TRAP(err,iConvert->ConvertL(*iBitmap,0));
  }
}

void CPhiImagePane::MiuoConvertComplete(TInt aError)
{
  if(aError<KErrNone) {TRAPD(err,PhiUtils::ShowErrorL(R_PHI_ERROR,aError));}
  else
  {
    iLoaded=ETrue;
    TSize size=iBitmap->SizeInPixels();
    TRect rect=Rect();
    if(rect.Width()>size.iWidth) iDrawOffset.iX=(rect.Width()-size.iWidth)/2;
    if(rect.Height()>size.iHeight) iDrawOffset.iY=(rect.Height()-size.iHeight)/2;
    DrawNow();
  }
}

void CPhiImagePane::ScrollL(TUint aCode,TBool aFast)
{
  if(!iLoaded) return;
  TSize size=iBitmap->SizeInPixels();
  TRect rect=Rect();
  TInt step=aFast?EStepFast:EStep;
  switch(aCode)
  {
    case EKeyLeftArrow:
      iOffset.iX-=step;
      break;
    case EKeyRightArrow:
      iOffset.iX+=step;
      break;
    case EKeyUpArrow:
      iOffset.iY-=step;
      break;
    case EKeyDownArrow:
      iOffset.iY+=step;
      break;
  }
  if(iOffset.iX<0) iOffset.iX=0;
  if(iOffset.iY<0) iOffset.iY=0;
  if(iOffset.iX+rect.Width()>size.iWidth) iOffset.iX=size.iWidth-rect.Width();
  if(iOffset.iY+rect.Height()>size.iHeight) iOffset.iY=size.iHeight-rect.Height();
  if(rect.Width()>size.iWidth) iOffset.iX=0;
  if(rect.Height()>size.iHeight) iOffset.iY=0;
  DrawNow();
}
