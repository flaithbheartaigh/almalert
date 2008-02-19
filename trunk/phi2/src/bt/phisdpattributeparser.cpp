/*
    phisdpattributeparser.cpp
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
#include "bt/phisdpattributeparser.hpp"
#include "bt/phisdpattributenotifier.hpp"

TPhiSdpAttributeParser::TPhiSdpAttributeParser(TSdpAttributeList& aNodeList,MPhiSdpAttributeNotifier& aObserver): iObserver(aObserver),iNodeList(aNodeList),iCurrentNodeIndex(0)
{
}

TBool TPhiSdpAttributeParser::HasFinished(void) const
{
  return CurrentNode().iCommand==EFinished;
}

void TPhiSdpAttributeParser::VisitAttributeValueL(CSdpAttrValue& aValue,TSdpElementType aType)
{
  switch(CurrentNode().iCommand)
  {
    case ECheckType:
      CheckTypeL(aType);
      break;
    case ECheckValue:
      CheckTypeL(aType);
      CheckValueL(aValue);
      break;
    case ECheckEnd:
      User::Leave(KErrGeneral);
      break;
    case ESkip:
      break;
    case EReadValue:
      CheckTypeL(aType);
      ReadValueL(aValue);
      break;
    case EFinished:
      User::Leave(KErrGeneral);
      return;
    default:
      User::Invariant();
  }
  AdvanceL();
}

void TPhiSdpAttributeParser::StartListL(CSdpAttrValueList& /*aList*/)
{
}

void TPhiSdpAttributeParser::EndListL()
{
  if(CurrentNode().iCommand!=ECheckEnd)
  {
    User::Leave(KErrGeneral);
  }
  AdvanceL();
}

void TPhiSdpAttributeParser::CheckTypeL(TSdpElementType aElementType) const
{
  if(CurrentNode().iType!=aElementType) User::Leave(KErrGeneral);
}

void TPhiSdpAttributeParser::CheckValueL(CSdpAttrValue& aValue) const
{
  switch(aValue.Type())
  {
    case ETypeNil:
      User::Invariant();
      break;
    case ETypeUint:
      if(aValue.Uint()!=(TUint)CurrentNode().iValue) User::Leave(KErrArgument);
      break;
    case ETypeInt:
      if(aValue.Int()!=CurrentNode().iValue) User::Leave(KErrArgument);
      break;
    case ETypeBoolean:
      if(aValue.Bool()!=CurrentNode().iValue) User::Leave(KErrArgument);
      break;
    case ETypeUUID:
      if(aValue.UUID()!=TUUID(CurrentNode().iValue)) User::Leave(KErrArgument);
      break;
    case ETypeDES:
    case ETypeDEA:
      User::Invariant();
      break;
    default:
      User::Invariant();
      break;
  }
}

void TPhiSdpAttributeParser::ReadValueL(CSdpAttrValue& aValue) const
{
  iObserver.FoundElementL(CurrentNode().iValue,aValue);
}

const TPhiSdpAttributeParser::SSdpAttributeNode& TPhiSdpAttributeParser::CurrentNode() const
{
  return iNodeList[iCurrentNodeIndex];
}

void TPhiSdpAttributeParser::AdvanceL()
{
  if(CurrentNode().iCommand==EFinished)
  {
    User::Leave(KErrEof);
  }
  ++iCurrentNodeIndex;
}
