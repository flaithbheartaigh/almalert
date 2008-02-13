/*
    langswdll.cpp
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

#include "langswdll.hpp"
#include <SharedData.hpp>

GLDEF_C TInt E32Dll(TDllReason/* aReason*/)
{
  return KErrNone;
}

EXPORT_C CAnimDll* CreateCAnimDllL()
{
  return new(ELeave)CLangDll;
}

CAnim* CLangDll::CreateInstanceL(TInt/* aType*/)
{
  return new(ELeave)CLang;
}

CLang::~CLang()
{
  iFunctions->GetRawEvents(EFalse);
  iSettings.Close();
}

TInt CLang::CommandReplyL(TInt aOpcode,TAny* aArgs)
{
  return KErrNone;
}

void CLang::Command(TInt aOpcode,TAny* aArgs)
{
}

void CLang::Animate(TDateTime* aDateTime)
{
}

TBool CLang::OfferRawEvent(const TRawEvent& aRawEvent)
{
  if(aRawEvent.Type()==TRawEvent::EKeyDown&&aRawEvent.ScanCode()==EStdKeyLeftShift) iShift=ETrue;
  if(aRawEvent.Type()==TRawEvent::EKeyUp&&aRawEvent.ScanCode()==EStdKeyLeftShift) iShift=EFalse;
  if(aRawEvent.Type()==TRawEvent::EKeyDown&&iShift&&aRawEvent.ScanCode()==EStdKeyBackspace)
  {
    iKey=ETrue;
    return ETrue;
  }
  if(aRawEvent.Type()==TRawEvent::EKeyUp&&iKey&&aRawEvent.ScanCode()==EStdKeyBackspace)
  {
    iKey=EFalse;
    TInt lang;
    iSettings.GetInt(KKeyInputLang,lang);
    iSettings.SetInt(KKeyInputLang,(lang==ELangEnglish?ELangRussian:ELangEnglish));
    return ETrue;
  }
  return EFalse;
}

void CLang::ConstructL(TAny* aArgs,TBool aHasFocus)
{
  User::LeaveIfError(iSettings.Connect(0));
  User::LeaveIfError(iSettings.Assign(KSettingsUid));
  iFunctions->GetRawEvents(ETrue);
}

void CLang::Redraw(void)
{
}

void CLang::FocusChanged(TBool aState)
{
}
