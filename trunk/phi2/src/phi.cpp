/*
    phi.cpp
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
#include "img/phiimage.hpp"

static const TUid KUidPhiApp={0x10204D95};

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
{
  return KErrNone;
}

EXPORT_C CApaApplication* NewApplication()
{
  return new CPhiApplication;
}

TUid CPhiApplication::AppDllUid() const
{
  return KUidPhiApp;
}

CApaDocument* CPhiApplication::CreateDocumentL()
{
  return CPhiDocument::NewL(*this);
}

CPhiDocument* CPhiDocument::NewL(CEikApplication& aApp)
{
  return new(ELeave)CPhiDocument(aApp);
}

CPhiDocument::CPhiDocument(CEikApplication& aApp): CAknDocument(aApp)
{
}

CEikAppUi* CPhiDocument::CreateAppUiL()
{
  return new(ELeave)CPhiAppUi;
}

void CPhiAppUi::ConstructL(void)
{
  BaseConstructL();
  SetKeyBlockMode(ENoKeyBlock);
  CEikStatusPane* status=iEikonEnv->AppUiFactory()->StatusPane();
  if(status) status->MakeVisible(EFalse);
  CAknView* view=CPhiFilesView::NewLC();
  AddViewL(view);
  CleanupStack::Pop(); //view
  view=CPhiImageView::NewLC();
  AddViewL(view);
  CleanupStack::Pop(); //view
}

void CPhiAppUi::HandleCommandL(TInt aCommand)
{
  switch(aCommand)
  {
    case EEikCmdExit:
    case EAknCmdExit:
      Exit();
      break;
  }
}

void CPhiAppUi::CmdShowImageL(const TDesC& aFileName)
{
  static_cast<CPhiImageView*>(View(KPhiImageViewId))->SetFileName(aFileName);
  ActivateLocalViewL(KPhiImageViewId);
}

void CPhiAppUi::CmdBackL(void)
{
  ActivateLocalViewL(KPhiFileViewId);
}
