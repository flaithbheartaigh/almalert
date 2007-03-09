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

class MTimeOutNotify
{
  public:
    virtual void TimerExpired(void)=0;
};

class CNtp: public CActive,public MTimeOutNotify
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
  public: //MTimeOutNotify
    void TimerExpired(void);
  protected: //CActive
    void DoCancel(void);
    void RunL(void);
  private:
    enum TState
    {
      EIdle,
      ELookingUp,
      ESending,
      EReceiving,
      ESending2,
      EReceiving2
    };
  private:
    CNtp(const TDesC& aServer,TInt aPort,TTimeIntervalSeconds aCorrection);
    void ConstructL(void);
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
    TTimeIntervalSeconds iTimeOffset;
    TTimeIntervalSeconds iCorrection;
};

#endif
