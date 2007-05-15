/*
    etelcgsm.hpp
    Copyright (C) 2005-2006 zg

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

#ifndef __ETELCGSM_HPP__
#define __ETELCGSM_HPP__

#include <etelagsm.h>

class MEtelSsCustom
{
  public:
    struct TSsTypeAndMode
    {
      TInt iValue1;
      TInt iValue2;
    };
    struct TSsInfo
    {
      TInt iValue[42];
    };
    struct TSsAdditionalInfo
    {
      TInt iValue[131];
    };
    struct TSsCallForwardingNbrs
    {
      TInt iValue[702];
    };
    struct TVerifyBarringPassword
    {
      TInt iValue[51];
    };
    struct TCnapMode
    {
      TInt iValue;
    };
  public:
    virtual void SsNetworkNotification(TRequestStatus&,MEtelSsCustom::TSsTypeAndMode&,MEtelSsCustom::TSsInfo&)=0;
    virtual void SsNetworkNotificationCancel(void) const=0;
    virtual TInt GetBasicServiceCode(TDes8&) const=0;
    virtual void GetBasicServiceCodeCancel(void) const=0;
    virtual TInt SetBasicServiceCode(int) const=0;
    virtual void SetBasicServiceCodeCancel(void) const=0;
    virtual void SsAdditionalInfoNotification(TRequestStatus&,MEtelSsCustom::TSsAdditionalInfo&)=0;
    virtual void SsAdditionalInfoNotificationCancel(void) const=0;
    virtual TInt ChangeCommonPassword(MAdvGsmPhoneCallBarring::TChangeLockPassword) const=0;
    virtual TInt ChangeCommonPassword(TRequestStatus&,MAdvGsmPhoneCallBarring::TChangeLockPassword)=0;
    virtual void ChangeCommonPasswordCancel(void) const=0;
    virtual void SSRequestCompleteNotification(TRequestStatus&,int&)=0;
    virtual void SSRequestCompleteNotificationCancel(void) const=0;
    virtual TInt GetForwNumbers(MEtelSsCustom::TSsCallForwardingNbrs&) const=0;
    virtual void NotifySSActivationDataLost(TRequestStatus&)=0;
    virtual void NotifySSActivationDataLostCancel(void) const=0;
    virtual void VerifyBarringPassword(TRequestStatus&,MEtelSsCustom::TVerifyBarringPassword&,int&)=0;
    virtual void VerifyBarringPasswordCancel(void) const=0;
    virtual TInt GetCnapMode(MEtelSsCustom::TCnapMode&) const=0;
    virtual void GetCnapMode(TRequestStatus&, MEtelSsCustom::TCnapMode&)=0;
    virtual void GetCnapModeCancel(void) const=0;
    virtual TInt CancelUssdSession(void) const=0;
    virtual void CancelUssdSession(TRequestStatus&)=0;
    virtual void CancelUssdSessionCancel(void) const=0;
};

class MEtelCallCustom
{
  public:
    struct TCallEvent
    {
      TInt iValue;
    };
    struct TDtmfInfo
    {
      TInt iValue1;
      TInt iValue2;
      TInt iValue3;
    };
    struct TEmerNumberCheckMode
    {
      TInt iValue[53];
    };
    struct TRemoteAlertingToneStatus
    {
      TInt iValue;
    };
    struct TCallOrigin
    {
      TInt iValue;
    };
    struct TTwoDigitDialSupport
    {
      TInt iValue;
    };
  public:
    virtual void CallEventNotification(TRequestStatus&,MEtelCallCustom::TCallEvent&)=0;
    virtual void CallEventNotificationCancel(void) const=0;
    virtual TInt ClearCallBlackList(void) const=0;
    virtual void ClearCallBlackList(TRequestStatus&)=0;
    virtual void ClearCallBlackListCancel(void) const=0;
    virtual void NotifyDtmfEvent(TRequestStatus&, MEtelCallCustom::TDtmfInfo&)=0;
    virtual void NotifyDtmfEventCancel(void) const=0;
    virtual TInt CheckEmergencyNumber(MEtelCallCustom::TEmerNumberCheckMode&, int&) const=0;
    virtual void CheckEmergencyNumber(TRequestStatus&,MEtelCallCustom::TEmerNumberCheckMode&,int&)=0;
    virtual void CheckEmergencyNumberCancel(void) const=0;
    virtual TInt GetDiagnosticOctets(TBuf<128>&,TDes8&) const=0;
    virtual TInt GetInBandTones(TBuf<128>&,int&) const=0;
    virtual TInt TerminateCall(TBuf<128>&) const=0;
    virtual void TerminateCall(TRequestStatus&,TBuf<128>&)=0;
    virtual void TerminateCallCancel(void) const=0;
    virtual TInt GetRemoteAlertingToneStatus(MEtelCallCustom::TRemoteAlertingToneStatus&) const=0;
    virtual TInt CallOrigin(TBuf<128>&,MEtelCallCustom::TCallOrigin&) const=0;
    virtual TInt CheckTwoDigitDialSupport(MEtelCallCustom::TTwoDigitDialSupport&) const=0;
    virtual void ActivateCCBS(TRequestStatus&,int&,TBuf<128>&) const=0;
    virtual void ActivateCCBSCancel(void) const=0;
    virtual void NotifyCCBSStatusChange(TRequestStatus&,MAdvGsmPhoneCCBS::TCcbsStatus&) const=0;
    virtual void NotifyCCBSStatusChangeCancel(void) const=0;
    virtual void DeactivateCCBS(TRequestStatus&,int) const=0;
    virtual void DeactivateCCBSCancel(void) const=0;
    virtual void NotifyCCBSRecall(TRequestStatus&,MAdvGsmPhoneCCBS::TCcbsRequest&) const=0;
    virtual void NotifyCCBSRecallCancel(void) const=0;
    virtual void AcceptCCBSRecall(TRequestStatus&,int,TBuf<128>&) const=0;
    virtual void AcceptCCBSRecallCancel(void) const=0;
    virtual TInt RefuseCCBSRecall(int) const=0;
    virtual TInt RejectCCBS(void) const=0;
};

class MEtelAudCustom
{
  public:
    enum TVolMuteState
    {
      EVolMuteState0
    };
    enum THandsFreeMode
    {
      EHandsFreeMode0
    };
  public:
    virtual TInt GetOutputVolume(int&) const=0;
    virtual void GetOutputVolume(TRequestStatus &, int &)=0;
    virtual void GetOutputVolumeCancel(void) const=0;
    virtual TInt SetOutputVolume(int) const=0;
    virtual void SetOutputVolume(TRequestStatus&,int)=0;
    virtual void SetOutputVolumeCancel(void) const=0;
    virtual void NotifyOutputVolumeChange(TRequestStatus&,int&)=0;
    virtual void NotifyOutputVolumeChangeCancel(void) const=0;
    virtual TInt GetVolMuteState(MEtelAudCustom::TVolMuteState&) const=0;
    virtual void GetVolMuteState(TRequestStatus&,MEtelAudCustom::TVolMuteState&)=0;
    virtual void GetVolMuteStateCancel(void) const=0;
    virtual TInt SetVolMuteState(MEtelAudCustom::TVolMuteState) const=0;
    virtual void SetVolMuteState(TRequestStatus&,MEtelAudCustom::TVolMuteState)=0;
    virtual void SetVolMuteStateCancel(void) const=0;
    virtual TInt SetHandsfreeMode(MEtelAudCustom::THandsFreeMode) const=0;
    virtual void SetHandsfreeMode(TRequestStatus&,MEtelAudCustom::THandsFreeMode)=0;
    virtual void SetHandsfreeModeCancel(void) const=0;
    virtual TInt GetHandsfreeMode(MEtelAudCustom::THandsFreeMode&) const=0;
    virtual void GetHandsfreeMode(TRequestStatus&,MEtelAudCustom::THandsFreeMode&)=0;
    virtual void GetHandsfreeModeCancel(void) const=0;
    virtual TInt GetMicMuteState(int&) const=0;
    virtual void GetMicMuteState(TRequestStatus&,int&)=0;
    virtual void GetMicMuteStateCancel(void) const=0;
    virtual TInt SetMicMuteState(int) const=0;
    virtual void SetMicMuteState(TRequestStatus&,int)=0;
    virtual void SetMicMuteStateCancel(void) const=0;
    virtual void NotifyMicMuteStateChange(TRequestStatus&,int&)=0;
    virtual void NotifyMicMuteStateChangeCancel(void) const=0;
    virtual void NotifyHandsFreeModeChange(TRequestStatus&,MEtelAudCustom::THandsFreeMode&)=0;
    virtual void NotifyHandsFreeModeChangeCancel(void) const=0;
};

class MEtelAccCustom
{
  public:
    enum TAccBtMode
    {
      EAccBtMode0
    };
    struct TAccMode
    {
      TInt iValue;
    };
    enum TAccLpsMode
    {
      EAccLpsMode0
    };
    struct TButtonState
    {
      TInt iValue;
    };
    enum TVibraMode
    {
      EVibraOff,
      EVibraOn
    };
  public:
    virtual TInt SetAccessoryMode(MEtelAccCustom::TAccBtMode) const=0;
    virtual void SetAccessoryMode(TRequestStatus&,MEtelAccCustom::TAccBtMode)=0;
    virtual void SetAccessoryModeCancel(void) const=0;
    virtual TInt GetAccessoryMode(MEtelAccCustom::TAccMode&) const=0;
    virtual void GetAccessoryMode(TRequestStatus&,MEtelAccCustom::TAccMode&)=0;
    virtual void GetAccessoryModeCancel(void) const=0;
    virtual void NotifyAccessoryModeChange(TRequestStatus&,MEtelAccCustom::TAccMode&)=0;
    virtual void NotifyAccessoryModeChangeCancel(void) const=0;
    virtual TInt GetLoopSetModeState(MEtelAccCustom::TAccLpsMode&) const=0;
    virtual void GetLoopSetModeState(TRequestStatus&,MEtelAccCustom::TAccLpsMode&)=0;
    virtual void GetLoopSetModeStateCancel(void) const=0;
    virtual TInt SetLoopSetModeState(MEtelAccCustom::TAccLpsMode) const=0;
    virtual void SetLoopSetModeState(TRequestStatus&,MEtelAccCustom::TAccLpsMode)=0;
    virtual void SetLoopSetModeStateCancel(void) const=0;
    virtual void NotifyHeadsetButtonChange(TRequestStatus&,MEtelAccCustom::TButtonState&)=0;
    virtual void NotifyHeadsetButtonChangeCancel(void) const=0;
    virtual TInt SetVibraMode(const MEtelAccCustom::TVibraMode) const=0;
    virtual void SetVibraMode(TRequestStatus&,MEtelAccCustom::TVibraMode)=0;
    virtual void SetVibraModeCancel(void) const=0;
};

class MEtelSecCustom
{
  public:
    enum TOperation
    {
      EOperation0
    };
  public:
    virtual TInt ChangeSecCodeLock(MEtelSecCustom::TOperation) const=0;
    virtual void ChangeSecCodeLock(TRequestStatus&,MEtelSecCustom::TOperation)=0;
    virtual void ChangeSecCodeLockCancel(void) const=0;
    virtual TInt CheckSecurityCode(MAdvGsmPhoneSecurity::TSecurityCodeType)=0;
    virtual void CheckSecurityCode(TRequestStatus&,MAdvGsmPhoneSecurity::TSecurityCodeType)=0;
    virtual void CheckSecurityCodeCancel(void) const=0;
    virtual TInt CheckUnblockCode(MAdvGsmPhoneSecurity::TUnblockCodeType)=0;
    virtual void CheckUnblockCode(TRequestStatus&,MAdvGsmPhoneSecurity::TUnblockCodeType) const=0;
    virtual void CheckUnblockCodeCancel(void) const=0;
    virtual TInt IsBlocked(MAdvGsmPhoneSecurity::TSecurityCodeType&,int&) const=0;
};

class MEtelSimCustom
{
  public:
    struct TFlexiblePlmnNetworkInfo //FIXME: realy unknown size
    {
      TInt iValue;
    };
    struct TFlexiblePlmnNameInfo
    {
      TInt iValue[29];
    };
    struct TOperatorId
    {
      TInt iValue1;
      TInt iValue2;
    };
    struct TOperatorLogo
    {
      TInt iValue[5];
    };
    struct TOperatorNameInfo
    {
      TInt iValue[67];
    };
    struct TCipheringInfo
    {
      TInt iValue1;
      TInt iValue2;
    };
    enum TLockNumber
    {
      ELockNumber0
    };
    struct TViagParams
    {
      TInt iValue[21];
    };
    struct TViagElements
    {
      TInt iValue[56];
    };
    struct TViagCacheRecordId
    {
      TInt iValue1;
      TInt iValue2;
    };
    struct TViagCacheRecordContent
    {
      TInt iValue;
    };
    enum TPCmd //FIXME: realy unknown size
    {
      EPCmd0
    };
    enum TEventList //FIXME: realy unknown size
    {
      EEventList0
    };
  public:
    virtual TInt GetServiceProviderName(TDes16 &) const=0;
    virtual void GetServiceProviderName(TRequestStatus&,TDes16&)=0;
    virtual void GetServiceProviderNameCancel(void) const=0;
    virtual TInt GetNetworkProviderName(TDes16 &) const=0;
    virtual void GetNetworkProviderName(TRequestStatus&,TDes16&)=0;
    virtual void GetNetworkProviderNameCancel(void) const=0;
    virtual void GetFlexibleOperatorName(TRequestStatus&,MEtelSimCustom::TFlexiblePlmnNetworkInfo const&,MEtelSimCustom::TFlexiblePlmnNameInfo&)=0;
    virtual void GetFlexibleOperatorNameCancel(void) const=0;
    virtual TInt GetServiceProviderDisplayInfo(int&) const=0;
    virtual void GetServiceProviderDisplayInfo(TRequestStatus&,int&)=0;
    virtual void GetServiceProviderDisplayInfoCancel(void) const=0;
    virtual TInt GetProgrammableOperatorLogo(MEtelSimCustom::TOperatorId&,MEtelSimCustom::TOperatorLogo&) const=0;
    virtual void GetProgrammableOperatorLogo(TRequestStatus&,MEtelSimCustom::TOperatorId&,MEtelSimCustom::TOperatorLogo&)=0;
    virtual void GetProgrammableOperatorLogoCancel(void) const=0;
    virtual TInt GetProgrammableOperatorName(MEtelSimCustom::TOperatorId&,MEtelSimCustom::TOperatorNameInfo&) const=0;
    virtual void GetProgrammableOperatorName(TRequestStatus&,MEtelSimCustom::TOperatorId&,MEtelSimCustom::TOperatorNameInfo&)=0;
    virtual void GetProgrammableOperatorNameCancel(void) const=0;
    virtual void NotifyProgrammableOperatorLogoChange(TRequestStatus&,MEtelSimCustom::TOperatorId&)=0;
    virtual void NotifyProgrammableOperatorLogoChangeCancel(void) const=0;
    virtual void NotifyCellReselection(TRequestStatus&)=0;
    virtual void NotifyCellReselectionCancel(void) const=0;
    virtual TInt GetCipheringInfo(MEtelSimCustom::TCipheringInfo&) const=0;
    virtual void GetCipheringInfo(TRequestStatus&,MEtelSimCustom::TCipheringInfo&)=0;
    virtual void GetCipheringInfoCancel(void) const=0;
    virtual void NotifyCipheringChange(TRequestStatus&,MEtelSimCustom::TCipheringInfo&)=0;
    virtual void NotifyCipheringChangeCancel(void) const=0;
    virtual TInt ActivateSimLock(TBuf<30> const&,MEtelSimCustom::TLockNumber) const=0;
    virtual TInt DeActivateSimLock(TBuf<30> const&,MEtelSimCustom::TLockNumber) const=0;
    virtual TInt ReadViagHomeZoneParams(MEtelSimCustom::TViagParams&,MEtelSimCustom::TViagElements&) const=0;
    virtual void ReadViagHomeZoneParams(TRequestStatus&,MEtelSimCustom::TViagParams&,MEtelSimCustom::TViagElements&)=0;
    virtual void ReadViagHomeZoneParamsCancel(void) const=0;
    virtual TInt ReadViagHomeZoneCache(MEtelSimCustom::TViagCacheRecordId&,MEtelSimCustom::TViagCacheRecordContent&) const=0;
    virtual void ReadViagHomeZoneCache(TRequestStatus&,MEtelSimCustom::TViagCacheRecordId&,MEtelSimCustom::TViagCacheRecordContent&)=0;
    virtual void ReadViagHomeZoneCacheCancel(void) const=0;
    virtual TInt WriteViagHomeZoneCache(MEtelSimCustom::TViagCacheRecordId const&,MEtelSimCustom::TViagCacheRecordContent const&) const=0;
    virtual void WriteViagHomeZoneCache(TRequestStatus&,MEtelSimCustom::TViagCacheRecordId const&,MEtelSimCustom::TViagCacheRecordContent const&)=0;
    virtual void WriteViagHomeZoneCacheCancel(void) const=0;
    virtual void NotifyLaunchBrowserPCmd(TRequestStatus&,TDes8&)=0;
    virtual void NotifyLaunchBrowserPCmdCancel(void) const=0;
    virtual void TerminalRsp(TRequestStatus&,MEtelSimCustom::TPCmd,TDesC8 const&)=0;
    virtual void SatRefreshCompleteNotification(TRequestStatus&)=0;
    virtual void SatRefreshCompleteNotificationCancel(void) const=0;
    virtual void NotifySetUpEventListPCmd(TRequestStatus&,TDes8&)=0;
    virtual void NotifySetUpEventListPCmdCancel(void) const=0;
    virtual void EventDownload(TRequestStatus&,MEtelSimCustom::TEventList,TDesC8 const&)=0;
    virtual void EventDownloadCancel(void) const=0;
};

class MEtelSmsCustom
{
  public:
    struct TSimCbTobic
    {
      TInt iValue[13];
    };
    struct TStorageType
    {
      TInt iValue;
    };
    struct TStorageStatus
    {
      TInt iValue;
    };
  public:
    virtual TInt StartSimCbTobicBrowsing(void) const=0;
    virtual TInt GetNextSimCbTobic(MEtelSmsCustom::TSimCbTobic&) const=0;
    virtual void GetNextSimCbTobic(TRequestStatus&,MEtelSmsCustom::TSimCbTobic&)=0;
    virtual void GetNextSimCbTobicCancel(void) const=0;
    virtual TInt DeleteSimCbTobic(int) const=0;
    virtual void DeleteSimCbTobic(TRequestStatus&,int)=0;
    virtual void DeleteSimCbTobicCancel(void) const=0;
    virtual void SmsStorageNotification(TRequestStatus&,MEtelSmsCustom::TStorageType&,MEtelSmsCustom::TStorageStatus&)=0;
    virtual void SmsStorageNotificationCancel(void) const=0;
};

//Cai    - Charge Advice Information
//AcmMax - Accumulated Call Meter Maximum Value
//Puct   - Price per Unit and Currency Table
class MEtelAocCustom
{
  public:
    virtual TInt DisableAcmMax(void) const=0;
    virtual void DisableAcmMax(TRequestStatus&)=0;
    virtual void DisableAcmMaxCancel(void) const=0;
    virtual TInt DisablePuct(void) const=0;
    virtual void DisablePuct(TRequestStatus&)=0;
    virtual void DisablePuctCancel(void) const=0;
};

class MEtelGprsCustom
{
  public:
    virtual void NotifyTransferredGprsDataAmount(TRequestStatus&,unsigned long&,unsigned long&)=0;
    virtual void NotifyTransferredGprsDataAmountCancel(void) const=0;
    virtual void ReleaseFile(TRequestStatus&,TDesC16 const&)=0;
    virtual void ReleaseFileCancel(void) const=0;
    virtual void RestartFile(TRequestStatus&,TDesC16 const&)=0;
    virtual void RestartFileCancel(void) const=0;
};

class MEtelNetCustom
{
  public:
    struct TNspsStatus
    {
      TInt iValue;
    };
  public:
    virtual void NotifyNSPSStatus(TRequestStatus&,MEtelNetCustom::TNspsStatus&)=0;
    virtual void NotifyNSPSStatusCancel(void) const=0;
    virtual void NetWakeup(TRequestStatus&)=0;
    virtual void NetWakeupCancel(void) const=0;
    virtual void NotifyNetworkConnectionFailure(TRequestStatus&)=0;
    virtual void NotifyNetworkConnectionFailureCancel(void) const=0;
    virtual TInt ResetNetServer(void) const=0;
    virtual void ResetNetServer(TRequestStatus&)=0;
    virtual void ResetNetServerCancel(void) const=0;
};

class MEtelPndCustom
{
  public:
    virtual void NotifyPndCacheReady(TRequestStatus&,TDesC16 const&)=0;
    virtual void NotifyPndCacheReadyCancel(void) const=0;
};

class MEtelAlsCustom
{
  public:
    enum TSetAlsBlock //FIXME: realy unknown size
    {
      ESetAlsBlock0
    };
    struct TGetAlsBlockStatus
    {
      TInt iValue;
    };
    struct TAlsSupport
    {
      TInt iValue;
    };
    enum TActiveLine
    {
      EActiveLine0
    };
  public:
    virtual TInt SetAlsBlocked(MEtelAlsCustom::TSetAlsBlock) const=0;
    virtual void SetAlsBlocked(TRequestStatus &, MEtelAlsCustom::TSetAlsBlock)=0;
    virtual void SetAlsBlockedCancel(void) const=0;
    virtual TInt GetAlsBlocked(MEtelAlsCustom::TGetAlsBlockStatus&) const=0;
    virtual void GetAlsBlocked(TRequestStatus&,MEtelAlsCustom::TGetAlsBlockStatus&)=0;
    virtual void GetAlsBlockedCancel(void) const=0;
    virtual void NotifyAlsBlockedChanged(TRequestStatus&,MEtelAlsCustom::TGetAlsBlockStatus&)=0;
    virtual void NotifyAlsBlockedChangedCancel(void) const=0;
    virtual TInt CheckAlsPpSupport(MEtelAlsCustom::TAlsSupport&) const=0;
    virtual void CheckAlsPpSupport(TRequestStatus&,MEtelAlsCustom::TAlsSupport&)=0;
    virtual void CheckAlsPpSupportCancel(void) const=0;
    virtual TInt ReadSimAlsLine(MEtelAlsCustom::TActiveLine&) const=0;
    virtual void ReadSimAlsLine(TRequestStatus&,MEtelAlsCustom::TActiveLine&)=0;
    virtual void ReadSimAlsLineCancel(void) const=0;
    virtual TInt WriteSimAlsLine(MEtelAlsCustom::TActiveLine) const=0;
    virtual void WriteSimAlsLine(TRequestStatus &, MEtelAlsCustom::TActiveLine)=0;
    virtual void WriteSimAlsLineCancel(void) const=0;
    virtual void NotifySimAlsLineChange(TRequestStatus&,MEtelAlsCustom::TActiveLine&)=0;
    virtual void NotifySimAlsLineChangeCancel(void) const=0;
};

class CCtmPhonePtrHolder;
class RCtmGsmPhone: public MEtelAlsCustom,public RAdvGsmPhone,public MEtelCallCustom,public MEtelSsCustom,public MEtelAudCustom,public MEtelAccCustom,public MEtelSecCustom,public MEtelSimCustom,public MEtelSmsCustom,public MEtelAocCustom,public MEtelGprsCustom,public MEtelNetCustom,public MEtelPndCustom
{
  public:
    RCtmGsmPhone(void);
  public: //MEtelCallCustom
    IMPORT_C void CallEventNotification(TRequestStatus&,MEtelCallCustom::TCallEvent&);
    IMPORT_C void CallEventNotificationCancel(void) const;
    IMPORT_C TInt ClearCallBlackList(void) const;
    IMPORT_C void ClearCallBlackList(TRequestStatus&);
    IMPORT_C void ClearCallBlackListCancel(void) const;
    IMPORT_C void NotifyDtmfEvent(TRequestStatus&, MEtelCallCustom::TDtmfInfo&);
    IMPORT_C void NotifyDtmfEventCancel(void) const;
    IMPORT_C TInt CheckEmergencyNumber(MEtelCallCustom::TEmerNumberCheckMode&, int&) const;
    IMPORT_C void CheckEmergencyNumber(TRequestStatus&,MEtelCallCustom::TEmerNumberCheckMode&,int&);
    IMPORT_C void CheckEmergencyNumberCancel(void) const;
    IMPORT_C TInt GetDiagnosticOctets(TBuf<128>&,TDes8&) const;
    IMPORT_C TInt GetInBandTones(TBuf<128>&,int&) const;
    IMPORT_C TInt TerminateCall(TBuf<128>&) const;
    IMPORT_C void TerminateCall(TRequestStatus&,TBuf<128>&);
    IMPORT_C void TerminateCallCancel(void) const;
    IMPORT_C TInt GetRemoteAlertingToneStatus(MEtelCallCustom::TRemoteAlertingToneStatus&) const;
    IMPORT_C TInt CallOrigin(TBuf<128>&,MEtelCallCustom::TCallOrigin&) const;
    IMPORT_C TInt CheckTwoDigitDialSupport(MEtelCallCustom::TTwoDigitDialSupport&) const;
    IMPORT_C void ActivateCCBS(TRequestStatus&,int&,TBuf<128>&) const;
    IMPORT_C void ActivateCCBSCancel(void) const;
    IMPORT_C void NotifyCCBSStatusChange(TRequestStatus&,MAdvGsmPhoneCCBS::TCcbsStatus&) const;
    IMPORT_C void NotifyCCBSStatusChangeCancel(void) const;
    IMPORT_C void DeactivateCCBS(TRequestStatus&,int) const;
    IMPORT_C void DeactivateCCBSCancel(void) const;
    IMPORT_C void NotifyCCBSRecall(TRequestStatus&,MAdvGsmPhoneCCBS::TCcbsRequest&) const;
    IMPORT_C void NotifyCCBSRecallCancel(void) const;
    IMPORT_C void AcceptCCBSRecall(TRequestStatus&,int,TBuf<128>&) const;
    IMPORT_C void AcceptCCBSRecallCancel(void) const;
    IMPORT_C TInt RefuseCCBSRecall(int) const;
    IMPORT_C TInt RejectCCBS(void) const;
  public: //MEtelSsCustom
    IMPORT_C void SsNetworkNotification(TRequestStatus&,MEtelSsCustom::TSsTypeAndMode&,MEtelSsCustom::TSsInfo&);
    IMPORT_C void SsNetworkNotificationCancel(void) const;
    IMPORT_C TInt GetBasicServiceCode(TDes8&) const;
    IMPORT_C void GetBasicServiceCodeCancel(void) const;
    IMPORT_C TInt SetBasicServiceCode(int) const;
    IMPORT_C void SetBasicServiceCodeCancel(void) const;
    IMPORT_C void SsAdditionalInfoNotification(TRequestStatus&,MEtelSsCustom::TSsAdditionalInfo&);
    IMPORT_C void SsAdditionalInfoNotificationCancel(void) const;
    IMPORT_C TInt ChangeCommonPassword(MAdvGsmPhoneCallBarring::TChangeLockPassword) const;
    IMPORT_C TInt ChangeCommonPassword(TRequestStatus&,MAdvGsmPhoneCallBarring::TChangeLockPassword);
    IMPORT_C void ChangeCommonPasswordCancel(void) const;
    IMPORT_C void SSRequestCompleteNotification(TRequestStatus&,int&);
    IMPORT_C void SSRequestCompleteNotificationCancel(void) const;
    IMPORT_C TInt GetForwNumbers(MEtelSsCustom::TSsCallForwardingNbrs&) const;
    IMPORT_C void NotifySSActivationDataLost(TRequestStatus&);
    IMPORT_C void NotifySSActivationDataLostCancel(void) const;
    IMPORT_C void VerifyBarringPassword(TRequestStatus&,MEtelSsCustom::TVerifyBarringPassword&,int&);
    IMPORT_C void VerifyBarringPasswordCancel(void) const;
    IMPORT_C TInt GetCnapMode(MEtelSsCustom::TCnapMode&) const;
    IMPORT_C void GetCnapMode(TRequestStatus&, MEtelSsCustom::TCnapMode&);
    IMPORT_C void GetCnapModeCancel(void) const;
    IMPORT_C TInt CancelUssdSession(void) const;
    IMPORT_C void CancelUssdSession(TRequestStatus&);
    IMPORT_C void CancelUssdSessionCancel(void) const;
  public: //MEtelAudCustom
    IMPORT_C TInt GetOutputVolume(int&) const;
    IMPORT_C void GetOutputVolume(TRequestStatus &, int &);
    IMPORT_C void GetOutputVolumeCancel(void) const;
    IMPORT_C TInt SetOutputVolume(int) const;
    IMPORT_C void SetOutputVolume(TRequestStatus&,int);
    IMPORT_C void SetOutputVolumeCancel(void) const;
    IMPORT_C void NotifyOutputVolumeChange(TRequestStatus&,int&);
    IMPORT_C void NotifyOutputVolumeChangeCancel(void) const;
    IMPORT_C TInt GetVolMuteState(MEtelAudCustom::TVolMuteState&) const;
    IMPORT_C void GetVolMuteState(TRequestStatus&,MEtelAudCustom::TVolMuteState&);
    IMPORT_C void GetVolMuteStateCancel(void) const;
    IMPORT_C TInt SetVolMuteState(MEtelAudCustom::TVolMuteState) const;
    IMPORT_C void SetVolMuteState(TRequestStatus&,MEtelAudCustom::TVolMuteState);
    IMPORT_C void SetVolMuteStateCancel(void) const;
    IMPORT_C TInt SetHandsfreeMode(MEtelAudCustom::THandsFreeMode) const;
    IMPORT_C void SetHandsfreeMode(TRequestStatus&,MEtelAudCustom::THandsFreeMode);
    IMPORT_C void SetHandsfreeModeCancel(void) const;
    IMPORT_C TInt GetHandsfreeMode(MEtelAudCustom::THandsFreeMode&) const;
    IMPORT_C void GetHandsfreeMode(TRequestStatus&,MEtelAudCustom::THandsFreeMode&);
    IMPORT_C void GetHandsfreeModeCancel(void) const;
    IMPORT_C TInt GetMicMuteState(int&) const;
    IMPORT_C void GetMicMuteState(TRequestStatus&,int&);
    IMPORT_C void GetMicMuteStateCancel(void) const;
    IMPORT_C TInt SetMicMuteState(int) const;
    IMPORT_C void SetMicMuteState(TRequestStatus&,int);
    IMPORT_C void SetMicMuteStateCancel(void) const;
    IMPORT_C void NotifyMicMuteStateChange(TRequestStatus&,int&);
    IMPORT_C void NotifyMicMuteStateChangeCancel(void) const;
    IMPORT_C void NotifyHandsFreeModeChange(TRequestStatus&,MEtelAudCustom::THandsFreeMode&);
    IMPORT_C void NotifyHandsFreeModeChangeCancel(void) const;
  public: //MEtelAccCustom
    IMPORT_C TInt SetAccessoryMode(MEtelAccCustom::TAccBtMode) const;
    IMPORT_C void SetAccessoryMode(TRequestStatus&,MEtelAccCustom::TAccBtMode);
    IMPORT_C void SetAccessoryModeCancel(void) const;
    IMPORT_C TInt GetAccessoryMode(MEtelAccCustom::TAccMode&) const;
    IMPORT_C void GetAccessoryMode(TRequestStatus&,MEtelAccCustom::TAccMode&);
    IMPORT_C void GetAccessoryModeCancel(void) const;
    IMPORT_C void NotifyAccessoryModeChange(TRequestStatus&,MEtelAccCustom::TAccMode&);
    IMPORT_C void NotifyAccessoryModeChangeCancel(void) const;
    IMPORT_C TInt GetLoopSetModeState(MEtelAccCustom::TAccLpsMode&) const;
    IMPORT_C void GetLoopSetModeState(TRequestStatus&,MEtelAccCustom::TAccLpsMode&);
    IMPORT_C void GetLoopSetModeStateCancel(void) const;
    IMPORT_C TInt SetLoopSetModeState(MEtelAccCustom::TAccLpsMode) const;
    IMPORT_C void SetLoopSetModeState(TRequestStatus&,MEtelAccCustom::TAccLpsMode);
    IMPORT_C void SetLoopSetModeStateCancel(void) const;
    IMPORT_C void NotifyHeadsetButtonChange(TRequestStatus&,MEtelAccCustom::TButtonState&);
    IMPORT_C void NotifyHeadsetButtonChangeCancel(void) const;
    IMPORT_C TInt SetVibraMode(const MEtelAccCustom::TVibraMode) const;
    IMPORT_C void SetVibraMode(TRequestStatus&,MEtelAccCustom::TVibraMode);
    IMPORT_C void SetVibraModeCancel(void) const;
  public: //MEtelSecCustom
    IMPORT_C TInt ChangeSecCodeLock(MEtelSecCustom::TOperation) const;
    IMPORT_C void ChangeSecCodeLock(TRequestStatus&,MEtelSecCustom::TOperation);
    IMPORT_C void ChangeSecCodeLockCancel(void) const;
    IMPORT_C TInt CheckSecurityCode(MAdvGsmPhoneSecurity::TSecurityCodeType);
    IMPORT_C void CheckSecurityCode(TRequestStatus&,MAdvGsmPhoneSecurity::TSecurityCodeType);
    IMPORT_C void CheckSecurityCodeCancel(void) const;
    IMPORT_C TInt CheckUnblockCode(MAdvGsmPhoneSecurity::TUnblockCodeType);
    IMPORT_C void CheckUnblockCode(TRequestStatus&,MAdvGsmPhoneSecurity::TUnblockCodeType) const;
    IMPORT_C void CheckUnblockCodeCancel(void) const;
    IMPORT_C TInt IsBlocked(MAdvGsmPhoneSecurity::TSecurityCodeType&,int&) const;
  public: //MEtelSimCustom
    IMPORT_C TInt GetServiceProviderName(TDes16 &) const;
    IMPORT_C void GetServiceProviderName(TRequestStatus&,TDes16&);
    IMPORT_C void GetServiceProviderNameCancel(void) const;
    IMPORT_C TInt GetNetworkProviderName(TDes16 &) const;
    IMPORT_C void GetNetworkProviderName(TRequestStatus&,TDes16&);
    IMPORT_C void GetNetworkProviderNameCancel(void) const;
    IMPORT_C void GetFlexibleOperatorName(TRequestStatus&,MEtelSimCustom::TFlexiblePlmnNetworkInfo const&,MEtelSimCustom::TFlexiblePlmnNameInfo&);
    IMPORT_C void GetFlexibleOperatorNameCancel(void) const;
    IMPORT_C TInt GetServiceProviderDisplayInfo(int&) const;
    IMPORT_C void GetServiceProviderDisplayInfo(TRequestStatus&,int&);
    IMPORT_C void GetServiceProviderDisplayInfoCancel(void) const;
    IMPORT_C TInt GetProgrammableOperatorLogo(MEtelSimCustom::TOperatorId&,MEtelSimCustom::TOperatorLogo&) const;
    IMPORT_C void GetProgrammableOperatorLogo(TRequestStatus&,MEtelSimCustom::TOperatorId&,MEtelSimCustom::TOperatorLogo&);
    IMPORT_C void GetProgrammableOperatorLogoCancel(void) const;
    IMPORT_C TInt GetProgrammableOperatorName(MEtelSimCustom::TOperatorId&,MEtelSimCustom::TOperatorNameInfo&) const;
    IMPORT_C void GetProgrammableOperatorName(TRequestStatus&,MEtelSimCustom::TOperatorId&,MEtelSimCustom::TOperatorNameInfo&);
    IMPORT_C void GetProgrammableOperatorNameCancel(void) const;
    IMPORT_C void NotifyProgrammableOperatorLogoChange(TRequestStatus&,MEtelSimCustom::TOperatorId&);
    IMPORT_C void NotifyProgrammableOperatorLogoChangeCancel(void) const;
    IMPORT_C void NotifyCellReselection(TRequestStatus&);
    IMPORT_C void NotifyCellReselectionCancel(void) const;
    IMPORT_C TInt GetCipheringInfo(MEtelSimCustom::TCipheringInfo&) const;
    IMPORT_C void GetCipheringInfo(TRequestStatus&,MEtelSimCustom::TCipheringInfo&);
    IMPORT_C void GetCipheringInfoCancel(void) const;
    IMPORT_C void NotifyCipheringChange(TRequestStatus&,MEtelSimCustom::TCipheringInfo&);
    IMPORT_C void NotifyCipheringChangeCancel(void) const;
    IMPORT_C TInt ActivateSimLock(TBuf<30> const&,MEtelSimCustom::TLockNumber) const;
    IMPORT_C TInt DeActivateSimLock(TBuf<30> const&,MEtelSimCustom::TLockNumber) const;
    IMPORT_C TInt ReadViagHomeZoneParams(MEtelSimCustom::TViagParams&,MEtelSimCustom::TViagElements&) const;
    IMPORT_C void ReadViagHomeZoneParams(TRequestStatus&,MEtelSimCustom::TViagParams&,MEtelSimCustom::TViagElements&);
    IMPORT_C void ReadViagHomeZoneParamsCancel(void) const;
    IMPORT_C TInt ReadViagHomeZoneCache(MEtelSimCustom::TViagCacheRecordId&,MEtelSimCustom::TViagCacheRecordContent&) const;
    IMPORT_C void ReadViagHomeZoneCache(TRequestStatus&,MEtelSimCustom::TViagCacheRecordId&,MEtelSimCustom::TViagCacheRecordContent&);
    IMPORT_C void ReadViagHomeZoneCacheCancel(void) const;
    IMPORT_C TInt WriteViagHomeZoneCache(MEtelSimCustom::TViagCacheRecordId const&,MEtelSimCustom::TViagCacheRecordContent const&) const;
    IMPORT_C void WriteViagHomeZoneCache(TRequestStatus&,MEtelSimCustom::TViagCacheRecordId const&,MEtelSimCustom::TViagCacheRecordContent const&);
    IMPORT_C void WriteViagHomeZoneCacheCancel(void) const;
    IMPORT_C void NotifyLaunchBrowserPCmd(TRequestStatus&,TDes8&);
    IMPORT_C void NotifyLaunchBrowserPCmdCancel(void) const;
    IMPORT_C void TerminalRsp(TRequestStatus&,MEtelSimCustom::TPCmd,TDesC8 const&);
    IMPORT_C void SatRefreshCompleteNotification(TRequestStatus&);
    IMPORT_C void SatRefreshCompleteNotificationCancel(void) const;
    IMPORT_C void NotifySetUpEventListPCmd(TRequestStatus&,TDes8&);
    IMPORT_C void NotifySetUpEventListPCmdCancel(void) const;
    IMPORT_C void EventDownload(TRequestStatus&,MEtelSimCustom::TEventList,TDesC8 const&);
    IMPORT_C void EventDownloadCancel(void) const;
  public: //MEtelSmsCustom
    IMPORT_C TInt StartSimCbTobicBrowsing(void) const;
    IMPORT_C TInt GetNextSimCbTobic(MEtelSmsCustom::TSimCbTobic&) const;
    IMPORT_C void GetNextSimCbTobic(TRequestStatus&,MEtelSmsCustom::TSimCbTobic&);
    IMPORT_C void GetNextSimCbTobicCancel(void) const;
    IMPORT_C TInt DeleteSimCbTobic(int) const;
    IMPORT_C void DeleteSimCbTobic(TRequestStatus&,int);
    IMPORT_C void DeleteSimCbTobicCancel(void) const;
    IMPORT_C void SmsStorageNotification(TRequestStatus&,MEtelSmsCustom::TStorageType&,MEtelSmsCustom::TStorageStatus&);
    IMPORT_C void SmsStorageNotificationCancel(void) const;
  public: //MEtelAocCustom
    IMPORT_C TInt DisableAcmMax(void) const;
    IMPORT_C void DisableAcmMax(TRequestStatus&);
    IMPORT_C void DisableAcmMaxCancel(void) const;
    IMPORT_C TInt DisablePuct(void) const;
    IMPORT_C void DisablePuct(TRequestStatus&);
    IMPORT_C void DisablePuctCancel(void) const;
  public: //MEtelGprsCustom
    IMPORT_C void NotifyTransferredGprsDataAmount(TRequestStatus&,unsigned long&,unsigned long&);
    IMPORT_C void NotifyTransferredGprsDataAmountCancel(void) const;
    IMPORT_C void ReleaseFile(TRequestStatus&,TDesC16 const&);
    IMPORT_C void ReleaseFileCancel(void) const;
    IMPORT_C void RestartFile(TRequestStatus&,TDesC16 const&);
    IMPORT_C void RestartFileCancel(void) const;
  public: //MEtelNetCustom
    IMPORT_C void NotifyNSPSStatus(TRequestStatus&,MEtelNetCustom::TNspsStatus&);
    IMPORT_C void NotifyNSPSStatusCancel(void) const;
    IMPORT_C void NetWakeup(TRequestStatus&);
    IMPORT_C void NetWakeupCancel(void) const;
    IMPORT_C void NotifyNetworkConnectionFailure(TRequestStatus&);
    IMPORT_C void NotifyNetworkConnectionFailureCancel(void) const;
    IMPORT_C TInt ResetNetServer(void) const;
    IMPORT_C void ResetNetServer(TRequestStatus&);
    IMPORT_C void ResetNetServerCancel(void) const;
  public: //MEtelPndCustom
    IMPORT_C void NotifyPndCacheReady(TRequestStatus&,TDesC16 const&);
    IMPORT_C void NotifyPndCacheReadyCancel(void) const;
  public: //MEtelAlsCustom
    IMPORT_C TInt SetAlsBlocked(MEtelAlsCustom::TSetAlsBlock) const;
    IMPORT_C void SetAlsBlocked(TRequestStatus &, MEtelAlsCustom::TSetAlsBlock);
    IMPORT_C void SetAlsBlockedCancel(void) const;
    IMPORT_C TInt GetAlsBlocked(MEtelAlsCustom::TGetAlsBlockStatus&) const;
    IMPORT_C void GetAlsBlocked(TRequestStatus&,MEtelAlsCustom::TGetAlsBlockStatus&);
    IMPORT_C void GetAlsBlockedCancel(void) const;
    IMPORT_C void NotifyAlsBlockedChanged(TRequestStatus&,MEtelAlsCustom::TGetAlsBlockStatus&);
    IMPORT_C void NotifyAlsBlockedChangedCancel(void) const;
    IMPORT_C TInt CheckAlsPpSupport(MEtelAlsCustom::TAlsSupport&) const;
    IMPORT_C void CheckAlsPpSupport(TRequestStatus&,MEtelAlsCustom::TAlsSupport&);
    IMPORT_C void CheckAlsPpSupportCancel(void) const;
    IMPORT_C TInt ReadSimAlsLine(MEtelAlsCustom::TActiveLine&) const;
    IMPORT_C void ReadSimAlsLine(TRequestStatus&,MEtelAlsCustom::TActiveLine&);
    IMPORT_C void ReadSimAlsLineCancel(void) const;
    IMPORT_C TInt WriteSimAlsLine(MEtelAlsCustom::TActiveLine) const;
    IMPORT_C void WriteSimAlsLine(TRequestStatus &, MEtelAlsCustom::TActiveLine);
    IMPORT_C void WriteSimAlsLineCancel(void) const;
    IMPORT_C void NotifySimAlsLineChange(TRequestStatus&,MEtelAlsCustom::TActiveLine&);
    IMPORT_C void NotifySimAlsLineChangeCancel(void) const;
  protected: //RPhone
    IMPORT_C void ConstructL(void);
    IMPORT_C void Destruct(void);
  private:
    CCtmPhonePtrHolder* iCtmPtrHolder;
};

#endif
