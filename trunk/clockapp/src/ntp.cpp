/*
    ntp.cpp
    Copyright (C) 2007 zg

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

#include "ntp.hpp"

void CNtp::NewLD(const TDesC& aServer,TInt aPort,TTimeIntervalSeconds aCorrection)
{
  CNtp* self=new(ELeave)CNtp(aServer,aPort,aCorrection);
  CleanupStack::PushL(self);
  self->ConstructL();
  CleanupStack::Pop(); //self
}

CNtp::~CNtp()
{
  iResolver.Close();
  iSocket.Close();
  iSocketServ.Close();
}

void CNtp::TimerExpired(void)
{
}

void CNtp::DoCancel(void)
{
  switch(iNtpState)
  {
    case ELookingUp:
      iResolver.Cancel();
      break;
    case ESending:
      break;
    case EReceiving:
      break;
    default:
      User::Invariant();
      break;
  }
  delete this;
}

void CNtp::RunL(void)
{
  TBool stop=ETrue;
  switch(iNtpState)
  {
    case ELookingUp:
      if(iStatus==KErrNone)
      {
        iAddress.SetPort(iPort);
        iAddress.SetAddress(TInetAddr::Cast(iNameEntry().iAddr).Address());
        iSendBuffer.SetLength(0);
        iSendBuffer.Append((KNtpVersion<<3)|KNtpModeClient);
        for(TInt i=1;i<KNtpPacketMin;i++) iSendBuffer.Append(0);
        iSocket.SendTo(iSendBuffer,iAddress,0,iStatus);
        iNtpState=ESending;
        SetActive();
        stop=EFalse;
      }
      break;
    case ESending:
      if(iStatus==KErrNone)
      {
        iSendStamp.HomeTime();
        iSocket.RecvFrom(iReceiveBuffer,iAddress,0,iStatus);
        iNtpState=EReceiving;
        SetActive();
        stop=EFalse;
      }
      break;
    case EReceiving:
      if(iStatus==KErrNone&&iReceiveBuffer.Length()>=KNtpPacketMin)
      {
        iReceiveStamp.HomeTime();
        TInt64 stamp(0),base(0xd504a2,0xc672e000);
        for(TInt i=0;i<4;i++)
        {
          stamp=stamp*256+(TUint)iReceiveBuffer[i+32];
        }
        stamp=stamp*1000000+base;
        TTime time(stamp);
        time+=iTimeOffset;
        time+=iCorrection;
        TTimeIntervalSeconds way;
        if(iReceiveStamp.SecondsFrom(iSendStamp,way)==KErrNone)
        {
          way=way.Int()/2;
          time+=way;
        }
        User::SetHomeTime(time);
      }
      break;
    default:
      User::Invariant();
      break;
  }
  if(stop) delete this;
}

CNtp::CNtp(const TDesC& aServer,TInt aPort,TTimeIntervalSeconds aCorrection): CActive(EPriorityStandard),iServerName(aServer),iPort(aPort),iCorrection(aCorrection)
{
  TLocale locale;
  iTimeOffset=locale.UniversalTimeOffset();
}

void CNtp::ConstructL(void)
{
  CActiveScheduler::Add(this);
  User::LeaveIfError(iSocketServ.Connect());
  User::LeaveIfError(iSocket.Open(iSocketServ,KAfInet,KSockDatagram,KProtocolInetUdp));
  User::LeaveIfError(iResolver.Open(iSocketServ,KAfInet,KProtocolInetUdp));
  iResolver.GetByName(iServerName,iNameEntry,iStatus);
  iNtpState=ELookingUp;
  SetActive();
}
