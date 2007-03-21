/*
    ntp.hpp
    Copyright (C) 2007 zg

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

#ifndef __NTP_HPP__
#define __NTP_HPP__

#include <in_sock.h>
#include <aknwaitdialog.h>

class CNtp: public CActive,public MProgressDialogCallback
{
  public:
    enum
    {
      KMaxServerNameLength=50,
      KNtpPacketMin=48,
      KNtpPacketMax=68,
      KNtpModeClient=3,
      KNtpVersion=1
    };
  public:
    static void NewLD(const TDesC& aServer,TInt aPort,TTimeIntervalSeconds aCorrection);
    ~CNtp();
  public: //MProgressDialogCallback
    void DialogDismissedL(TInt aButtonId);
  protected: //CActive
    void DoCancel(void);
    void RunL(void);
  private:
    enum TState
    {
      EIdle,
      ELookingUp,
      ESending,
      EReceiving
    };
  private:
    CNtp(const TDesC& aServer,TInt aPort,TTimeIntervalSeconds aCorrection);
    void ConstructL(void);
    void CleanupL(void);
  private:
    TState iNtpState;
    TBuf<KMaxServerNameLength> iServerName;
    TInt iPort;
    RSocketServ iSocketServ;
    RSocket iSocket;
    RHostResolver iResolver;
    TNameEntry iNameEntry;
    TInetAddr iAddress;
    TBuf8<KNtpPacketMin> iSendBuffer;
    TBuf8<KNtpPacketMax> iReceiveBuffer;
    TTime iSendStamp;
    TTime iReceiveStamp;
    TTime iNewStamp;
    TTimeIntervalSeconds iTimeOffset;
    TTimeIntervalSeconds iCorrection;
    CAknWaitDialog* iWaitDialog;
};

#endif
