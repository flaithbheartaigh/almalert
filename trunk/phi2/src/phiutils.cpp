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
#include <phien.rsg>

void PhiUtils::FormatSize(TInt aSize,TDes& aResult)
{
  TInt unit=R_PHI_BYTE,mod=0;
  while((aSize>=10000)&&(unit<R_PHI_MBYTE))
  {
    mod=(aSize%1024)*100/1024;
    aSize/=1024;
    unit++;
  }
  TBuf<4> unitStr;
  CCoeEnv::Static()->ReadResourceAsDes16(unitStr,unit);
  _LIT(KSizeFormat1,"%d.%02d%S");
  _LIT(KSizeFormat2,"%d%S");
  if(mod) aResult.Format(KSizeFormat1,aSize,mod,&unitStr);
  else aResult.Format(KSizeFormat2,aSize,&unitStr);
}

void PhiUtils::ShowMessageL(TInt aResourceID)
{
  TBuf<32> message;
  CCoeEnv::Static()->ReadResourceAsDes16(message,aResourceID);
  ShowMessageL(message,CAknNoteDialog::EShortTimeout);
}

void PhiUtils::ShowMessageL(const TDesC& aMessage,CAknNoteDialog::TTimeout aTimeout)
{
  CAknInformationNote* dlg=new(ELeave)CAknInformationNote;
  dlg->SetTimeout(aTimeout);
  dlg->ExecuteLD(aMessage);
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
