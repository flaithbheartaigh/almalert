/*
    phibtservicesearcher.hpp
    Copyright (C) 2007-2008 zg

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __PHIBTSERVICESEARCHER_HPP__
#define __PHIBTSERVICESEARCHER_HPP__

#include <e32base.h>
#include <bttypes.h>
#include <btextnotifiers.h>

#include "bt/phisdpattributeparser.hpp"
#include "bt/phisdpattributenotifier.hpp"

class CPhiBTServiceSearcher: public CBase,public MSdpAgentNotifier,public MPhiSdpAttributeNotifier
{
  public:
    ~CPhiBTServiceSearcher();
    void SelectDeviceByDiscoveryL(TRequestStatus& aObserverRequestStatus);
    void CancelSelectDeviceByDiscoveryL(void);
    void FindServiceL(TRequestStatus& iObserverRequestStatus);
    void CancelFindService(void);
    const TBTDevAddr& BTDevAddr(void);
    const TBTDeviceResponseParams& ResponseParams(void);
  protected:
    CPhiBTServiceSearcher();
    virtual void Finished(TInt aError=KErrNone);
    virtual TBool HasFinishedSearching(void) const;
    TBool HasFoundService(void) const;
  protected:
    virtual const TPhiSdpAttributeParser::TSdpAttributeList& ProtocolList(void) const=0;
    virtual const TUUID& ServiceClass() const=0;
    virtual void FoundElementL(TInt aKey, CSdpAttrValue& aValue)=0;
  public:
    void NextRecordRequestComplete(TInt aError,TSdpServRecordHandle aHandle,TInt aTotalRecordsCount);
    void AttributeRequestResult(TSdpServRecordHandle aHandle,TSdpAttributeID aAttrID,CSdpAttrValue* aAttrValue);
    void AttributeRequestComplete(TSdpServRecordHandle aHandle, TInt aError);
  private:
    void NextRecordRequestCompleteL(TInt aError,TSdpServRecordHandle aHandle,TInt aTotalRecordsCount);
    void AttributeRequestResultL(TSdpServRecordHandle aHandle,TSdpAttributeID aAttrID,CSdpAttrValue* aAttrValue);
    void AttributeRequestCompleteL(TSdpServRecordHandle,TInt aError);
  private:
    TRequestStatus* iStatusObserver;
    TBool iIsDeviceSelectorConnected;
    RNotifier iDeviceSelector;
    TBTDeviceResponseParamsPckg iResponse;
    CSdpAgent* iAgent;
    CSdpSearchPattern* iSdpSearchPattern;
    TBool iHasFoundService;
};

#endif
