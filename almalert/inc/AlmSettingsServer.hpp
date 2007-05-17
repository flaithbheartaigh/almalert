/*
    AlmSettingsServer.hpp
    Copyright (C) 2006-2007 zg

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
#include "AlmLock.hpp"

class CAlmSettingsServer: public CServer,public CLockNotifier
{
  public:
    static CAlmSettingsServer* NewLC(void);
    ~CAlmSettingsServer();
    static TInt ThreadFunction(TAny* aNone);
    void IncrementSessions(void);
    void DecrementSessions(void);
    RDbDatabase& DbL(void);
    void Notify(void);
  public: //CLockNotifier
    void LockNotifyL(TBool aState);
  private:
    CAlmSettingsServer(TInt aPriority);
    void ConstructL(void);
    static void ThreadFunctionL(void);
    static void SignalL(void);
    void DbOpenL(void);
    void DbClose(void);
  private: //CServer
    CSharableSession* NewSessionL(const TVersion& aVersion) const;
  private:
    TInt iSessionCount;
    RDbNamedDatabase iBase;
    RDbs iSession;
    TBool iLock;
};

typedef void (*TSettingsGetFunctionL)(const RDbView& aViev,TAny* aData);

class CAlmSettingsSession: public CSession
{
  public:
    static CAlmSettingsSession* NewL(RThread& aClient,CAlmSettingsServer& aServer);
    ~CAlmSettingsSession();
    void Notify(void);
  public: //CSession
    void ServiceL(const RMessage& aMessage);
  private:
    CAlmSettingsSession(RThread& aClient,CAlmSettingsServer& aServer);
    void ConstructL(void);
    void DispatchMessageL(const RMessage& aMessage);
    void ProcessDataL(void);
    HBufC8* ValueLC(const TAny* aSrc);
    TUint32 CategoryL(const TAny* aSrc);
    void ProcessCompactL(void);
    void CancelNotyfy(void);
  private:
    CAlmSettingsServer& iServer;
    RMessagePtr iMsgPtr;
    TBool iActive;
    TBool iComplete;
};

#endif
