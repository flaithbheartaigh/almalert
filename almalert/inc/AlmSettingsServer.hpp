/*
    AlmSettingsServer.hpp
    Copyright (C) 2006 zg

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

#ifndef __ALMSETTINGSSERVER_HPP__
#define __ALMSETTINGSSERVER_HPP__

#include <e32base.h>
#include <d32dbms.h>

class CAlmSettingsServer: public CServer
{
  public:
    static CAlmSettingsServer* NewLC(void);
    ~CAlmSettingsServer();
    static TInt ThreadFunction(TAny* aNone);
    void IncrementSessions(void);
    void DecrementSessions(void);
    inline RDbDatabase& Db(void) {return iBase;};
  private:
    CAlmSettingsServer(TInt aPriority);
    void ConstructL(void);
    static void ThreadFunctionL(void);
  private: //CServer
    CSharableSession* NewSessionL(const TVersion& aVersion) const;
  private:
    TInt iSessionCount;
    RDbNamedDatabase iBase;
    RDbs iSession;
};

typedef void (*TSettingsGetFunctionL)(const RDbView& aViev,TAny* aData);

class CAlmSettingsSession: public CSession
{
  public:
    static CAlmSettingsSession* NewL(RThread& aClient,CAlmSettingsServer& aServer);
    ~CAlmSettingsSession();
  public: //CSession
    void ServiceL(const RMessage& aMessage);
  private:
    CAlmSettingsSession(RThread& aClient,CAlmSettingsServer& aServer);
    void ConstructL(void);
    void DispatchMessageL(const RMessage& aMessage);
    HBufC8* ValueLC(const TAny* aSrc);
    TUint32 CategoryL(const TAny* aSrc);
  private:
    CAlmSettingsServer& iServer;
};

#endif
