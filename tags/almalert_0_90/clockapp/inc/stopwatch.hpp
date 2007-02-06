/*
    stopwatch.hpp
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

#ifndef __STOPWATCH_HPP__
#define __STOPWATCH_HPP__

#include <aknview.h>
#include <coemain.h> //MCoeForegroundObserver

class CStopWatchControl;
class CStopWatchView: public CAknView
{
  public:
    static CStopWatchView* NewLC(void);
    ~CStopWatchView();
  private:
    void ConstructL(void);
  public: //CAknView
    TUid Id(void) const;
    void HandleCommandL(TInt aCommand);
    void HandleClientRectChange(void);
  private: //CAknView
    void DoActivateL(const TVwsViewId& aPrevViewId,TUid aCustomMessageId,const TDesC8& aCustomMessage);
    void DoDeactivate(void);
  private:
    CStopWatchControl* iControl;
};

class CStopWatchControl: public CCoeControl,public MCoeForegroundObserver
{
  public:
    static CStopWatchControl* NewL(const TRect& aRect);
    ~CStopWatchControl();
  public:
    TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
    void Draw(const TRect& aRect) const;
  private:
    CStopWatchControl(void);
    void ConstructL(const TRect& aRect);
  public: //MCoeForegroundObserver
    void HandleGainingForeground(void);
    void HandleLosingForeground(void);
  public:
    enum TSignal
    {
      ESignalStop,
      ESignalReset,
      ESignalLast
    };
  public:
    void ProcessSignal(TSignal aSignal);
  private:
    enum TStates
    {
      EStateClear,
      EStateStart,
      EStateStop,
      EStateLap1,
      EStateLap2,
      EStateLap2Visible,
      EStateLast
    };
  private:
    void ProcessNothing(void);
    void ProcessStart(void);
    void ProcessStop(void);
    void ProcessRestart(void);
    void ProcessLap(void);
    void CurrentTime(TDes& aTime) const;
    void InitRefresh(void);
    static TInt RefreshTimeout(TAny* aContainer);
  private:
    TStates iState;
    TTime iStart;
    TTime iStop;
    TTime iLap;
    TInt64 iPause;
    CPeriodic* iRefresh;
};

const TInt KWhiteColor=0;
const TInt KBlackColor=0xd7;

#endif
