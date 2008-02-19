/*
    phiobexservicesearcher.hpp
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

#ifndef __PHIOBEXSERVICESEARCHER_HPP__
#define __PHIOBEXSERVICESEARCHER_HPP__

#include <e32base.h>
#include "bt/phibtservicesearcher.hpp"

class CPhiObexServiceSearcher: public CPhiBTServiceSearcher
{
  public:
    static CPhiObexServiceSearcher* NewL(void);
    ~CPhiObexServiceSearcher();
    TInt Port();
  protected:
    const TUUID& ServiceClass(void) const;
    const TPhiSdpAttributeParser::TSdpAttributeList& ProtocolList(void) const;
    void FoundElementL(TInt aKey,CSdpAttrValue& aValue);
  private:
    CPhiObexServiceSearcher();
    void ConstructL(void);
  private:
    TUUID iServiceClass;
    TInt iPort;
};

#endif
