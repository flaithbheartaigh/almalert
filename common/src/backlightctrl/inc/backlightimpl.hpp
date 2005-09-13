#ifndef __BACKLIGHTCTRLIMPL_HPP__
#define __BACKLIGHTCTRLIMPL_HPP__

#include <backlightctrl.hpp>
#include <coemain.h>
#include "../../lightdrv/inc/lightdrv.hpp"

class CBackLightTimerContainer;

class MBackLightNotify
{
  public:
    virtual void BackLightNotify(TInt aType,TInt aState)=0;
};

class CBackLightControlImpl: public CBackLightControl,public MBackLightNotify,public MCoeForegroundObserver
{
  public:
    CBackLightControlImpl(MBackLightControlObserver* aCallback);
    void ConstructL(void);
    ~CBackLightControlImpl();
  public: //MBackLightNotify
    void BackLightNotify(TInt aType,TInt aState);
  public: //MCoeForegroundObserver
    void HandleGainingForeground(void);
    void HandleLosingForeground(void);
  public:
    TInt BackLightOn(TInt aType,TUint16 aDuration);
    TInt BackLightBlink(TInt aType,TUint16 aDuration,TUint16 aOnTime,TUint16 aOffTime);
    TInt BackLightOff(TInt aType);
    TInt BackLightChange(TInt aType,TUint16 aDuration);
    TInt BackLightState(TInt aType);
  public:
    static void Panic(TInt aPanic);
    static TBool IsOriginalNGage(void);
  private:
    TInt NormalizeType(TInt aType);
  private:
    MBackLightControlObserver* iCallback; //0x0c
    TBool iExtended; //0x10
    CBackLightTimerContainer* iTimerKeys; //0x14
    CBackLightTimerContainer* iTimerScreen; //0x18
    CLightDrv* iDrv; //0x1c
};

#endif
