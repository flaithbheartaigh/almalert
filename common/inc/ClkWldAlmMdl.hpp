/*
    ClkWldAlmMdl.hpp
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

#ifndef __CLKWLDALMMDL_HPP__
#define __CLKWLDALMMDL_HPP__

#include <e32base.h>
#include <clkmdlobserver.h>
#include <t32alm.h>
#include <ClkNitzMdl.hpp>

class CClkMdlResponder;

class CClkModelBase: public CBase
{
  public:
    IMPORT_C ~CClkModelBase(void);
  public:
    IMPORT_C void NotifyL(TInt aNotification);
    IMPORT_C void SetClkModelObserver(MClkModelObserver* aObserver);
    IMPORT_C void SetResponderActive(CClkMdlResponder* aResponder);
    IMPORT_C void Start(void);
    IMPORT_C void Stop(void);
  public:
    virtual void ProcessResponderL(TInt aResult)=0;
  protected:
    IMPORT_C CClkModelBase(void);
  private:
    void DoObserverNotifyL(TInt aResult);
  private:
    TBool iProgress; //0x04
    MClkModelObserver* iObserver; //0x08
    CClkMdlResponder* iResponder; //0x0c
};

class CClkMdlResponder: public CActive
{
  public:
    void Start(void);
  protected:
    IMPORT_C CClkMdlResponder(CClkModelBase& aClkModel,TInt aPriority);
    IMPORT_C void RunL(void);
    virtual void DoStart(void)=0;
  private:
    void NotifyModelL(TInt aNotification);
    CClkModelBase* iClkModel; //0x18
};

class CClkIntermediateObserver: public CBase,public MClkModelObserver
{
  public:
    ~CClkIntermediateObserver();
    IMPORT_C void NotifyL(TInt aNotification);
    IMPORT_C void SetClkModelObserver(MClkModelObserver* aObserver);
  public: //MClkModelObserver
    IMPORT_C void HandleUpdateL(TInt aNotification);
  protected:
    CClkIntermediateObserver();
  protected:
    static void Fault(TInt aCode);
  private:
    static void Panic(TInt aCode);
  private:
    MClkModelObserver* iObserver; //0x08
};

class CClkIdleObserver: public CClkIntermediateObserver
{
  public:
    IMPORT_C ~CClkIdleObserver();
  public:
    IMPORT_C virtual void DoIdleCancel(void);
    IMPORT_C virtual void DoIdleNotifyL(void);
    IMPORT_C void HandleUpdateL(TInt aNotification);
  protected:
    IMPORT_C CClkIdleObserver();
    IMPORT_C void ConstructL(MClkModelObserver* aObserver,TInt aPriority);
  private:
    void CancelIdleRefresh(void);
    void StartIdleRefresh(void);
    static TInt IdleCallBackL(TAny* aObserver);
  private:
    TInt iNotification; //0x0c
    CIdle* iIdle; //0x10
};

class CClkAlmModel;

class CClkAlmResponder: public CClkMdlResponder
{
  public:
    CClkAlmResponder(RAlarmServer* aServer,CClkAlmModel* aModel,TInt aPriority);
  protected:
    void DoStart(void);
    void DoCancel(void);
  private:
    RAlarmServer* iServer; //0x1c
};

struct SClockAlarmInfo
{
  TTime iAlarmTime;
  TAlarmSetState iState;
  TAlarmMessage iMessage;
  TAlarmSoundName iSound;
  TAlarmClockRepeat iRepeat;
};

class CClkAlmModel: public CClkModelBase
{
  public:
    IMPORT_C static CClkAlmModel* NewL(MClkModelObserver* aObserver,TInt aPriority);
    IMPORT_C ~CClkAlmModel();
  public:
    void ProcessResponderL(TInt aResult);
  public:
    IMPORT_C TInt ClockAlarmEnable(TInt aAlarmId,TAlarmSetState aState);
    IMPORT_C TInt ClockAlarmInfo(TInt aClockAlarmIndex,SClockAlarmInfo& anInfo) const;
    IMPORT_C void ClockAlarmSet(TInt aClockAlarmIndex,const SClockAlarmInfo& anInfo);
    IMPORT_C TAlarmSetState ClockAlarmState(TInt aAlarmId) const;
    IMPORT_C TInt ClockAlarmType(TAlarmType& aType,TInt aAlarmId) const;
    IMPORT_C TInt ClockAlarmDelete(TInt aAlarmId);
  private:
    void ConstructL(MClkModelObserver* aObserver,TInt aPriority);
  private:
    RAlarmServer iServer;
};

class CClkAlmIdleObserver: public CClkIdleObserver
{
  public:
    IMPORT_C static CClkAlmIdleObserver* NewL(MClkModelObserver* aObserver,TInt aPriority);
};


class CClkNitzIdleObserver: public CClkIdleObserver
{
  public:
    IMPORT_C static CClkNitzIdleObserver* NewL(MClkModelObserver* aObserver,TInt aPriority);
};

class CClkNitzModel;

class CClkNitzResponder: public CClkMdlResponder
{
  public:
    CClkNitzResponder(RClkNitzMdlServer* aServer,CClkNitzModel* aModel,TInt aPriority);
  protected:
    void DoStart(void);
    void DoCancel(void);
  private:
    RClkNitzMdlServer* iServer; //0x1c
};

class CClkNitzModel: public CClkModelBase
{
  public:
    IMPORT_C static CClkNitzModel* NewL(MClkModelObserver* aObserver,TInt aPriority);
    IMPORT_C ~CClkNitzModel();
  public:
    void ProcessResponderL(TInt aResult);
  public:
    IMPORT_C TInt NitzState(TInt& aSetting) const;
    IMPORT_C TInt SetNitzState(TInt aSetting);
    IMPORT_C TInt GetCurrentNITZInfo(MAdvGsmPhoneNitz::TNITZInfo& aInfo);
  private:
    void ConstructL(MClkModelObserver* aObserver,TInt aPriority);
    void StartNitzSrvL(void);
    TInt IsNitzSrvRunning(void);
  private:
    RClkNitzMdlServer iServer;
};

#endif
