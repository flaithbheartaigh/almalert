/*
    akntimeoffsetsettingpage.cpp
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

#include "akntimeoffsetsettingpage.hpp"

CAknTimeOffsetSettingPage::~CAknTimeOffsetSettingPage()
{
}

CAknTimeOffsetSettingPage::CAknTimeOffsetSettingPage(TInt aResourceID,TTimeIntervalSeconds& aTimeOffsetValue): CAknMfneSettingPage(aResourceID),iTimeOffsetValue(aTimeOffsetValue),iBackupTimeOffsetValue(iTimeOffsetValue)
{
}

CAknTimeOffsetSettingPage::CAknTimeOffsetSettingPage(const TDesC* aSettingTitleText,TInt aSettingNumber,TInt aControlType,TInt aEditorResourceId,TInt aSettingPageResourceId,TTimeIntervalSeconds& aTimeOffsetValue): CAknMfneSettingPage(aSettingTitleText,aSettingNumber,aControlType,aEditorResourceId,aSettingPageResourceId),iTimeOffsetValue(aTimeOffsetValue),iBackupTimeOffsetValue(iTimeOffsetValue)
{
}

void CAknTimeOffsetSettingPage::ConstructL(void)
{
  BaseConstructL();
  TimeOffsetEditor()->SetUpAndDownKeysConsumed(ETrue);
  TimeOffsetEditor()->SetTimeOffset(iTimeOffsetValue);
}

void CAknTimeOffsetSettingPage::UpdateSettingL(void)
{
  iTimeOffsetValue=TimeOffsetEditor()->TimeOffset();
  if(iSettingPageObserver) iSettingPageObserver->HandleSettingPageEventL(this,MAknSettingPageObserver::EEventSettingChanged);
}

void CAknTimeOffsetSettingPage::AcceptSettingL(void)
{
  TimeOffsetEditor()->PrepareForFocusLossL();
  iTimeOffsetValue=TimeOffsetEditor()->TimeOffset();
  if(iSettingPageObserver) iSettingPageObserver->HandleSettingPageEventL(this,MAknSettingPageObserver::EEventSettingOked);
}

void CAknTimeOffsetSettingPage::RestoreOriginalSettingL(void)
{
  iTimeOffsetValue=iBackupTimeOffsetValue;
  if(iSettingPageObserver) iSettingPageObserver->HandleSettingPageEventL(this,MAknSettingPageObserver::EEventSettingCancelled);
}

CEikTimeOffsetEditor* CAknTimeOffsetSettingPage::TimeOffsetEditor(void)
{
  return (CEikTimeOffsetEditor*)EditorControl();
}
