/*
    NetmonApp.cpp
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

#include "NetmonApp.hpp"
#include "NetmonContainer.hpp"

//Netmon global
GLDEF_C TInt E32Dll(TDllReason)
{
  return KErrNone;
}

EXPORT_C CApaApplication* NewApplication()
{
  return new CNetmonApplication;
}

//CNetmonApplication
static const TUid KUidNetmonApp={0xa00001d3};

CApaDocument* CNetmonApplication::CreateDocumentL()
{
  CApaDocument* document=CNetmonDocument::NewL(*this);
  return document;
}

TUid CNetmonApplication::AppDllUid() const
{
  return KUidNetmonApp;
}

//CNetmonDocument
CNetmonDocument* CNetmonDocument::NewL(CEikApplication& aApp)
{
  CNetmonDocument* self=new(ELeave)CNetmonDocument(aApp);
  return self;
}

CNetmonDocument::CNetmonDocument(CEikApplication& aApp): CAknDocument(aApp)
{
}

CEikAppUi* CNetmonDocument::CreateAppUiL()
{
  CEikAppUi* appUi=new(ELeave)CNetmonAppUi;
  return appUi;
}

//CNetmonAppUi
void CNetmonAppUi::ConstructL(void)
{
  BaseConstructL();
  iContainer=CNetmonContainer::NewL();
  AddToStackL(iContainer);
}

CNetmonAppUi::CNetmonAppUi(): CAknAppUi()
{
}

CNetmonAppUi::~CNetmonAppUi()
{
  if(iContainer)
  {
    RemoveFromStack(iContainer);
    delete iContainer;
  }
}

void CNetmonAppUi::HandleCommandL(TInt aCommand)
{
  switch(aCommand)
  {
    case EEikCmdExit:
    case EAknSoftkeyExit:
      Exit();
      break;
    default:
      break;
  }
}
