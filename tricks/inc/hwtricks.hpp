/*
    hwtricks.hpp
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

#ifndef __HWTRICKS_HPP__
#define __HWTRICKS_HPP__

#include <e32base.h>

typedef TPckgBuf<TUint16> TPnReceiveAllocationLengthPckg;

class CPnMsg;

class CHWServer: public CBase
{
  public:
    IMPORT_C static CHWServer* NewLC(void);
  public:
    virtual void SendL(CPnMsg& aMsg)=0;
    virtual void ReceiveL(TRequestStatus& aStatus,CPnMsg& aMsg,TPnReceiveAllocationLengthPckg& aLen)=0;
    virtual void ReceiveCancelL(void)=0;
};

class HWVibra
{
  public:
    IMPORT_C static void SwitchL(TBool aState);
    IMPORT_C static void Reserved_1(void);
    IMPORT_C static void Reserved_2(void);
    IMPORT_C static void Reserved_3(void);
};

class HWBacklight
{
  public:
    enum TType
    {
      EScreen=1,
      EKeys=2,
      EBoth=3
    };
    enum TState
    {
      ESlowOn=1,
      ESlowOff=2,
      EBlink1=3,
      EBlink2=4,
      EOn=5
    };
  public:
    IMPORT_C static void GameModeL(TBool aState);
    IMPORT_C static void SwitchL(TInt aType,TInt aState);
    IMPORT_C static void Reserved_1(void);
    IMPORT_C static void SetBrightnessL(TUint8 aBrightness);
    IMPORT_C static void BrightnessL(TUint8& aBrightness);
    IMPORT_C static void Reserved_2(void);
    IMPORT_C static void Reserved_3(void);
    IMPORT_C static void Reserved_4(void);
    IMPORT_C static void Reserved_5(void);
};

class HWOther
{
  public:
    enum TInfoType
    {
      EInfoBuild,
      EInfoServerSupported,
      EInfoVibraSupported,
      ESwitchLightsSupported,
      EBrightnessSupported
    };
  public:
    IMPORT_C static void InfoL(TInfoType aType,TInt& aValue);
};

#endif
