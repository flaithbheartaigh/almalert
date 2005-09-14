#ifndef __BACKLIGHTTIMER_HPP__
#define __BACKLIGHTTIMER_HPP__

#include <e32base.h>

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
    CBackLightTimer(MTimeOutNotify* aTimeOutNotify,TUint aParam);
    void RunL(void);
  private:
    MTimeOutNotify* iNotify;
    TUint iParam;
};

#endif
