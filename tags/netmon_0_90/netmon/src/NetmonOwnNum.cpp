/*
    NetmonOwnNum.cpp
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

#include "NetmonOwnNum.hpp"

CNetmonOwnNum* CNetmonOwnNum::NewL(void)
{
  CNetmonOwnNum* self=new(ELeave)CNetmonOwnNum;
  CleanupStack::PushL(self);
  self->ConstructL();
  CleanupStack::Pop(); //self
  return self;
}

void CNetmonOwnNum::DoCancel(void)
{
  iPhone.SetOwnNumberEntryCancel();
}

void CNetmonOwnNum::SetL(const TDesC& aName,const TDesC& aPhone)
{
  if(!IsActive())
  {
    iOwn.iText=aName;
    iOwn.iTelNumber.iTypeOfNumber=0x91;
    iOwn.iTelNumber.iTelNumber=aPhone;
    iOwn.iInfoTransferCap=MAdvGsmPhoneOwnNumber::EITCVoice;
    iOwn.iBearerSpeed=0;
    iOwn.iBearerService=MAdvGsmPhoneOwnNumber::EServiceAsynchronousModem;
    iOwn.iBearerCE=MAdvGsmPhoneOwnNumber::EBearerCETransparent;
    iPhone.SetOwnNumberEntry(iStatus,0,iOwn);
    SetActive();
  }
}

void CNetmonOwnNum::GetL(TDes& aName,TDes& aPhone)
{
  MAdvGsmPhoneOwnNumber::TOwnNumberEntry own;
  if(iPhone.GetOwnNumberEntry(0,own)==KErrNone)
  {
    aName=own.iText;
    aPhone=own.iTelNumber.iTelNumber;
  }
  else
  {
    aName.Zero();
    aPhone.Zero();
  }
}
