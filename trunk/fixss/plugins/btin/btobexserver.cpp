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

_LIT(KObexTempFile,"e:\\obex.temp");
_LIT(KObexDrive,"e:\\");
_LIT(KServerTransportName,"RFCOMM");

CBtListenActive::CBtListenActive(): CBase()
{
}

void CBtListenActive::ConstructL(void)
{
  iBTConnection=CBTConnection::NewL();
  StartBTObexServerL();
}

void CBtListenActive::NewLC(void)
{
  CBtListenActive* self=new(ELeave)CBtListenActive;
  CleanupStack::PushL(self);
  self->ConstructL();
}

CBtListenActive::~CBtListenActive()
{
  TRAPD(err,StopBTObexServerL());
  delete iObexServer;
  delete iBTConnection;
  DeleteObject();
}

void CBtListenActive::StartBTObexServerL(void)
{
  TObexBluetoothProtocolInfo obexProtocolInfo;
  obexProtocolInfo.iTransport.Copy(KServerTransportName);
  TInt ii=9;
  for(;ii<30;ii++)
  {
    obexProtocolInfo.iAddr.SetPort(ii);
    TRAPD(err,iObexServer=CObexServer::NewL(obexProtocolInfo));
    if(err==KErrNone) break;
  }
  iBTConnection->RegisterSecuritySettingsL(0x1105,0x1013,ii,EFalse,ETrue,EFalse);
  iBTConnection->RegisterSDPSettingsL(0x1105,ii,iSdpServRecordHandle);
  iObexServer->Start(this);
}

void CBtListenActive::StopBTObexServerL(void)
{
  if(iBTConnection)
  {
    iBTConnection->UnregisterSDPSettingsL(iSdpServRecordHandle);
    iBTConnection->UnregisterSecuritySettingsL(0x1105);
  }
  if(iObexServer)
  {
    delete iObexServer;
    iObexServer=NULL;
  }
}

void CBtListenActive::ErrorIndication(TInt aError)
{
  TRAPD(err,ResetObjectL());
}

void CBtListenActive::TransportUpIndication(void)
{
}

void CBtListenActive::TransportDownIndication(void)
{
}

TInt CBtListenActive::ObexConnectIndication(const TObexConnectInfo& aRemoteInfo,const TDesC8& aInfo)
{
  return KErrNone;
}

void CBtListenActive::ObexDisconnectIndication(const TDesC8& aInfo)
{
}

CObexBufObject* CBtListenActive::PutRequestIndication(void)
{
  if(iCurrObject) DeleteObject();
  TRAPD(err,iCurrObject=CObexBufObject::NewL(NULL));
  if(err==KErrNone)
  {
    TRAPD(err,iCurrObject->SetDataBufL(KObexTempFile()));
    if(err!=KErrNone) DeleteObject();
  }
  return iCurrObject;
}

TInt CBtListenActive::PutPacketIndication(void)
{
  return KErrNone;
}

TInt CBtListenActive::PutCompleteIndication(void)
{
  if(iCurrObject)
  {
    TFileName newName(KObexDrive);
    newName.Append(iCurrObject->Name());
    DeleteObject();
    RFs fs;
    if(fs.Connect()==KErrNone)
    {
      if(fs.Rename(KObexTempFile,newName)!=KErrNone) fs.Delete(KObexTempFile);
    }
  }
  return KErrNone;
}

CObexBufObject* CBtListenActive::GetRequestIndication(CObexBaseObject* aRequiredObject)
{
  return NULL;
}

TInt CBtListenActive::GetPacketIndication(void)
{
  return KErrNone;
}

TInt CBtListenActive::GetCompleteIndication(void)
{
  return KErrNone;
}

TInt CBtListenActive::SetPathIndication(const CObex::TSetPathInfo& aPathInfo,const TDesC8& aInfo)
{
  return KErrNone;
}

void CBtListenActive::AbortIndication(void)
{
  TRAPD(err,ResetObjectL());
}

void CBtListenActive::ResetObjectL(void)
{
  if(iCurrObject)
  {
    iCurrObject->Reset();
    if(!iBuffer) iBuffer=CBufFlat::NewL(16);
    iCurrObject->SetDataBufL(iBuffer);
    RFs fs;
    User::LeaveIfError(fs.Connect());
    CleanupClosePushL(fs);
    fs.Delete(KObexTempFile);
    CleanupStack::PopAndDestroy(); //fs
  }
}

void CBtListenActive::DeleteObject(void)
{
  delete iCurrObject;
  iCurrObject=NULL;
  delete iBuffer;
  iBuffer=NULL;
}
