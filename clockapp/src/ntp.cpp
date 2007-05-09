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
#include <clockapp.rsg>
#include <aknnotewrappers.h>

void CNtp::NewLD(const TDesC& aServer,TInt aPort,TTimeIntervalSeconds aCorrection)
{
  CNtp* self=new(ELeave)CNtp(aServer,aPort,aCorrection);
  CleanupStack::PushL(self);
  self->ConstructL();
  CleanupStack::Pop(); //self
}

CNtp::~CNtp()
{
  TRAPD(err,CleanupL());
  Cancel();
  iResolver.Close();
  iSocket.Close();
  iSocketServ.Close();
}

void CNtp::DialogDismissedL(TInt aButtonId)
{
  if(IsActive())
  {
    iWaitDialog=NULL; //dont' call ProcessFinishedL in in CleanupL
    Cancel();
    iStatus=KErrCancel;
    delete this;
  }
}

void CNtp::DoCancel(void)
{
  switch(iNtpState)
  {
    case ELookingUp:
      iResolver.Cancel();
      break;
    case ESending:
    case EReceiving:
      iSocket.CancelAll();
      break;
    default:
      User::Invariant();
      break;
  }
  iNtpState=EIdle;
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
        TInt status=(iReceiveBuffer[0]>>6)&KNtpStatusAlarm,mode=(iReceiveBuffer[0]&0x07);
        if(status||mode!=KNtpModeServer)
        {
          if(!status) status=KNtpModeServer;
          iStatus=KNtpErrorBase-status;
        }
        else
        {
          TInt64 stamp(0),base(0xd504a2,0xc672e000);
          for(TInt i=0;i<4;i++)
          {
            stamp=stamp*256+(TUint)iReceiveBuffer[i+32];
          }
          stamp=stamp*1000000+base;
          iNewStamp=stamp;
          iNewStamp+=iTimeOffset;
          iNewStamp+=iCorrection;
          TTimeIntervalSeconds way;
          if(iReceiveStamp.SecondsFrom(iSendStamp,way)==KErrNone)
          {
            way=way.Int()/2;
            iNewStamp+=way;
          }
          User::SetHomeTime(iNewStamp);
        }
      }
      break;
    default:
      User::Invariant();
      break;
  }
  if(stop)
  {
    iNtpState=EIdle;
    delete this;
  }
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
  iWaitDialog=new(ELeave)CAknWaitDialog(reinterpret_cast<CEikDialog**>(&iWaitDialog),ETrue);
  iWaitDialog->SetTone(CAknNoteDialog::ENoTone);
  iWaitDialog->SetCallback(this);
  iWaitDialog->ExecuteLD(R_CLOCKAPP_WAIT_NOTE_SOFTKEY_CANCEL);
}

void CNtp::CleanupL(void)
{
  const TInt KNtpErrors[]={R_CLOCKAPP_NTP_ERROR1,R_CLOCKAPP_NTP_ERROR2,R_CLOCKAPP_NTP_ERROR3,R_CLOCKAPP_NTP_ERROR4};
  if(iWaitDialog) iWaitDialog->ProcessFinishedL();
  CCoeEnv* env=CCoeEnv::Static();
  TBuf<128> message;
  if(iStatus==KErrNone)
  {
    HBufC* format=env->AllocReadResourceAsDes16LC(R_QTN_TIME_LONG_WITH_ZERO);
    TBuf<64> title;
    env->ReadResourceAsDes16(title,R_CLOCKAPP_NTP_OK);
    TBuf<29> before,after;
    iReceiveStamp.FormatL(before,*format);
    iNewStamp.FormatL(after,*format);
    message.Format(title,&before,&after);
    CleanupStack::PopAndDestroy(); //format
  }
  else
  {
    TInt status=KNtpErrorBase-iStatus.Int();
    if(status>0&&status<=KNtpModeServer)
    {
      env->ReadResourceAsDes16(message,KNtpErrors[status-1]);
    }
    else
    {
      TBuf<64> title;
      env->ReadResourceAsDes16(title,R_CLOCKAPP_NTP_ERROR);
      message.Format(title,iStatus.Int());
    }
  }
  CAknInformationNote* dlg=new(ELeave)CAknInformationNote;
  dlg->SetTimeout(CAknNoteDialog::ENoTimeout);
  dlg->ExecuteLD(message);
}
