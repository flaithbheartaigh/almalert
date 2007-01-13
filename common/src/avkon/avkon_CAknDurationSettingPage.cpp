/*
    avkon_CAknDurationSettingPage.cpp
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

#include <aknmfnesettingpage.h>

EXPORT_C CAknDurationSettingPage::~CAknDurationSettingPage()
{
}

EXPORT_C CAknDurationSettingPage::CAknDurationSettingPage(TInt aResourceID,TTimeIntervalSeconds& aDurationValue): CAknMfneSettingPage(aResourceID),iDurationValue(aDurationValue),iBackupDurationValue(iDurationValue)
{
}

EXPORT_C CAknDurationSettingPage::CAknDurationSettingPage(const TDesC* aSettingTitleText,TInt aSettingNumber,TInt aControlType,TInt aEditorResourceId,TInt aSettingPageResourceId,TTimeIntervalSeconds& aDurationValue): CAknMfneSettingPage(aSettingTitleText,aSettingNumber,aControlType,aEditorResourceId,aSettingPageResourceId),iDurationValue(aDurationValue),iBackupDurationValue(iDurationValue)
{
}

EXPORT_C void CAknDurationSettingPage::ConstructL(void)
{
  BaseConstructL();
  DurationEditor()->SetUpAndDownKeysConsumed(ETrue);
  DurationEditor()->SetDuration(iDurationValue);
}

EXPORT_C void CAknDurationSettingPage::UpdateSettingL(void)
{
  iDurationValue=DurationEditor()->Duration();
  if(iSettingPageObserver) iSettingPageObserver->HandleSettingPageEventL(this,MAknSettingPageObserver::EEventSettingChanged);
}

EXPORT_C void CAknDurationSettingPage::AcceptSettingL(void)
{
  DurationEditor()->PrepareForFocusLossL();
  iDurationValue=DurationEditor()->Duration();
  if(iSettingPageObserver) iSettingPageObserver->HandleSettingPageEventL(this,MAknSettingPageObserver::EEventSettingOked);
}

EXPORT_C void CAknDurationSettingPage::RestoreOriginalSettingL(void)
{
  iDurationValue=iBackupDurationValue;
  if(iSettingPageObserver) iSettingPageObserver->HandleSettingPageEventL(this,MAknSettingPageObserver::EEventSettingCancelled);
}

EXPORT_C CEikDurationEditor* CAknDurationSettingPage::DurationEditor(void)
{
  return (CEikDurationEditor*)EditorControl();
}

EXPORT_C void CAknDurationSettingPage::WriteInternalStateL(RWriteStream& aWriteStream) const
{
}

EXPORT_C void CAknDurationSettingPage::Reserved_2(void)
{
}

EXPORT_C void CAknDurationSettingPage::CAknSettingPage_Reserved_1(void)
{
}

EXPORT_C void CAknDurationSettingPage::CAknSettingPage_Reserved_2(void)
{
}
