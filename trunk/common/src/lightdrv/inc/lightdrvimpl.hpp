/*
    lightdrvimpl.hpp
    Copyright (C) 2005 zg

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
    TInt DoSetGameMode(TInt aGameMode);
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
