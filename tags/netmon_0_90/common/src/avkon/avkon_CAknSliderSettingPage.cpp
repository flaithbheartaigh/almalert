/*
    avkon_CAknSliderSettingPage.cpp
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

#include <aknslidersettingpage.h>

EXPORT_C CAknSliderSettingPage::CAknSliderSettingPage(TInt aResourceID,TInt& aSliderValue): CAknSettingPage(aResourceID),iSliderValue(aSliderValue),iBackupSliderValue(aSliderValue)
{
}

EXPORT_C CAknSliderSettingPage::CAknSliderSettingPage(const TDesC* aSettingTitleText,TInt aSettingNumber,TInt aControlType,TInt aEditorResourceId,TInt aSettingPageResourceId,TInt& aSliderValue): CAknSettingPage(aSettingTitleText,aSettingNumber,aControlType,aEditorResourceId,aSettingPageResourceId),iSliderValue(aSliderValue),iBackupSliderValue(aSliderValue)
{
}

EXPORT_C CAknSliderSettingPage::~CAknSliderSettingPage()
{
}

EXPORT_C void CAknSliderSettingPage::ConstructL(void)
{
  BaseConstructL();
  SliderControl()->SetValueL(iSliderValue);
}

EXPORT_C void CAknSliderSettingPage::UpdateSettingL()
{
  iSliderValue=SliderControl()->Value();
  if(iSettingPageObserver) iSettingPageObserver->HandleSettingPageEventL(this,MAknSettingPageObserver::EEventSettingChanged);
}

EXPORT_C void CAknSliderSettingPage::AcceptSettingL(void)
{
  iSliderValue=SliderControl()->Value();
  if(iSettingPageObserver) iSettingPageObserver->HandleSettingPageEventL(this,MAknSettingPageObserver::EEventSettingChanged);
  //BUG: must be
  //if(iSettingPageObserver) iSettingPageObserver->HandleSettingPageEventL(this,MAknSettingPageObserver::EEventSettingOked);
}

EXPORT_C void CAknSliderSettingPage::RestoreOriginalSettingL(void)
{
  iSliderValue=iBackupSliderValue;
  //BUG: lost
  //if(iSettingPageObserver) iSettingPageObserver->HandleSettingPageEventL(this,MAknSettingPageObserver::EEventSettingCancelled);
}

EXPORT_C CAknSlider* CAknSliderSettingPage::SliderControl(void)
{
  return (CAknSlider*)EditorControl();
}

EXPORT_C void CAknSliderSettingPage::SizeChanged(void)
{
  StandardSettingPageLayout();
  AknLayoutUtils::LayoutControl(EditorControl(),Rect(),TAknWindowLineLayout::TAknWindowLineLayout(0x127));
}

EXPORT_C void CAknSliderSettingPage::Draw(const TRect &aRect) const
{
  BaseDraw(aRect);
}

EXPORT_C void CAknSliderSettingPage::WriteInternalStateL(RWriteStream& aWriteStream) const
{
}

EXPORT_C void CAknSliderSettingPage::Reserved_2(void)
{
}

EXPORT_C void CAknSliderSettingPage::CAknSettingPage_Reserved_1(void)
{
}

EXPORT_C void CAknSliderSettingPage::CAknSettingPage_Reserved_2(void)
{
}
