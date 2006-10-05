/*
    vibraimpl.hpp
    Copyright (C) 2005 zg

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __VIBRAIMPL_HPP__
#define __VIBRAIMPL_HPP__

#include <vibractrl.hpp>
#include <CommonEngine.hpp>
#include <coemain.h>
#include <phonet.hpp>

#include "vibratimer.hpp"

class CVibraControlImpl: public CVibraControl,public MSharedDataNotifyHandler,public MTimeOutNotify,public MCoeForegroundObserver
{
  public:
    ~CVibraControlImpl();
  public: //CVibraControl
    void StartVibraL(TUint16 aDuration);
    void StopVibraL(void);
    TVibraModeState VibraSettings(void) const;
    void StartVibraL(TUint16 aDuration,TInt aIntensity);
  public: //MCoeForegroundObserver
    void HandleGainingForeground(void);
    void HandleLosingForeground(void);
  public: //MSharedDataNotifyHandler
    void HandleNotifyL(TUid anUid,const TDesC16& aKey,const TDesC16& aValue);
  public: //MTimeOutNotify
    void TimerExpired(void);
  public:
    static const TDesC8& Copyright(void);
  private:
    CVibraControlImpl(MVibraControlObserver* aCallback);
    void ConstructL(void);
    static void DoCleanup(TAny* aPtr);
    static void DoCleanupIntensity(TAny* aPtr);
  private:
    void Open(void);
    void Close(void);
    void Send(TBool aState);
    void SendL(TBool aState);
  private:
    MVibraControlObserver* iCallback;
    RSharedDataClient iShared;
    TVibraModeState iVibraState;
    CVibraTimer* iTimer;
    TBool iCaptured;
    RMutex iMutex;
    RPhoNet iPhoNet;
    TUint8 iObjectNumber;
  friend class VibraFactory;
};

#endif
