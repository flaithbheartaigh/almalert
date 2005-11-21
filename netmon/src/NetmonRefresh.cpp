/*
    NetmonRefresh.cpp
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

#include "NetmonRefresh.hpp"

_LIT(KTsyName,"phonetsy.tsy");

CNetmonRefresh* CNetmonRefresh::NewL(void)
{
  CNetmonRefresh* self=new(ELeave)CNetmonRefresh;
  CleanupStack::PushL(self);
  self->ConstructL();
  CleanupStack::Pop(); //self
  return self;
}

CNetmonRefresh::~CNetmonRefresh()
{
  Cancel();
  iMessaging.Close();
  iPhone.Close();
  iServer.Close();
}

void CNetmonRefresh::Refresh(void)
{
  if(!IsActive())
  {
    iMessaging.SendUssdMessage(iStatus,iMsg);
    SetActive();
  }
}

void CNetmonRefresh::DoCancel(void)
{
  iMessaging.SendUssdMessageCancel();
}

void CNetmonRefresh::RunL(void)
{
}

CNetmonRefresh::CNetmonRefresh(): CActive(EPriorityIdle)
{
  CActiveScheduler::Add(this);
}

void CNetmonRefresh::ConstructL(void)
{
  User::LeaveIfError(iServer.Connect());
  User::LeaveIfError(iServer.LoadPhoneModule(KTsyName));
  TInt numberPhones;
  User::LeaveIfError(iServer.EnumeratePhones(numberPhones));
  if(numberPhones<1) User::Leave(KErrNotFound);
  RTelServer::TPhoneInfo info;
  User::LeaveIfError(iServer.GetPhoneInfo(0,info));
  User::LeaveIfError(iPhone.Open(iServer,info.iName));
  User::LeaveIfError(iMessaging.Open(iPhone));
  iMsg.iSendType=RAdvGsmSmsMessaging::EUssdMOCommand;
  iMsg.iDcs=0xf;
  iMsg.iMsg.Append('*');
}
