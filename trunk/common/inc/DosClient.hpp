/*
    DosClient.hpp
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

#ifndef __DOSCLIENT_HPP__
#define __DOSCLIENT_HPP__

#include <e32std.h>

class RDosServer: public RSessionBase
{
  public:
    IMPORT_C void Close(void);
    IMPORT_C TInt Connect(void);
    IMPORT_C void ServerShutdown(void) const;
    IMPORT_C TInt StartServer(void) const;
};

class RDosSubSession: public RSubSessionBase
{
  public:
    IMPORT_C void Close(void);
};

class RDosLights: public RDosSubSession
{
  public:
    enum TType
    {
      ESwitchScreen=0,
      ESwitchBoth,
      ESwitchKeys //only on qd
    };
    enum TState
    {
      ESlowOn=0,
      ESlowOff,
      EOn,
      EBlink
    };
  public:
    IMPORT_C TInt Open(RDosServer& aSession);
    IMPORT_C TInt SwitchLights(TInt aType,TInt aState) const;
    IMPORT_C TInt SetLightSensorSettings(TUint8 aParam1,TUint8 aParam2) const;
    IMPORT_C TInt GetLightSensorSettings(TUint8& aParam1,TUint8& aParam2) const;
};

#endif
