/*
    BtEng.hpp
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

#ifndef __BTENG_HPP__
#define __BTENG_HPP__

#include <CommonEngine.hpp>
#include <bttypes.h>
#include <btdevice.h>
#include <btsdp.h>
#include <es_sock.h>
#include <btmanclient.h>

enum TBTDiscoverabilityMode
{
  EBTDiscoverabilityModeHidden,
  EBTDiscoverabilityModeLimited,
  EBTDiscoverabilityModeShownToAll
};

enum TBTSearchMode
{
  EBTSearchModeLimited,
  EBTSearchModeNormal
};

class MBTMCMSettingsCB
{
  public:
    virtual void DiscoverabilityModeNotify(TBTDiscoverabilityMode aDiscoverabilityMode)=0;
    virtual void PowerStateNotify(TBool aPowerState)=0;
    virtual void AddressNotify(TBTDevAddr& anAddress)=0;
    virtual void ConnectionStatusNotify(TBool& aConnectionStatus)=0;
};

class CBTMCMSettings: public CBase,public MSharedDataNotifyHandler
{
  public:
    ~CBTMCMSettings();
    IMPORT_C static CBTMCMSettings* NewLC(MBTMCMSettingsCB *aCallback=NULL);
    IMPORT_C static CBTMCMSettings* NewL(MBTMCMSettingsCB *aCallback=NULL);
  public:
    IMPORT_C static TInt GetAllSettings(TBool& aPowerState,TBTDiscoverabilityMode& aDiscoverabilityMode,TBTSearchMode& aSearchMode,TDes16& aLocalBTName,TBool& aIsLocalNameModified);
    IMPORT_C TInt SetDefaultValuesL(void);
    IMPORT_C static TInt IsLocalNameModified(TBool& aIsLocalNameModified);
    IMPORT_C static TInt GetLocalBTName(TDes16& aLocalBTName);
    IMPORT_C TInt SetLocalBTName(const TDesC16& aLocalBTName);
    IMPORT_C TInt SetDiscoverabilityModeL(TBTDiscoverabilityMode aDiscoverabilityMode,TBool aSkip=EFalse);
    IMPORT_C static TInt GetDiscoverabilityModeL(TBTDiscoverabilityMode& aDiscoverabilityMode);
    IMPORT_C TInt SetSearchModeL(TBTSearchMode aSearchMode);
    IMPORT_C static TInt GetSearchModeL(TBTSearchMode& aSearchMode);
    IMPORT_C TInt SetPowerStateL(TBool aPowerState,TBool aKeepAliveExistings=EFalse);
    IMPORT_C static TInt GetPowerStateL(TBool& aPowerState);
    IMPORT_C static TInt GetLocalBDAddress(TBTDevAddr& anAddress);
    IMPORT_C TInt GetAccessoryAddr(TBTDevAddr& anAddress);
    IMPORT_C TInt GetConnectionStatus(TBool& aConnectionStatus);
    IMPORT_C TInt IsWiredAccConnected(TBool& aConnected);
    IMPORT_C TInt IsCallOngoing(TBool& aCallOngoing);
  public: //MSharedDataNotifyHandler
    void HandleNotifyL(TUid anUid,const TDesC16& aKey,const TDesC16& aValue);
  private:
    CBTMCMSettings();
    void ConstructL(MBTMCMSettingsCB *aCallback);
  private:
    static TInt GetPowerState(RSharedDataClient& aShared,TBool& aPowerState);
    static TInt GetDiscoverabilityMode(RSharedDataClient& aShared,TBTDiscoverabilityMode& aDiscoverabilityMode);
    static TInt GetSearchMode(RSharedDataClient& aShared,TBTSearchMode& aSearchMode);
    static TInt GetLocalBTName(RSharedDataClient& aShared,TDes16& aLocalBTName);
    static TInt IsLocalNameModified(RSharedDataClient& aShared,TBool& aIsLocalNameModified);
  private:
    MBTMCMSettingsCB* iCallback;
    RSharedDataClient iShared;
};

class RBTDeviceHandler
{
  public:
    IMPORT_C TInt AddDeviceIntoRegistryL( CBTDevice& aDevice );
    IMPORT_C TInt GetDevicesFromRegistryL( CBTDeviceArray& aDevices );
    IMPORT_C TInt DeleteDeviceFromRegistryL( CBTDevice& aDevice );
    IMPORT_C TInt ModifyDeviceInRegistryL( CBTDevice& aDevice );
    IMPORT_C TInt SetDeviceSecurityL(CBTDevice& aDevice,TBool aAuthenticate,TBool aAuthorise,TBool aEncrypt);
    IMPORT_C TInt RemoveTrustFromDevicesL(void);
    IMPORT_C TInt DeleteDevicesWithoutLinkKeyL(void);
    IMPORT_C TInt DeleteAllDevicesL(void);
    IMPORT_C TInt IsDeviceConnectedL(const CBTDevice& aDevice,TBool& aStatus);
    IMPORT_C TInt AreExistingConnectionsL(TBool& aStatus);
    IMPORT_C TInt CloseBasebandConnectionL(const CBTDevice& aDevice);
    IMPORT_C TInt CloseAllBasebandConnectionsL(void);
    IMPORT_C TInt GetOpenBasebandConnectionsL(CBTDeviceArray& aDevices);
  private:
    TInt GetBTAddressArrayL(TUint& aLinkCount,TBTDevAddr*& aLinkArray);
    void AddDeviceIntoListL(CBTDevice* aDevice,CBTDeviceArray* aDeviceArray);
};

class MBTSdpSearchComplete
{
  public:
    virtual void ReturnChannel(TUint aChannel,TInt aStatus)=0;
    virtual void SearchComplete(TInt aStatus)=0;
};

class MBTDeviceSearchObserver
{
  public:
    virtual void DeviceSearchComplete(TInt aStatus)=0;
};

class MBTBondingObserver
{
  public:
    virtual void BondingCompleteL(TInt aStatus)=0;
};

class CBTBondingHandler;
class CBTSdpDbHandler;
class CBTSdpResultReceiver;

class CBTConnection: public CBase,public MBTSdpSearchComplete,public MBTDeviceSearchObserver
{
  public:
    IMPORT_C static CBTConnection* NewL(void);
    IMPORT_C static CBTConnection* NewLC(void);
    ~CBTConnection();
    IMPORT_C TInt SearchRemoteDeviceL(CBTDevice& aDevice,TBTMajorServiceClass aServiceClass);
    IMPORT_C TInt SearchRemoteDeviceL(CBTDevice& aDevice);
    IMPORT_C TInt SearchRemoteChannelL(const CBTDevice& aDevice,TUint aService,TUint& aChannel);
    IMPORT_C TInt SearchRemoteChannelL(const CBTDevice& aDevice,TUint aService,TUint& aChannel,TRequestStatus& aStatus);
    IMPORT_C TInt RegisterSecuritySettingsL(TUint aService,TInt aProtocol,TInt aChannel,TBool aAuthenticate,TBool aAuthorise,TBool aEncrypt);
    IMPORT_C TInt UnregisterSecuritySettingsL(TUint aService);
    IMPORT_C TInt RegisterSDPSettingsL(TUint aService,TUint aChannel,TSdpServRecordHandle& aHandle);
    IMPORT_C TInt UnregisterSDPSettingsL(TSdpServRecordHandle aHandle);
    IMPORT_C TInt ReserveLocalChannelL(RSocketServ& aSocketServer,RSocket& aListenSocket,TUint& aChannel);
    IMPORT_C TInt ReleaseLocalChannelL(RSocket& aListenSocket);
    IMPORT_C TInt PassiveConnectSocketL(RSocketServ& aSocketServer,RSocket& aDataSocket,RSocket& aListenSocket,TUint& aChannel,TRequestStatus& aStatus);
    IMPORT_C TInt ActiveConnectSocketL(RSocketServ& aSocketServer,RSocket& aSocket,CBTDevice& aDevice,TUint aChannel,TRequestStatus& aStatus);
    IMPORT_C TInt ActiveConnectSocketL(RSocketServ& aSocketServer,RSocket& aSocket,TInt aProtocol,CBTDevice& aDevice,TUint aChannel,TRequestStatus& aStatus);
    IMPORT_C TInt GetRemoteDeviceInfoL(RSocket& aSocket,CBTDevice& aDevice);
    IMPORT_C TInt DisconnectSocketL(RSocket& asocket,RSocket::TShutdown aHow=RSocket::ENormal);
    IMPORT_C void AuthenticateConnection(RSocket& aSocket,TRequestStatus& aStatus);
    IMPORT_C void CancelAuthentication(RSocket& aSocket);
    IMPORT_C void CancelConnection(RSocket& aSocket);
    IMPORT_C TInt PerformBondingL(CBTDevice& aDevice,MBTBondingObserver* aBondingObserver);
    IMPORT_C void CancelBonding(void);
  private: //MBTSDPSearchComplete
    void ReturnChannel(TUint aChannel,TInt aStatus);
    void SearchComplete(TInt aStatus);
  private: //MBTDeviceSearchObserver
    void DeviceSearchComplete(TInt aStatus);
  private:
    TSdpServRecordHandle RegisterSDPRecordDUNL(TUint8 aChannel);
    TSdpServRecordHandle RegisterSDPRecordFaxL(TUint8 aChannel);
    TSdpServRecordHandle RegisterSDPRecordObjectPushL(TUint8 aChannel);
    TSdpServRecordHandle RegisterSDPRecordFileTransferL(TUint8 aChannel);
  private:
    void ConstructL(void);
    CBTConnection();
  private:
    CBTBondingHandler* iBondingHandler;
    CBTSdpDbHandler* iBTSdpDbHandler;
    RBTMan iBTMan;
    RBTSecuritySettings iSecSet;
    CBTSdpResultReceiver* iSdpResultReceiver;
    CSdpAgent* iSdpAgent;
    CSdpSearchPattern* iSdpSearchPattern;
    TUint iChannel;
    TInt iStatus;
    TInt iDeviceSearchStatus;
};

class CBTRfs: public CBase
{
  public:
    IMPORT_C static CBTRfs* NewL(void);
    IMPORT_C static CBTRfs* NewLC(void);
    ~CBTRfs();
    IMPORT_C TInt RestoreFactorySettingsL(TBool aUser=ETrue);
  private:
    void ConstructL(void);
    CBTRfs();
};

#endif
