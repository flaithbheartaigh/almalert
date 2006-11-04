/*
    settingsex.hpp
    Copyright (C) 2006 zg

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

#ifndef __SETTINGSEX_HPP__
#define __SETTINGSEX_HPP__

#include <aknview.h>
#include <aknsettingitemlist.h>
#include <aknlists.h>

class CSettingsControl;
class CSettingsView: public CAknView
{
  public:
    static CSettingsView* NewLC(void);
    ~CSettingsView();
  private:
    void ConstructL(void);
  public: //CAknView
    TUid Id(void) const;
    void HandleCommandL(TInt aCommand);
    void HandleClientRectChange(void);
  private: //CAknView
    void DoActivateL(const TVwsViewId& aPrevViewId,TUid aCustomMessageId,const TDesC8& aCustomMessage);
    void DoDeactivate(void);
  private:
    CSettingsControl* iControl;
};

class CSettingsControl: public CAknSettingItemList
{
  public:
    static CSettingsControl* NewL(const TRect& aRect);
    ~CSettingsControl();
  protected: //CAknSettingItemList
    CAknSettingItem* CreateSettingItemL(TInt aSettingId);
  public: //CAknSettingItemList
    void HandleListBoxEventL(CEikListBox* aListBox,TListBoxEvent aEventType);
  private:
    CSettingsControl(void);
    void ConstructL(const TRect& aRect);
  private:
    TFileName iAlarmTone;
};

class CAknFileSettingItem: public CAknSettingItem
{
  public:
    CAknFileSettingItem(TInt aIdentifier,TFileName& aText);
    ~CAknFileSettingItem();
    void StoreL(void);
    void LoadL(void);
    const TDesC& SettingTextL(void);
    void EditItemL(TBool aCalledFromMenu);
  private:
    IMPORT_C void CAknSettingItem_Reserved();
  private:
    TFileName& iExternalText;
    TFileName iInternalText;
};

class CAknFileSettingPage: public CAknSettingPage
{
  public:
    CAknFileSettingPage(TInt aResourceID,TFileName& aFileValue);
    CAknFileSettingPage(const TDesC* aSettingTitleText,TInt aSettingNumber,TInt aControlType,TInt aEditorResourceId,TInt aSettingPageResourceId,TFileName& aFileValue);
    CAknSingleGraphicPopupMenuStyleListBox* FileControl(void);
    void ConstructL(void);
    TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
  protected:
    void SizeChanged(void);
    void Draw(const TRect& aRect) const;
  protected:
    ~CAknFileSettingPage();
    void UpdateSettingL(void);
    void AcceptSettingL(void);
    void RestoreOriginalSettingL(void);
    void CheckAndSetDataValidity(void);
  private:
    void ReadFilesL(void);
    void SetFolderL(const TDesC& aFolder);
    void UpdateFileL(void);
  private:
    TFileName& iFileValue;
    TFileName iBackupFileValue;
    CDesCArrayFlat* iFiles; //not owned
};

#endif
