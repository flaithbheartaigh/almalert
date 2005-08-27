#ifndef __LIGHTDRV_HPP__
#define __LIGHTDRV_HPP__

#include <e32base.h>

class CLightDrv: public CBase
{
  public:
    enum TType
    {
      EScreen,
      EKeys
    };
    enum TState
    {
      EOn,
      EOff
    };
  public:
    IMPORT_C ~CLightDrv();
    IMPORT_C static CLightDrv* NewL(TBool aGameModeType);
    IMPORT_C void SwitchLight(TInt aType,TInt aState);
    IMPORT_C void SetGameMode(TInt aGameMode);
  public:
    virtual char* Copyright(void) {return "fuck off!!!";};
    virtual void DoSwitchLight(TInt aType,TInt aState)=0;
    virtual void DoSetGameMode(TInt aGameMode)=0;
};

#endif
