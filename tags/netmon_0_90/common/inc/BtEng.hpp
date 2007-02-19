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
    IMPORT_C TInt DeleteDevicesWithoutLinkKeyL(void);
};

#endif
