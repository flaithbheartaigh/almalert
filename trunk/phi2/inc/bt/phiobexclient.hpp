/*
    phiobexclient.hpp
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

#ifndef __PHIOBEXCLIENT_HPP__
#define __PHIOBEXCLIENT_HPP__

#include <e32base.h>
#include <obex.h>
#include <aknprogressdialog.h>

class CPhiObexServiceSearcher;

class CPhiObexClient: public CActive,public MProgressDialogCallback
{
  public:
    static void NewLD(const TDesC& aFile);
    ~CPhiObexClient();
  public: //MProgressDialogCallback
    void DialogDismissedL(TInt aButtonId);
  protected:
    void DoCancel(void);
    void RunL(void);
  private:
    CPhiObexClient(const TDesC& aFile);
    void ConstructL(const TDesC& aFile);
    void CleanupL(void);
    void ConnectToServerL(void);
    void ConnectL(void);
    void DisconnectL(void);
    void SendObjectL(void);
    //dialog
    static TInt RedrawCallBack(TAny* aPhiObexClient);
    TInt DoRedraw(void);
  private:
    enum TState
    {
      EIdle, //idle
      EGettingDevice, //searching for a device
      EGettingService, //searching for a service
      EGettingConnection, //connecting to a service on a remote machine
      EWaitingToSend, //sending a message to the remote machine
      EDisconnecting //disconnecting the server
    };
    TState iState;
    CPhiObexServiceSearcher* iServiceSearcher;
    CObexClient* iClient;
    CObexFileObject* iCurrObject;
    //dialog
    CAknProgressDialog* iProgressDialog;
    CEikProgressInfo* iProgressInfo;
    CPeriodic* iRedrawTimer;
};

#endif
