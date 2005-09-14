#ifndef __BACKLIGHTCTRL_HPP__
#define __BACKLIGHTCTRL_HPP__

#include <e32base.h>

class MBackLightControlObserver;

class CBackLightControl: public CBase
{
  public:
    enum TBackLightType
    {
      EBackLightTypeScreen=0,
      EBackLightTypeKeys,
      EBackLightTypeBoth
    };
    enum TBackLightState
    {
      EBackLightStateOn=0,
      EBackLightStateOff,
      EBackLightStateBlink,
      EBackLightStateUnknown
    };
  public:
    virtual TInt BackLightOn(TInt aState,TUint16 aDuration)=0;
    virtual TInt BackLightBlink(TInt aType,TUint16 aDuration,TUint16 aOnTime,TUint16 aOffTime)=0;
    virtual TInt BackLightOff(TInt aType)=0;
    virtual TInt BackLightChange(TInt aType,TUint16 aDuration)=0;
    virtual TInt BackLightState(TInt aType)=0;
  public:
    static CBackLightControl* NewL(void);
    static CBackLightControl* NewL(MBackLightControlObserver* aCallback);
    static CBackLightControl* NewLC(MBackLightControlObserver* aCallback);
    ~CBackLightControl();
  protected:
    CBackLightControl();
};

class MBackLightControlObserver
{
  public:
    virtual void ScreenNotify(TInt aState)=0;
    virtual void KeysNotify(TInt aState)=0;
};

#endif
