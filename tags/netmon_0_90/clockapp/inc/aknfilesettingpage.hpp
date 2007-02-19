/*
    aknfilesettingpage.hpp
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

#ifndef __AKNFILESETTINGPAGE_HPP__
#define __AKNFILESETTINGPAGE_HPP__

#include <aknsettingpage.h>
#include <aknlists.h>

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
    void DynamicInitL(void);
    void UpdateCbaL(void);
  private:
    void ReadFilesL(const TDesC* aPos=NULL);
    void SetFolderL(const TDesC& aFolder);
    void UpdateFileL(void);
  private:
    RFs& iFs;
    TFileName& iFileValue;
    TFileName iBackupFileValue;
    CDesCArrayFlat* iFiles; //not owned
    TAknLayoutRect iOutlineFrame;
};

#endif
