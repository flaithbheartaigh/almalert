#ifndef __BACKLIGHTCTRLIMPL_HPP__
#define __BACKLIGHTCTRLIMPL_HPP__

#include <backlightctrl.hpp>
#include <coemain.h>
#include "backlighttimer.hpp"

class CBackLightControlImpl: public CBackLightControl,public MCoeForegroundObserver,public MTimeOutNotify
{
  public:
    CBackLightControlImpl(MBackLightControlObserver* aCallback);
    void ConstructL(void);
    ~CBackLightControlImpl();
  public: //MCoeForegroundObserver
    void HandleGainingForeground(void);
    void HandleLosingForeground(void);
  public: //MTimeOutNotify
    void TimerExpired(TUint aParam);
  public:
    TInt BackLightOn(TInt aType,TUint16 aDuration);
    TInt BackLightBlink(TInt aType,TUint16 aDuration,TUint16 aOnTime,TUint16 aOffTime);
    TInt BackLightOff(TInt aType);
    TInt BackLightChange(TInt aType,TUint16 aDuration);
    TInt BackLightState(TInt aType);
  public:
    static const TDesC8& Copyright(void);
  private:
    enum TType
    {
      EScreen,
      EKeys,
      EScreenBlink,
      EKeysBlink
    };
    struct SBlink
    {
      TUint16 iOn;
      TUint16 iOff;
    };
  private:
    void UpdateState(TInt aType,TInt aState,TUint16 aDuration,SBlink aBlink);
    TInt Switch(void);
    TInt Start(TInt aType,TUint16 aDuration);
  private:
    MBackLightControlObserver* iCallback;
    TBool iScreenState;
    TBool iKeysState;
    TBool iScreenCurrentState;
    TBool iKeysCurrentState;
    TBool iScreenBlink;
    TBool iKeysBlink;
    TBool iScreenCurrentBlink;
    TBool iKeysCurrentBlink;
    CBackLightTimer* iScreen;
    CBackLightTimer* iKeys;
    CBackLightTimer* iScreenBlinker;
    CBackLightTimer* iKeysBlinker;
    SBlink iScreenTime;
    SBlink iKeysTime;
    SBlink iScreenCurrentTime;
    SBlink iKeysCurrentTime;
};

#endif
