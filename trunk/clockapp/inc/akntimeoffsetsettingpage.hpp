/*
    akntimeoffsetsettingpage.hpp
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

#ifndef __AKNTIMEOFFSETSETTINGPAGE_HPP__
#define __AKNTIMEOFFSETSETTINGPAGE_HPP__

#include <aknmfnesettingpage.h>

class CAknTimeOffsetSettingPage: public CAknMfneSettingPage
{
  public:
    CAknTimeOffsetSettingPage(TInt aResourceID,TTimeIntervalSeconds& aTimeOffsetValue);
    CAknTimeOffsetSettingPage(const TDesC* aSettingTitleText,TInt aSettingNumber,TInt aControlType,TInt aEditorResourceId,TInt aSettingPageResourceId,TTimeIntervalSeconds& aTimeOffsetValue);
    void ConstructL(void);
    CEikTimeOffsetEditor* TimeOffsetEditor(void);
  protected:
    ~CAknTimeOffsetSettingPage(void);
    void UpdateSettingL(void);
    void AcceptSettingL(void);
    void RestoreOriginalSettingL(void);
  private:
    TTimeIntervalSeconds& iTimeOffsetValue;
    TTimeIntervalSeconds iBackupTimeOffsetValue;
};

#endif
