#ifndef __BACKLIGHTTIMER_HPP__
#define __BACKLIGHTTIMER_HPP__

#include <e32base.h>
#include <backlightctrl.hpp>
#include "backlightimpl.hpp"

class MTimeOutNotify
{
  public:
    virtual void TimerExpired(TUint aParam)=0;
};

class CBackLightTimer: public CTimer
{
  public:
    static CBackLightTimer* NewL(MTimeOutNotify* aTimeOutNotify,TUint aParam);
    ~CBackLightTimer();
    TInt Start(TInt anInterval);
  protected:
    CBackLightTimer(MTimeOutNotify* aTimeOutNotify,TUint aParam); //EPriorityHigh
    void DoCancel(void);
    void RunL(void);
  private:
    MTimeOutNotify* iNotify;
    TInt iUnknown1;
    TUint iParam;
};

class CBackLightTimerContainer: public MTimeOutNotify
{
  public:
    static CBackLightTimerContainer* NewL(MBackLightNotify* aCallback,TInt aType,CLightDrv* aDrv);
    virtual ~CBackLightTimerContainer();
  public:
    void TimerExpired(TUint aParam);
  public:
    TInt BackLightSwitch(TInt aState,TUint16 aDuration);
    TInt StartBlink(TUint16 aOnTime,TUint16 aOffTime,TUint16 aDuration);
    TInt BackLightChange(TUint16 aDuration);
    TInt BackLightState(void);
    TInt ResumeBackLight(void);
    void PauseBackLight(void);
  private:
    CBackLightTimerContainer(MBackLightNotify* aCallback,TInt aType,CLightDrv* aDrv);
    void ConstructL(void);
  private:
    TInt CancelBlink(void);
    TInt CancelCommon(void);
    TInt NormalizeType(TInt aType);
    TInt NormalizeState(TInt aState);
  private:
    CBackLightTimer* iCommonTimer; //0x04
    CBackLightTimer* iBlinkTimer; //0x08
    CBackLightTimer* iBlinkPeriodTimer; //0x0c
    TInt iState; //0x10
    TInt iUnknown02; //0x14
    TInt iUnknown03; //0x18
    TInt iUnknown04; //0x1c
    TInt iUnknown05; //0x20
    TInt iUnknown06; //0x24
    TInt iType; //0x28
    TBool iBlink; //0x2c
    TInt iUnknown09; //0x30
    TInt iUnknown0a; //0x34
    TInt iUnknown0b; //0x38
    TUint iUnknown0c; //0x3c
    TUint iUnknown0d; //0x40
    TTime iBlinkStartTime; //0x44
    TTime iStartTime; //0x4c
    MBackLightNotify* iCallback; //0x54
    CLightDrv* iDrv; //0x58
};

#endif
