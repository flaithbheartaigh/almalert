#ifndef __LIGHTDRVIMPL_HPP__
#define __LIGHTDRVIMPL_HPP__

#include "lightdrv.hpp"
#include <DosClient.hpp>
#include <CommonEngine.hpp>

class CLightDrvImpl: public CLightDrv
{
  public:
    ~CLightDrvImpl();
  protected:
    CLightDrvImpl();
    void ConstructL(TBool aGameModeType);
  public:
    void DoSwitchLight(TInt aType,TInt aState);
    void DoSetGameMode(TInt aGameMode);
  private:
    void StartTimer(void);
    void StopTimer(void);
    static TInt Timeout(TAny* aLightDrv);
    void DoTimeout(void);
  private:
    TInt iGameModeType; //0x04
    RDosServer iServer; //0x08
    RDosLights iLights; //0x0c
    TInt iScreenState; //0x14
    TInt iKeysState; //0x18
    RSharedDataClient iSysAp; //0x1c
    CPeriodic* iTimer; //0x2c
    TTimeIntervalMicroSeconds32 iPeriod; //0x30
  friend class CLightDrv;
};

#endif
