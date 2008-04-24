/*
    phifiles.cpp
    Copyright (C) 2007-2008 zg

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

#include "phi.hpp"
#include "phifiles.hpp"
#include "philistpane.hpp"
#include "phiutils.hpp"
#include <phien.rsg>
#include "phi.hrh"
#include <e32hal.h>

CPhiFilesView* CPhiFilesView::NewLC(void)
{
  CPhiFilesView* self=new(ELeave)CPhiFilesView;
  CleanupStack::PushL(self);
  self->ConstructL();
  return self;
}

CPhiFilesView::~CPhiFilesView()
{
  delete iControl;
}

TUid CPhiFilesView::Id(void) const
{
  return KPhiFileViewId;
}

void CPhiFilesView::HandleCommandL(TInt aCommand)
{
  TKeyEvent event={0,0,0,0};
  switch(aCommand)
  {
    case EPhiCopy:
      event.iCode='*';
      break;
    case EPhiCut:
      event.iCode='#';
      break;
    case EPhiPaste:
      event.iCode='0';
      break;
    case EPhiDelete:
      event.iCode=EKeyBackspace;
      break;
    case EPhiInfo:
      event.iCode='5';
      break;
    case EPhiSetAttr:
      event.iCode='5';
      event.iModifiers=EModifierShift;
      break;
    case EPhiRename:
      event.iCode='6';
      break;
    case EPhiMkDir:
      event.iCode='7';
      break;
    case EPhiRefresh:
      event.iCode='9';
      break;
    case EPhiBt:
      event.iCode='1';
      break;
    case EPhiPinboard:
      event.iCode='2';
      break;
    case EPhiSortName:
      event.iCode='1';
      event.iModifiers=EModifierShift;
      break;
    case EPhiSortExt:
      event.iCode='2';
      event.iModifiers=EModifierShift;
      break;
    case EPhiSortSize:
      event.iCode='3';
      event.iModifiers=EModifierShift;
      break;
    case EPhiSortDate:
      event.iCode='4';
      event.iModifiers=EModifierShift;
      break;
    case EPhiCompressMemory:
      {
        TMemoryInfoV1Buf info1,info2;
        UserHal::MemoryInfo(info1);
        User::CompressAllHeaps();
        UserHal::MemoryInfo(info2);
        TBuf<32> format;
        CCoeEnv::Static()->ReadResourceAsDes16(format,R_PHI_MEMORY_COMPRESS_MESSAGE);
        TBuf<16> str1,str2;
        PhiUtils::FormatSize(info1().iFreeRamInBytes,str1);
        PhiUtils::FormatSize(info2().iFreeRamInBytes,str2);
        TBuf<64> message;
        message.Format(format,&str1,&str2);
        PhiUtils::ShowMessageL(message,CAknNoteDialog::ENoTimeout);
      }
      return;
    case EPhiOpenWith:
      event.iCode=EKeyDevice3;
      event.iModifiers=EModifierShift;
      break;
    case EAknSoftkeyBack:
    case EAknSoftkeyExit:
      AppUi()->HandleCommandL(EEikCmdExit);
      return;
  }
  iControl->OfferKeyEventL(event,EEventKey);
}

void CPhiFilesView::DoActivateL(const TVwsViewId& aPrevViewId,TUid aCustomMessageId,const TDesC8& aCustomMessage)
{
  if(!iControl)
  {
    TRect rect=ClientRect();
    rect.iTl.iY=0;
    iControl=CPhiListPane::NewL(rect,this,iPath,iWhere,iSortMode);
    iPhiAppUi->AddToStackL(*this,iControl);
  }
  iControl->ActivateL();
}

void CPhiFilesView::DoDeactivate(void)
{
  if(iControl)
  {
    iPhiAppUi->RemoveFromStack(iControl);
    delete iControl;
    iControl=NULL;
  }
}

CPhiFilesView::CPhiFilesView(): CAknView(),iWhere(CPhiFs::ERoot),iSortMode(ESortByName)
{
}

void CPhiFilesView::ConstructL(void)
{
  BaseConstructL(R_PHI_FILE_VIEW);
}

void CPhiFilesView::DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane)
{
  iControl->DynInitMenuPaneL(aResourceId,aMenuPane);
}
