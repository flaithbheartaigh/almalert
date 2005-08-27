/*
    lightdrv.hpp
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
