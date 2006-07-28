/*
    hwtricks.hpp
    Copyright (C) 2005-2006 zg

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

class CIsiMsg;
class CHWServer: public CBase
{
  public:
    IMPORT_C static CHWServer* NewLC(void);
  public:
    virtual void SendL(CIsiMsg& aMsg)=0;
    virtual void ReceiveL(TRequestStatus& aStatus,CIsiMsg& aMsg,TPckgBuf<TUint16>& aLen)=0;
    virtual void ReceiveCancelL(void)=0;
  public: //for internal use
    static CHWServer* NewLC(TBool anExtended);
    static CIsiMsg* SendAndReceiveLC(CIsiMsg* aSend);
    static void SendL(CIsiMsg* aSend);
};

class HWVibra
{
  public:
    static const TUint8 KMaxIntensity=100;
    static const TUint8 KDefaultIntensity=40;
  public:
    IMPORT_C static void SwitchL(TBool aState);
    IMPORT_C static void StateL(TBool& aState);
    IMPORT_C static void SetIntensityL(TUint8 anIntensity);
    IMPORT_C static void IntensityL(TUint8& anIntensity);
};

class HWBacklight
{
  public:
    struct SParam
    {
      TUint8 iType;
      TUint8 iValue1;
      TUint8 iValue2;
      SParam(TUint8 aType,TUint8 aValue1,TUint8 aValue2): iType(aType),iValue1(aValue1),iValue2(aValue2) {};
    };
  public:
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
    enum TBrightnessBlock
    {
      EBlockOff=1,
      EBlockOn=2
    };
    enum TBrightnessTimer
    {
      ETimerOn=1,
      ETimerOff=2
    };
    enum TBrightnessSmooth
    {
      ESmoothSlow=1,
      ESmoothFast=2
    };
    enum TBrightnessType
    {
      EBrightnessBlock=0,
      EBrightnessTimer,
      EBrightnessBlink, //aValue1 - on time, aValue2 - off time. all times in 100 ms.
      EBrightnessSmooth,
      EBrightnessSensor,
      EBrightnessScreen,
      EBrightnessKeys
    };
  public:
    static const TUint8 KMaxBrightness=100;
  public:
    IMPORT_C static void SetGameModeL(TBool aState);
    IMPORT_C static void SwitchL(TInt aType,TInt aState);
    IMPORT_C static void Reserved_1(void);
    IMPORT_C static void SetBrightnessL(TBrightnessType aType,TUint8 aValue1,TUint8 aValue2=0);
    IMPORT_C static void BrightnessL(TBrightnessType aType,TUint8& aValue1,TUint8& aValue2);
    IMPORT_C static void SwitchL(TInt aType,TInt aState,const RArray<SParam>& aParams);
    IMPORT_C static void Reserved_3(void);
    IMPORT_C static void SetBrightnessL(const RArray<SParam>& aParams);
    IMPORT_C static void BrightnessL(RArray<SParam>& aParams);
};

class HWOther
{
  public:
    enum TInfoType
    {
      EInfoBuild,
      EInfoServerSupported,
      EInfoVibraSupported,
      EInfoSwitchLightsSupported,
      EInfoBrightnessSupported,
      EInfoContrastSupported,
      EInfoNetmonSupported,
      EInfoNetmonExtendedSupported
    };
  public:
    IMPORT_C static void InfoL(TInfoType aType,TInt& aValue);
};

class HWLcd
{
  public:
    IMPORT_C static void SetContrastL(TInt aContrast);
    IMPORT_C static void ContrastL(TInt& aContrast);
    IMPORT_C static void MaxContrastL(TInt& aMaxContrast);
};

class HWNetmon
{
  public:
    enum TFlags
    {
      ERaw=1,
      EExt=2,
      ESigned=4
    };
    enum TBandFrequency
    {
      EDefault,
      E900MHz,
      E1800MHz,
      E1900MHz
    };
  public:
    static const TUint16 KOffChannel=3333;
  public:
    IMPORT_C static void ValueL(TUint8 aUnit,TUint16 anAddress,TDes16& aValue,TUint32 aFlags=0);
    IMPORT_C static void SetChannelL(TUint16 aChannel=KOffChannel);
    IMPORT_C static void SetFrequencyL(TBandFrequency aFrequency);
};

#endif
