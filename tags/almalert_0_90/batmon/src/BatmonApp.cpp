/*
    BatmonApp.cpp
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

#include "BatmonApp.hpp"
#include "BatmonContainer.hpp"
#include <aknnotewrappers.h> //CAknInformationNote
#include <hwtricks.hpp>

//Batmon global
GLDEF_C TInt E32Dll(TDllReason)
{
  return KErrNone;
}

EXPORT_C CApaApplication* NewApplication()
{
  return new CBatmonApplication;
}

//CBatmonApplication
static const TUid KUidBatmonApp={0xa0000547};

CApaDocument* CBatmonApplication::CreateDocumentL()
{
  CApaDocument* document=CBatmonDocument::NewL(*this);
  return document;
}

TUid CBatmonApplication::AppDllUid() const
{
  return KUidBatmonApp;
}

//CBatmonDocument
CBatmonDocument* CBatmonDocument::NewL(CEikApplication& aApp)
{
  CBatmonDocument* self=new(ELeave)CBatmonDocument(aApp);
  return self;
}

CBatmonDocument::CBatmonDocument(CEikApplication& aApp): CAknDocument(aApp)
{
}

CEikAppUi* CBatmonDocument::CreateAppUiL()
{
  CEikAppUi* appUi=new(ELeave)CBatmonAppUi;
  return appUi;
}

//CBatmonAppUi
void CBatmonAppUi::ConstructL(void)
{
  BaseConstructL();
  iContainer=CBatmonContainer::NewL();
  AddToStackL(iContainer);
}

CBatmonAppUi::CBatmonAppUi(): CAknAppUi()
{
}

CBatmonAppUi::~CBatmonAppUi()
{
  if(iContainer)
  {
    RemoveFromStack(iContainer);
    delete iContainer;
  }
}

void CBatmonAppUi::HandleCommandL(TInt aCommand)
{
  switch(aCommand)
  {
    case EAknSoftkeyOk:
      {
        TInt build;
        HWOther::InfoL(HWOther::EInfoBuild,build);
        TBuf16<128> About;
        About.Append(_L("Batmon ver. 1.00\n\x00a9 by zg\nhwtricks.dll build "));
        About.AppendNum(build);
        About.Append(_L("\nhttp://almalert.sf.net"));
        CAknInformationNote* dlg=new(ELeave)CAknInformationNote;
        dlg->SetTimeout(CAknNoteDialog::ENoTimeout);
        dlg->ExecuteLD(About);
      }
      break;
    case EAknSoftkeyCancel:
    case EEikCmdExit:
    case EAknSoftkeyExit:
      Exit();
      break;
    default:
      break;
  }
}
