/*
    clockappdoc.cpp
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

#include "clockapp.hpp"
#include <s32file.h>
#include <AlmSettingsClient.hpp>

CClkDocument* CClkDocument::NewL(CEikApplication& aApp)
{
  CClkDocument* self=new(ELeave)CClkDocument(aApp);
  CleanupStack::PushL(self);
  self->ConstructL();
  CleanupStack::Pop(); //self
  return self;
}

CClkDocument::~CClkDocument()
{
  delete iAlm;
  delete iNitz;
}

CClkDocument::CClkDocument(CEikApplication& aApp): CAknDocument(aApp)
{
}

void CClkDocument::ConstructL(void)
{
  iAlm=CClkAlmModel::NewL(NULL,-5);
  iNitz=CClkNitzModel::NewL(NULL,-3);
}

CEikAppUi* CClkDocument::CreateAppUiL()
{
  CEikAppUi* appUi=new(ELeave)CClkAppUi;
  return appUi;
}
