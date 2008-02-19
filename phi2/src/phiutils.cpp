/*
    phiutils.cpp
    Copyright (C) 2008 zg

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "phiutils.hpp"
#include <phi.rsg>
#include <aknnotewrappers.h>

void PhiUtils::FormatSize(TInt aSize,TDes& aResult)
{
  TInt unit=R_PHI_BYTE;
  while((aSize>=10000)&&(unit<R_PHI_MBYTE))
  {
    aSize/=1024;
    unit++;
  }
  TBuf<4> unitStr;
  CCoeEnv::Static()->ReadResourceAsDes16(unitStr,unit);
  _LIT(KSizeFormat,"%d%S");
  aResult.Format(KSizeFormat,aSize,&unitStr);
}

void PhiUtils::ShowMessageL(TInt aResourceID)
{
  TBuf<32> message;
  CCoeEnv::Static()->ReadResourceAsDes16(message,aResourceID);
  CAknInformationNote* dlg=new(ELeave)CAknInformationNote;
  dlg->SetTimeout(CAknNoteDialog::EShortTimeout);
  dlg->ExecuteLD(message);
}

TBool PhiUtils::ShowConfirmationL(TInt aResourceID)
{
  TBool res=EFalse;
  TBuf<32> message;
  CCoeEnv::Static()->ReadResourceAsDes16(message,aResourceID);
  CAknQueryDialog* dlg=CAknQueryDialog::NewL();
  if(dlg->ExecuteLD(R_PHI_CONFIRMATION_QUERY,message)==EAknSoftkeyYes) res=ETrue;
  return res;
}

void PhiUtils::ShowErrorL(TInt aResourceID,TInt anError)
{
  TBuf<32> message;
  CCoeEnv::Static()->ReadResourceAsDes16(message,aResourceID);
  TBuf<64> error;
  error.Format(message,anError);
  CAknErrorNote* dlg=new(ELeave)CAknErrorNote;
  dlg->SetTimeout(CAknNoteDialog::ENoTimeout);
  dlg->ExecuteLD(error);
}
