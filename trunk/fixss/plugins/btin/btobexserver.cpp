/*
    btobexserver.cpp
    Copyright (C) 2008 zg

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

#include "btobexserver.hpp"

_LIT(KObexDrive,"e:\\");
_LIT(KServerTransportName,"RFCOMM");

CBtListenBase::CBtListenBase(TUint aService): CBase(),iService(aService)
{
}

void CBtListenBase::ConstructL(TInt aChannel,TBool aAuthenticate)
{
  iBTConnection=CBTConnection::NewL();
  StartBTObexServerL(aChannel,aAuthenticate);
}

CBtListenBase::~CBtListenBase()
{
  TRAPD(err,StopBTObexServerL());
  delete iObexServer;
  delete iBTConnection;
  DeleteObject();
}

void CBtListenBase::StartBTObexServerL(TInt aChannel,TBool aAuthenticate)
{
  TObexBluetoothProtocolInfo obexProtocolInfo;
  obexProtocolInfo.iTransport.Copy(KServerTransportName);
  TInt ii=aChannel;
  for(;ii<30;ii++)
  {
    obexProtocolInfo.iAddr.SetPort(ii);
    TRAPD(err,iObexServer=CObexServer::NewL(obexProtocolInfo));
    if(err==KErrNone) break;
  }
  iBTConnection->RegisterSecuritySettingsL(iService,0x1013,ii,aAuthenticate,ETrue,EFalse);
  iBTConnection->RegisterSDPSettingsL(iService,ii,iSdpServRecordHandle);
  iObexServer->Start(this);
}

void CBtListenBase::StopBTObexServerL(void)
{
  if(iBTConnection)
  {
    iBTConnection->UnregisterSDPSettingsL(iSdpServRecordHandle);
    iBTConnection->UnregisterSecuritySettingsL(iService);
  }
  if(iObexServer)
  {
    delete iObexServer;
    iObexServer=NULL;
  }
}

void CBtListenBase::ErrorIndication(TInt aError)
{
  TRAPD(err,ResetObjectL());
}

void CBtListenBase::TransportUpIndication(void)
{
}

void CBtListenBase::TransportDownIndication(void)
{
}

TInt CBtListenBase::ObexConnectIndication(const TObexConnectInfo& aRemoteInfo,const TDesC8& aInfo)
{
  return KErrNone;
}

void CBtListenBase::ObexDisconnectIndication(const TDesC8& aInfo)
{
}

CObexBufObject* CBtListenBase::PutRequestIndication(void)
{
  if(iCurrObject) DeleteObject();
  TRAPD(err,iCurrObject=CObexBufObject::NewL(NULL));
  if(err==KErrNone)
  {
    TRAPD(err,iCurrObject->SetDataBufL(FileName()));
    if(err!=KErrNone) DeleteObject();
  }
  return iCurrObject;
}

TInt CBtListenBase::PutPacketIndication(void)
{
  return KErrNone;
}

TInt CBtListenBase::PutCompleteIndication(void)
{
  if(iCurrObject)
  {
    TFileName newName(KObexDrive);
    newName.Append(iCurrObject->Name());
    DeleteObject();
    RFs fs;
    if(fs.Connect()==KErrNone)
    {
      if(fs.Rename(FileName(),newName)!=KErrNone) fs.Delete(FileName());
    }
  }
  return KErrNone;
}

CObexBufObject* CBtListenBase::GetRequestIndication(CObexBaseObject* aRequiredObject)
{
  return NULL;
}

TInt CBtListenBase::GetPacketIndication(void)
{
  return KErrNone;
}

TInt CBtListenBase::GetCompleteIndication(void)
{
  return KErrNone;
}

TInt CBtListenBase::SetPathIndication(const CObex::TSetPathInfo& aPathInfo,const TDesC8& aInfo)
{
  return KErrNone;
}

void CBtListenBase::AbortIndication(void)
{
  TRAPD(err,ResetObjectL());
}

void CBtListenBase::ResetObjectL(void)
{
  if(iCurrObject)
  {
    iCurrObject->Reset();
    if(!iBuffer) iBuffer=CBufFlat::NewL(16);
    iCurrObject->SetDataBufL(iBuffer);
    RFs fs;
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);
    fs.Delete(FileName());
    CleanupStack::PopAndDestroy(); //fs
  }
}

void CBtListenBase::DeleteObject(void)
{
  delete iCurrObject;
  iCurrObject=NULL;
  delete iBuffer;
  iBuffer=NULL;
}

/*
void CBtListenBase::NewLC(void)
{
  CBtListenBase* self=new(ELeave)CBtListenBase;
  CleanupStack::PushL(self);
  self->ConstructL();
}
*/
void CBtListenObex::NewLC(void)
{
  CBtListenObex* self=new(ELeave)CBtListenObex(0x1105);
  CleanupStack::PushL(self);
  self->ConstructL(9,EFalse);
}

CBtListenObex::CBtListenObex(TUint aService): CBtListenBase(aService)
{
}

const TDesC& CBtListenObex::FileName(void)
{
  _LIT(KObexTempFile,"e:\\obex.temp");
  return KObexTempFile;
}

void CBtListenFtp::NewLC(void)
{
  CBtListenFtp* self=new(ELeave)CBtListenFtp(0x1106);
  CleanupStack::PushL(self);
  self->ConstructL(10,ETrue);
  _LIT8(KWho,"\xf9\xec\x7b\xc4\x95\x3c\x11\xd2\x98\x4e\x52\x54\x0\xdc\x9e\x9");
  User::LeaveIfError(self->Server().SetLocalWho(KWho));
}

CBtListenFtp::~CBtListenFtp()
{
  DeleteListObject();
}

CBtListenFtp::CBtListenFtp(TUint aService): CBtListenBase(aService)
{
}

const TDesC& CBtListenFtp::FileName(void)
{
  _LIT(KFtpTempFile,"e:\\ftp.temp");
  return KFtpTempFile;
}

CObexBufObject* CBtListenFtp::GetRequestIndication(CObexBaseObject* aRequiredObject)
{
  DeleteListObject();
  _LIT8(KType,"x-obex/folder-listing");
  _LIT8(KBody,"<?xml version=\"1.0\"?><!DOCTYPE folder-listing SYSTEM \"obex-folder-listing.dtd\"><folder-listing version=\"1.0\"></folder-listing>");
  if(aRequiredObject->Type().Compare(KType)==0)
  {
    iListObject=CObexBufObject::NewL(NULL);
    iListBuffer=CBufFlat::NewL(16);
    iListBuffer->ExpandL(0,256);
    iListBuffer->Write(0,KBody);
    iListObject->SetDataBufL(iListBuffer);
    iListObject->SetTypeL(KType);
  }
  return iListObject;
}

void CBtListenFtp::DeleteListObject(void)
{
  delete iListObject;
  iListObject=NULL;
  delete iListBuffer;
  iListBuffer=NULL;
}
