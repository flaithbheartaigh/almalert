/*
    phiobexservicesearcher.cpp
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

#include <bt_sock.h>
#include "bt/phiobexservicesearcher.hpp"

const TUint KBtProtocolIdOBEX=0x0008;
const TUint KRfcommChannel=1;
const TUint KServiceClass=0x1105;

const TPhiSdpAttributeParser::SSdpAttributeNode gObexProtocolListData[]=
{
  {TPhiSdpAttributeParser::ECheckType,ETypeDES},
  {TPhiSdpAttributeParser::ECheckType,ETypeDES},
  {TPhiSdpAttributeParser::ECheckValue,ETypeUUID,KL2CAP},
  {TPhiSdpAttributeParser::ECheckEnd},
  {TPhiSdpAttributeParser::ECheckType,ETypeDES},
  {TPhiSdpAttributeParser::ECheckValue,ETypeUUID,KRFCOMM},
  {TPhiSdpAttributeParser::EReadValue,ETypeUint,KRfcommChannel},
  {TPhiSdpAttributeParser::ECheckEnd},
  {TPhiSdpAttributeParser::ECheckType,ETypeDES},
  {TPhiSdpAttributeParser::ECheckValue,ETypeUUID,KBtProtocolIdOBEX},
  {TPhiSdpAttributeParser::ECheckEnd},
  {TPhiSdpAttributeParser::ECheckEnd},
  {TPhiSdpAttributeParser::EFinished}
};

const TStaticArrayC<TPhiSdpAttributeParser::SSdpAttributeNode> gObexProtocolList=CONSTRUCT_STATIC_ARRAY_C(gObexProtocolListData);

CPhiObexServiceSearcher* CPhiObexServiceSearcher::NewL(void)
{
  CPhiObexServiceSearcher* self=new(ELeave)CPhiObexServiceSearcher;
  CleanupStack::PushL(self);
  self->ConstructL();
  CleanupStack::Pop(); //self
  return self;
}

CPhiObexServiceSearcher::CPhiObexServiceSearcher(): CPhiBTServiceSearcher(),iServiceClass(KServiceClass),iPort(-1)
{
}

CPhiObexServiceSearcher::~CPhiObexServiceSearcher()
{
}

void CPhiObexServiceSearcher::ConstructL()
{
}

const TUUID& CPhiObexServiceSearcher::ServiceClass() const
{
  return iServiceClass;
}

const TPhiSdpAttributeParser::TSdpAttributeList& CPhiObexServiceSearcher::ProtocolList() const
{
  return gObexProtocolList;
}

void CPhiObexServiceSearcher::FoundElementL(TInt aKey,CSdpAttrValue& aValue)
{
  if(aKey!=static_cast<TInt>(KRfcommChannel)) User::Invariant();
  iPort=aValue.Uint();
}

TInt CPhiObexServiceSearcher::Port()
{
  return iPort;
}
