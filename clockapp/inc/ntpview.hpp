/*
    ntpview.hpp
    Copyright (C) 2007 zg

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

#ifndef __NTPVIEW_HPP__
#define __NTPVIEW_HPP__

#include <aknview.h>
#include <aknsettingitemlist.h>

class CNtpControl;
class CNtpView: public CAknView
{
  public:
    static CNtpView* NewLC(void);
    ~CNtpView();
  private:
    CNtpView();
    void ConstructL(void);
  public: //CAknView
    TUid Id(void) const;
    void HandleCommandL(TInt aCommand);
    void HandleClientRectChange(void);
  private: //CAknView
    void DoActivateL(const TVwsViewId& aPrevViewId,TUid aCustomMessageId,const TDesC8& aCustomMessage);
    void DoDeactivate(void);
  private:
    CNtpControl* iControl;
    TFileName iServer;
    TInt iPort;
    TTimeIntervalSeconds iCorrection;
};

class CNtpControl: public CAknSettingItemList
{
  public:
    static CNtpControl* NewL(const TRect& aRect,TFileName& aServer,TInt& aPort,TTimeIntervalSeconds& aCorrection);
    ~CNtpControl();
  protected: //CAknSettingItemList
    CAknSettingItem* CreateSettingItemL(TInt aSettingId);
  public: //CAknSettingItemList
    void StoreSettingsL(void);
  private:
    CNtpControl(TFileName& aServer,TInt& aPort,TTimeIntervalSeconds& aCorrection);
    void ConstructL(const TRect& aRect);
  private:
    TFileName& iServer;
    TInt& iPort;
    TBuf<16> iPortStr;
    TTimeIntervalSeconds& iCorrection;
};

#endif
