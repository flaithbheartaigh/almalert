/*
    avkon_CAknTextSettingPage.cpp
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

#include <akntextsettingpage.h>

EXPORT_C CAknTextSettingPage::CAknTextSettingPage(TInt aResourceID,TDes& aText,TInt aTextSettingPageFlags): CAknEdwinSettingPage(aResourceID),iText(aText),iTextSettingPageFlags(aTextSettingPageFlags)
{
}

EXPORT_C CAknTextSettingPage::CAknTextSettingPage(const TDesC* aSettingTitleText,TInt aSettingNumber,TInt aControlType,TInt aEditorResourceId,TInt aSettingPageResourceId,TDes& aText,TInt aTextSettingPageFlags): CAknEdwinSettingPage(aSettingTitleText,aSettingNumber,aControlType,aEditorResourceId,aSettingPageResourceId),iText(aText),iTextSettingPageFlags(aTextSettingPageFlags)
{
}

EXPORT_C CAknTextSettingPage::~CAknTextSettingPage()
{
  delete iBackupText;
}

EXPORT_C void CAknTextSettingPage::ConstructL(void)
{
  BaseConstructL();
  iBackupText=iText.AllocL();
  CEikEdwin* edwin=TextControl();
  if(!(iTextSettingPageFlags&EPredictiveTextEntryPermitted))
  {
    edwin->SetAknEditorFlags(edwin->AknEdwinFlags()|EAknEditorFlagNoT9);
  }
  TInt max=edwin->MaxLength(),limit=iText.MaxLength();
  if(max&&limit>max) limit=max;
  edwin->SetTextLimit(limit);
  edwin->SetTextL(&iText);
  CheckAndSetDataValidity();
  UpdateCbaL();
}

EXPORT_C void CAknTextSettingPage::HandleControlEventL(CCoeControl* aControl,TCoeEvent aEventType)
{
  if(aEventType==EEventStateChanged)
  {
    if(!iUpdateMode) UpdateSettingL();
    CheckAndSetDataValidity();
    UpdateCbaL();
  }
}

EXPORT_C void CAknTextSettingPage::DynamicInitL(void)
{
  UpdateCbaL();
}

EXPORT_C void CAknTextSettingPage::UpdateSettingL(void)
{
  UpdateTextL();
  if(iSettingPageObserver) iSettingPageObserver->HandleSettingPageEventL(this,MAknSettingPageObserver::EEventSettingChanged);
}

EXPORT_C void CAknTextSettingPage::UpdateTextL(void)
{
  HBufC* text=TextControl()->GetTextInHBufL();
  if(text)
  {
    iText.Copy(text->Left(iText.MaxLength()));
    delete text;
  }
  else
  {
    iText.SetLength(0);
  }
}

EXPORT_C void CAknTextSettingPage::AcceptSettingL(void)
{
  UpdateTextL();
  if(iSettingPageObserver) iSettingPageObserver->HandleSettingPageEventL(this,MAknSettingPageObserver::EEventSettingOked);
}

EXPORT_C void CAknTextSettingPage::RestoreOriginalSettingL(void)
{
  iText.Copy(*iBackupText);
  if(iSettingPageObserver) iSettingPageObserver->HandleSettingPageEventL(this,MAknSettingPageObserver::EEventSettingCancelled);
}

EXPORT_C void CAknTextSettingPage::UpdateCbaL(void)
{
  if(!DataValidity())
  {
    Cba()->SetCommandSetL(InvalidDataCbaResourceId());
  }
  else
  {
    Cba()->SetCommandSetL(DefaultCbaResourceId());
  }
  Cba()->DrawDeferred();
}

EXPORT_C void CAknTextSettingPage::SizeChanged(void) //ok
{
  StandardSettingPageLayout();
  iEdwinLayoutRect.LayoutRect(Rect(),TAknWindowLineLayout(0x128));
  TRect rect=iEdwinLayoutRect.Rect();
  iHorizontalShadow.LayoutRect(rect,TAknWindowLineLayout(0x13d));
  iVerticalShadow.LayoutRect(rect,TAknWindowLineLayout(0x13e));
  iOutlineFrame.LayoutRect(rect,TAknWindowLineLayout(0x13f));
  AknLayoutUtils::LayoutEdwin(TextControl(),rect,TAknMultiLineTextLayout(0x141,0,0,0,0,0,0,0,6));
}

EXPORT_C void CAknTextSettingPage::CheckAndSetDataValidity(void)
{
  SetDataValidity(ETrue);
  if(!(iTextSettingPageFlags&EZeroLengthAllowed))
  {
    CEikEdwin* edwin=TextControl();
    if(edwin)
    {
      TPtrC ptr=edwin->Text()->Read(0);
      SetDataValidity(AknTextUtils::IsEmptyText(ptr));
    }
    else SetDataValidity(EFalse);
  }
}

EXPORT_C void CAknTextSettingPage::WriteInternalStateL(RWriteStream& aWriteStream) const
{
}

EXPORT_C void CAknTextSettingPage::Reserved_2(void)
{
}

EXPORT_C void CAknTextSettingPage::CAknSettingPage_Reserved_1(void)
{
}

EXPORT_C void CAknTextSettingPage::CAknSettingPage_Reserved_2(void)
{
}

EXPORT_C void CAknTextSettingPage::CAknEdwinSettingPage_Reserved_1(void)
{
}
