/*
    NetmonFlashSms.cpp
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

#include "NetmonFlashSms.hpp"

CNetmonFlashSms* CNetmonFlashSms::NewL(void)
{
  CNetmonFlashSms* self=new(ELeave)CNetmonFlashSms;
  CleanupStack::PushL(self);
  self->ConstructL();
  CleanupStack::Pop(); //self
  return self;
}

void CNetmonFlashSms::SendL(const TDesC& aPhone,const TDesC& aBody)
{
  if(!IsActive()&&iSc.Length())
  {
    TSms::TPdu pdu;
    AddPhone(pdu,iSc);

    pdu.Append(0x11);                     // First octet of the SMS-SUBMIT message.
    pdu.Append(0);                        // TP-Message-Reference.
    AddPhone(pdu,aPhone);
    pdu.Append(0);                        // TP-PID.
    pdu.Append(0x18);                     // TP-DCS. 16 bits (UCS2), message class 0.
    pdu.Append(0xaa);                     // TP-VP. 4 days.
    TInt length=aBody.Length();
    pdu.Append(length*2);                 // TP-UDL
    for(TInt i=0;i<length;i++)            // TP-UD.
    {
      TUint16 symbol=aBody[i];
      pdu.Append(symbol>>8);
      pdu.Append(symbol&0xff);
    }
    iMsg.SetPduL(pdu);
    iMsg.SetUseDefaultSca(EFalse);
    iMessaging.SendAdvSmsMessage(iStatus,iMsg,iMsgResult);
    SetActive();
  }
}

void CNetmonFlashSms::DoCancel(void)
{
  iMessaging.SendMessageCancel();
}

void CNetmonFlashSms::ConstructL(void)
{
  CMessaging::ConstructL();
  TGsmTelNumber sc;
  User::LeaveIfError(iMessaging.GetDefaultSCAddress(sc));
  iSc.Copy(sc.iTelNumber);
  iParser=NewPhoneNumberParser();
}

void CNetmonFlashSms::AddPhone(TDes8& aPdu,const TDesC& aPhone)
{
  TBuf<KRawPhoneNumberSize> phone;
  iParser->ExtractRawNumber(aPhone,phone);
  aPdu.Append(phone.Length());
  aPdu.Append(0x91);
  if(phone.Length()&1) phone.Append('?');
  TInt length=phone.Length()/2;
  for(TInt i=0;i<length;i++)
  {
    aPdu.Append(phone[i*2]-'0'+(phone[i*2+1]-'0')*16);
  }
}
