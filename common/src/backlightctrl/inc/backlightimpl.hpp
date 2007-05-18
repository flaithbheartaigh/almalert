#ifndef __BACKLIGHTCTRLIMPL_HPP__
#define __BACKLIGHTCTRLIMPL_HPP__

#include <backlightctrl.hpp>
#include <coemain.h>
#include <phonet.hpp>

#include "backlighttimer.hpp"

class CIsiMsg;
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
    TInt BackLightOff(TInt aType,TUint16 aDuration);
    TInt BackLightChange(TInt aType,TUint16 aDuration);
    TInt BackLightState(TInt aType);
    TInt SetScreenBrightness(TInt aState,TUint16 aDuration);
    TInt ScreenBrightness(void);
  public:
    static const TDesC8& Copyright(void);
  private:
    enum TType
    {
      EScreen,
      EKeys,
      EScreenBlink,
      EKeysBlink,
      EBrightness
    };
    struct SBlink
    {
      TUint16 iOn;
      TUint16 iOff;
    };
  private:
    void UpdateState(TInt aType,TInt aState,TUint16 aDuration,SBlink aBlink);
    TInt Switch(void);
    void UpdateBrightness(TUint8 aBrightness,TUint16 aDuration);
    TInt SwitchBrightness(void);
    TInt Start(TInt aType,TUint16 aDuration);
    TInt NormalizeBrightness(TUint8 aBrightness);
    TInt SetScreenBrightnessInternal(TUint8 aState,TUint16 aDuration);
  private:
    MBackLightControlObserver* iCallback;
    TUint8 iBrightnessOriginalState;
    TBool iScreenState;
    TBool iKeysState;
    TUint8 iBrightnessState;
    TBool iScreenCurrentState;
    TBool iKeysCurrentState;
    TUint8 iBrightnessCurrentState;
    TBool iScreenBlink;
    TBool iKeysBlink;
    TBool iScreenCurrentBlink;
    TBool iKeysCurrentBlink;
    CBackLightTimer* iScreen;
    CBackLightTimer* iKeys;
    CBackLightTimer* iScreenBlinker;
    CBackLightTimer* iKeysBlinker;
    CBackLightTimer* iBrightness;
    SBlink iScreenTime;
    SBlink iKeysTime;
    SBlink iScreenCurrentTime;
    SBlink iKeysCurrentTime;
  private:
    void Open(void);
    void Close(void);
    void SetGameModeL(TBool aState);
    void SwitchL(TInt aType,TInt aState);
    void SetBrightnessL(TUint8 aValue);
    void BrightnessL(TUint8& aValue);
    CIsiMsg* SendAndReceiveLC(CIsiMsg* aSend);
    void SendL(CIsiMsg* aSend);
  private:
    enum TSwitchType
    {
      ESwitchScreen=1,
      ESwitchKeys=2,
      ESwitchBoth=3
    };
    enum TSwitchState
    {
      ESmoothOn=1,
      ESmoothOff=2,
      EBlink=3,
      EBlinkAlways=4,
      EOn=5,
      EUnlock=0xa1,
      ELock=0xa2
    };
  private:
    TBool iCaptured;
    RMutex iMutex;
    RPhoNet iPhoNet;
    TUint8 iObjectNumber;
};

#endif
