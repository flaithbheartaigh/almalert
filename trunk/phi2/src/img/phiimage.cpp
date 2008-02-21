/*
    phiimage.cpp
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

#include "img/phiimage.hpp"
#include "img/phiimagepane.hpp"
#include "phi.hpp"
#include <phien.rsg>

CPhiImageView* CPhiImageView::NewLC(void)
{
  CPhiImageView* self=new(ELeave)CPhiImageView;
  CleanupStack::PushL(self);
  self->ConstructL();
  return self;
}

CPhiImageView::~CPhiImageView()
{
  delete iControl;
}

TUid CPhiImageView::Id(void) const
{
  return KPhiImageViewId;
}

void CPhiImageView::HandleCommandL(TInt aCommand)
{
  switch(aCommand)
  {
    case EAknSoftkeyBack:
      iPhiAppUi->CmdBackL();
      break;
    default:
      AppUi()->HandleCommandL(EEikCmdExit);
  }
}

void CPhiImageView::SetFileName(const TDesC& aFileName)
{
  iFileName=aFileName;
}

void CPhiImageView::DoActivateL(const TVwsViewId& aPrevViewId,TUid aCustomMessageId,const TDesC8& aCustomMessage)
{
  if(!iControl)
  {
    TRect rect=ClientRect();
    rect.iTl.iY=0;
    rect.iBr.iY=CCoeEnv::Static()->ScreenDevice()->SizeInPixels().iHeight;
    iControl=CPhiImagePane::NewL(rect,this,iFileName);
    iPhiAppUi->AddToStackL(*this,iControl);
  }
  iControl->ActivateL();
}

void CPhiImageView::DoDeactivate(void)
{
  if(iControl)
  {
    iPhiAppUi->RemoveFromStack(iControl);
    delete iControl;
    iControl=NULL;
  }
}

void CPhiImageView::ConstructL(void)
{
  BaseConstructL(R_PHI_IMAGE_VIEW);
}
