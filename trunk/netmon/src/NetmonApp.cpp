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
#include "NetmonFlashSms.hpp"
#include "NetmonSetSC.hpp"
#include "NetmonOwnNum.hpp"
#include "NetmonIMSI.hpp"
#include "netmon.hrh"

#include <aknquerydialog.h> //CAknTextQueryDialog
#include <hwtricks.hpp>
#include <netmon.rsg>

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
  iNetmonFlashSms=CNetmonFlashSms::NewL();
  iNetmonSetSC=CNetmonSetSC::NewL();
  iNetmonOwnNum=CNetmonOwnNum::NewL();
  iNetmonIMSI=CNetmonIMSI::NewL();
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
  delete iNetmonIMSI;
  delete iNetmonOwnNum;
  delete iNetmonSetSC;
  delete iNetmonFlashSms;
}

void CNetmonAppUi::HandleCommandL(TInt aCommand)
{
  switch(aCommand)
  {
    case ENetmonAbout:
      {
        TInt build;
        HWOther::InfoL(HWOther::EInfoBuild,build);
        TBuf16<128> about;
        about.Append(_L("Netmon ver. 0.90\n\x00a9 by zg\nhwtricks.dll build "));
        about.AppendNum(build);
        about.Append(_L("\nhttp://almalert.sf.net"));
        CMobilePhone::ShowResult(about);
      }
      break;
    case ENetmonIMSI:
      iNetmonIMSI->GetL();
      break;
    case ENetmonSetSCAddress:
      {
        TBuf<KGsmMaxTelNumberSize> phone;
        iNetmonSetSC->GetL(phone);
        CAknTextQueryDialog* dlg=CAknTextQueryDialog::NewL(phone);
        if(dlg->ExecuteLD(R_SET_SC)==EAknSoftkeyOk)
        {
          iNetmonSetSC->SetL(phone);
        }
      }
      break;
    case ENetmonSetOwnNumber:
      {
        TBuf<MAdvGsmPhoneOwnNumber::KSizeOfOwnNumberText> name;
        TBuf<KGsmMaxTelNumberSize> phone;
        iNetmonOwnNum->GetL(name,phone);
        CAknMultiLineDataQueryDialog* dlg=CAknMultiLineDataQueryDialog::NewL(name,phone);
        if(dlg->ExecuteLD(R_OWN_NUMBER)==EAknSoftkeyOk)
        {
          iNetmonOwnNum->SetL(name,phone);
        }
      }
      break;
    case ENetmonSendFlashSms:
      {
        TBuf<CNetmonFlashSms::KRawPhoneNumberSize> phone;
        TBuf<CNetmonFlashSms::KSmsBodySize> body;
        CAknMultiLineDataQueryDialog* dlg=CAknMultiLineDataQueryDialog::NewL(phone,body);
        if(dlg->ExecuteLD(R_SMS_SEND)==EAknSoftkeyOk)
        {
          iNetmonFlashSms->SendL(phone,body);
        }
      }
      break;
    case EEikCmdExit:
    case EAknSoftkeyExit:
      Exit();
      break;
    default:
      break;
  }
}
