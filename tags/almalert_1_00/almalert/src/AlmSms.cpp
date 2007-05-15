/*
    AlmSms.cpp
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

#include "AlmUtils.hpp"
#include <msvapi.h> //MMsvSessionObserver
#include <mtclreg.h> //CClientMtmRegistry
#include <smsclnt.h> //CSmsClientMtm
#include <smut.h> //KUidMsgTypeSMS
#include <txtrich.h> //CRichText

class TDummyObserver: public MMsvSessionObserver
{
  public:
    void HandleSessionEventL(TMsvSessionEvent /*aEvent*/,TAny* /*aArg1*/,TAny* /*aArg2*/,TAny* /*aArg3*/) {};
};

void AlmUtils::SendMessageL(const TDesC& aFrom,const TDesC& aTo,const TDesC& aDescription,const TDesC& aBody)
{
  TDummyObserver ob;
  CMsvSession* session=CMsvSession::OpenSyncL(ob);
  CleanupStack::PushL(session);
  CClientMtmRegistry* registry=CClientMtmRegistry::NewL(*session);
  CleanupStack::PushL(registry);
  CSmsClientMtm* smsMtm=STATIC_CAST(CSmsClientMtm*,registry->NewMtmL(KUidMsgTypeSMS));
  CleanupStack::PushL(smsMtm);
  smsMtm->SwitchCurrentEntryL(KMsvGlobalInBoxIndexEntryId);
  smsMtm->CreateMessageL(KUidMsgTypeSMS.iUid);
  smsMtm->AddAddresseeL(aTo);
  CRichText& body=smsMtm->Body();
  body.Reset();
  body.InsertL(0,aBody);
  TMsvEntry entry=smsMtm->Entry().Entry();
  entry.SetInPreparation(EFalse);
  entry.SetVisible(ETrue);
  entry.iDate.HomeTime();
  entry.iDescription.Set(aDescription);
  entry.iDetails.Set(aFrom);
  entry.SetUnread(ETrue);
  smsMtm->Entry().ChangeL(entry);
  smsMtm->SaveMessageL();
  CleanupStack::PopAndDestroy(3); //smsMtm,registry,session
}
