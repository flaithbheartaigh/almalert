/*
    phisdpattributeparser.hpp
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

#ifndef __PHISDPATTRIBUTEPARSER_HPP__
#define __PHISDPATTRIBUTEPARSER_HPP__

#include <btsdp.h>
#include "bt/staticarrayc.hpp"

class MPhiSdpAttributeNotifier;

class TPhiSdpAttributeParser: public MSdpAttributeValueVisitor
{
  public:
    enum TNodeCommand
    {
      ECheckType, //check the type of the value
      ECheckValue, //check the type and the value
      ECheckEnd, //check that a list ends at this point
      ESkip, //do not check this value - can not match a list end
      EReadValue, //pass the value onto the observer
      EFinished //marks the end of the node list
    };
    struct SSdpAttributeNode
    {
      TNodeCommand iCommand;
      TSdpElementType iType;
      TInt iValue;
    };
    typedef const TStaticArrayC<SSdpAttributeNode> TSdpAttributeList;
    TPhiSdpAttributeParser(TSdpAttributeList& aNodeList,MPhiSdpAttributeNotifier& aObserver);
    TBool HasFinished(void) const;
  public: //MSdpAttributeValueVisitor
    void VisitAttributeValueL(CSdpAttrValue& aValue,TSdpElementType aType);
    void StartListL(CSdpAttrValueList& aList);
    void EndListL(void);
  private:
    void CheckTypeL(TSdpElementType aElementType) const;
    void CheckValueL(CSdpAttrValue& aValue) const;
    void ReadValueL(CSdpAttrValue& aValue) const;
    const SSdpAttributeNode& CurrentNode() const;
    void AdvanceL(void);
  private:
    MPhiSdpAttributeNotifier& iObserver;
    TSdpAttributeList& iNodeList;
    TInt iCurrentNodeIndex;
};

#endif
