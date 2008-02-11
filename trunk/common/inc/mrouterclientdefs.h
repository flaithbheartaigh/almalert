/*
    mrouterclientdefs.h
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

#ifndef __MROUTERCLIENTDEFS_H__
#define __MROUTERCLIENTDEFS_H__

#include <nifman.h>
#include <btregistry.h>

enum TBearer
{
  EmR_RS232,
  EmR_IRCOMM,
  EmR_RFCOMM,
  EmR_USB,
  EmR_EMULATOR,
  EmR_OTA,
  EmR_Bearer_Unknown
};

enum TBTProfile
{
  EBT_SerialPort=0x1101,
  EBT_LANAccess=0x1102,
  EBT_DialupNetworking=0x1103,
};

enum TRoutePref
{
  EmR_RouteGenConn,
  EmR_RoutemRouter
};

const TInt KAttemptForever=KMaxTInt;

struct TmRConfig
{
  enum TBearer iBearer;
  enum TBTProfile iBTProfile;
  TBTDevAddr iBDAddr;
  enum TRoutePref iRoutePref;
  TInfoName iUser;
  TPassword iPass;
  THostName iHost;
  TUint iPort;
  TUint iBearerUnit;
  TUint iBearerSpeed;
};

enum TmRSimpleConnectState
{
  EmRConnecting,
  EmRConnected,
  EmRDisconnected
};

struct TmRConnectState
{
  TmRSimpleConnectState iConnectState;
  TNifProgress iNifProgress;
};

struct TmRStatus
{
  TBearer iBearer;
  TmRConnectState iConnectState;
};

enum TBTListenStatus {EStopped,EStarted,ENotSupported};

const TInt KECTcpAdapterPort=3000;
_LIT(KECTcpAdapterName,"ECTCPADAPTER");
const TInt KContProPort=3002;
_LIT(KContProName,"CONTPRO");
const TInt KBackupProPort=3003;
_LIT(KBackupProName,"BACKUPPRO");

#endif
