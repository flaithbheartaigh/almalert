/*
    NetmonSetSC.cpp
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

#include "NetmonSetSC.hpp"

CNetmonSetSC* CNetmonSetSC::NewL(void)
{
  CNetmonSetSC* self=new(ELeave)CNetmonSetSC;
  CleanupStack::PushL(self);
  self->ConstructL();
  CleanupStack::Pop(); //self
  return self;
}

void CNetmonSetSC::DoCancel(void)
{
  iMessaging.SetDefaultSCAddressCancel();
}

void CNetmonSetSC::SetL(const TDesC& aSC)
{
  if(!IsActive())
  {
    iSc.iTypeOfNumber=0x91;
    iSc.iTelNumber=aSC;
    iMessaging.SetDefaultSCAddress(iStatus,iSc);
    SetActive();
  }
}

void CNetmonSetSC::GetL(TDes& aSC)
{
  TGsmTelNumber sc;
  User::LeaveIfError(iMessaging.GetDefaultSCAddress(sc));
  aSC=sc.iTelNumber;
}
