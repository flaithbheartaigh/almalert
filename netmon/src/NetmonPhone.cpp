/*
    NetmonPhone.cpp
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

#include "NetmonPhone.hpp"
#include <aknnotewrappers.h> //CAknInformationNote

_LIT(KTsyName,"phonetsy.tsy");

CMobilePhone::~CMobilePhone()
{
  Cancel();
  Close();
}

void CMobilePhone::RunL(void)
{
  TBuf16<128> about;
  if(iStatus==KErrNone)
  {
    about.Append(_L("Success."));
  }
  else
  {
    about.Append(_L("Error ("));
    about.AppendNum(iStatus.Int());
    about.Append(_L(")."));
  }
  CAknInformationNote* dlg=new(ELeave)CAknInformationNote;
  dlg->SetTimeout(CAknNoteDialog::ENoTimeout);
  dlg->ExecuteLD(about);
}

void CMobilePhone::Close()
{
  iPhone.Close();
  iServer.Close();
}

void CMobilePhone::ConstructL(void)
{
  User::LeaveIfError(iServer.Connect());
  User::LeaveIfError(iServer.LoadPhoneModule(KTsyName));
  TInt numberPhones;
  User::LeaveIfError(iServer.EnumeratePhones(numberPhones));
  if(numberPhones<1) User::Leave(KErrNotFound);
  RTelServer::TPhoneInfo info;
  User::LeaveIfError(iServer.GetPhoneInfo(0,info));
  User::LeaveIfError(iPhone.Open(iServer,info.iName));
}

CMobilePhone::CMobilePhone(): CActive(EPriorityIdle)
{
  CActiveScheduler::Add(this);
}

void CMessaging::Close()
{
  iMessaging.Close();
  CMobilePhone::Close();
}

void CMessaging::ConstructL(void)
{
  CMobilePhone::ConstructL();
  User::LeaveIfError(iMessaging.Open(iPhone));
}
