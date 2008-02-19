/*
    phibtservicesearcher.cpp
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

#include "bt/phibtservicesearcher.hpp"

CPhiBTServiceSearcher::CPhiBTServiceSearcher(): iIsDeviceSelectorConnected(EFalse)
{
}

CPhiBTServiceSearcher::~CPhiBTServiceSearcher()
{
  if(iIsDeviceSelectorConnected)
  {
    iDeviceSelector.CancelNotifier(KDeviceSelectionNotifierUid);
    iDeviceSelector.Close();
  }
  delete iSdpSearchPattern;
  iSdpSearchPattern=NULL;
  delete iAgent;
  iAgent=NULL;
}

void CPhiBTServiceSearcher::SelectDeviceByDiscoveryL(TRequestStatus& aObserverRequestStatus)
{
  if(!iIsDeviceSelectorConnected)
  {
    User::LeaveIfError(iDeviceSelector.Connect());
    iIsDeviceSelectorConnected=ETrue;
  }
  TBTDeviceSelectionParamsPckg selectionFilter;
  selectionFilter().SetUUID(ServiceClass());
  iDeviceSelector.StartNotifierAndGetResponse(aObserverRequestStatus,KDeviceSelectionNotifierUid,selectionFilter,iResponse);
}

void CPhiBTServiceSearcher::CancelSelectDeviceByDiscoveryL(void)
{
  if(iIsDeviceSelectorConnected)
  {
    User::LeaveIfError(iDeviceSelector.CancelNotifier(KDeviceSelectionNotifierUid));
  }
}

void CPhiBTServiceSearcher::FindServiceL(TRequestStatus& aObserverRequestStatus)
{
  if(!iResponse().IsValidBDAddr()) User::Leave(KErrNotFound);
  iHasFoundService=EFalse;
  delete iSdpSearchPattern;
  iSdpSearchPattern=NULL;
  delete iAgent;
  iAgent=NULL;
  iAgent=CSdpAgent::NewL(*this,BTDevAddr());
  iSdpSearchPattern=CSdpSearchPattern::NewL();
  iSdpSearchPattern->AddL(ServiceClass());
  iAgent->SetRecordFilterL(*iSdpSearchPattern);
  iStatusObserver=&aObserverRequestStatus;
  iAgent->NextRecordRequestL();
}

void CPhiBTServiceSearcher::CancelFindService(void)
{
  if(iAgent)
  {
    iAgent->Cancel();
    if(iStatusObserver) User::RequestComplete(iStatusObserver,KErrCancel);
  }
}

void CPhiBTServiceSearcher::NextRecordRequestComplete(TInt aError,TSdpServRecordHandle aHandle,TInt aTotalRecordsCount)
{
  TRAPD(err,NextRecordRequestCompleteL(aError,aHandle,aTotalRecordsCount));
  if(err!=KErrNone) User::Invariant();
}

void CPhiBTServiceSearcher::NextRecordRequestCompleteL(TInt aError,TSdpServRecordHandle aHandle,TInt aTotalRecordsCount)
{
  if(aError==KErrEof)
  {
    Finished();
    return;
  }
  if(aError!=KErrNone)
  {
    Finished(aError);
    return;
  }
  if(aTotalRecordsCount==0)
  {
    Finished(KErrNotFound);
    return;
  }
  iAgent->AttributeRequestL(aHandle,KSdpAttrIdProtocolDescriptorList);
}

void CPhiBTServiceSearcher::AttributeRequestResult(TSdpServRecordHandle aHandle,TSdpAttributeID aAttrID,CSdpAttrValue* aAttrValue)
{
  TRAPD(err,AttributeRequestResultL(aHandle,aAttrID,aAttrValue));
  if(err!=KErrNone) User::Invariant();
  delete aAttrValue;
}

void CPhiBTServiceSearcher::AttributeRequestResultL(TSdpServRecordHandle /*aHandle*/,TSdpAttributeID aAttrID,CSdpAttrValue* aAttrValue)
{
  if(aAttrID!=KSdpAttrIdProtocolDescriptorList) User::Invariant();
  TPhiSdpAttributeParser parser(ProtocolList(),*this);
  aAttrValue->AcceptVisitorL(parser);
  if(parser.HasFinished())
  {
    iHasFoundService=ETrue;
  }
}

void CPhiBTServiceSearcher::AttributeRequestComplete(TSdpServRecordHandle aHandle,TInt aError)
{
  TRAPD(err,AttributeRequestCompleteL(aHandle,aError));
  if(err!=KErrNone) User::Invariant();
}

void CPhiBTServiceSearcher::AttributeRequestCompleteL(TSdpServRecordHandle /*aHandle*/,TInt aError)
{
  if(aError!=KErrNone)
  {
  }
  else if(!HasFinishedSearching())
  {
    iAgent->NextRecordRequestL();
  }
  else
  {
    Finished();
  }
}

void CPhiBTServiceSearcher::Finished(TInt aError)
{
  if(aError==KErrNone&&!HasFoundService())
  {
    aError=KErrNotFound;
  }
  User::RequestComplete(iStatusObserver,aError);
}

TBool CPhiBTServiceSearcher::HasFinishedSearching(void) const
{
  return EFalse;
}

const TBTDevAddr& CPhiBTServiceSearcher::BTDevAddr(void)
{
  return iResponse().BDAddr();
}

const TBTDeviceResponseParams& CPhiBTServiceSearcher::ResponseParams(void)
{
  return iResponse();
}

TBool CPhiBTServiceSearcher::HasFoundService(void) const
{
  return iHasFoundService;
}
