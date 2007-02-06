/*
    AlmBD.cpp
    Copyright (C) 2005 zg

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

#include "AlmUtils.hpp"
#include <cntdb.h> //CContactDatabase
#include <cntitem.h> //CContactItem
#include <cntfldst.h> //CContact*Field
#include <BtEng.hpp> //CBTMCMSettings
#include <txtetext.h> //CEditableText
#include <SharedData.hpp> //KSysAppUid

TDateTime AlmUtils::StripTime(TDateTime aDate)
{
  aDate.SetHour(0);
  aDate.SetMinute(0);
  aDate.SetSecond(0);
  aDate.SetMicroSecond(0);
  return aDate;
}

void AlmUtils::ProcessBirthdaysL(const TTimeIntervalDays aShift,TInt& aMsgCount)
{
  aMsgCount=0;
  _LIT(KFrom,"Calendar Server");
  const TLitC<16> KDescription=
  {
    15,
    {
      'B','i','r','t','h','d','a','y',' ','a','l','e','r','t',0x2026,0
    }
  };
  TBuf<30> to;
  TBuf<130> body;
  User::LeaveIfError(CBTMCMSettings::GetLocalBTName(to));
  TTime now;
  now.HomeTime();
  now=StripTime(now.DateTime());
  TInt year=now.DateTime().Year();

  CContactDatabase* db=CContactDatabase::OpenL();
  CleanupStack::PushL(db);
  TContactIter iter(*db);
  TContactItemId curItem;
  while((curItem=iter.NextL())!=KNullContactId)
  {
    CContactItem* item=db->OpenContactLX(curItem);
    CleanupStack::PushL(item);
    CContactItemFieldSet& fieldSet=item->CardFields();
    TInt bdIndex=fieldSet.Find(KUidContactFieldBirthday);
    if(bdIndex>=0&&fieldSet[bdIndex].StorageType()==KStorageTypeDateTime)
    {
      TDateTime bd=StripTime(fieldSet[bdIndex].DateTimeStorage()->Time().DateTime());
      User::LeaveIfError(bd.SetYearLeapCheck(year));
      TTime birthday(bd);
      if(birthday<now)
      {
        User::LeaveIfError(bd.SetYearLeapCheck(year+1));
        birthday=bd;
      }
      TTimeIntervalDays diff=birthday.DaysFrom(now);
      if(diff<=aShift&&diff>=TTimeIntervalDays(0))
      {
        body.Zero();
        body.Append(KDescription().Left(14));
        body.Append(':');
        body.Append(CEditableText::EParagraphDelimiter);
        TInt gnIndex=fieldSet.Find(KUidContactFieldGivenName);
        if(gnIndex>=0&&fieldSet[gnIndex].StorageType()==KStorageTypeText)
        {
          TPtrC name=fieldSet[gnIndex].TextStorage()->StandardTextLC();
          body.Append(name);
          CleanupStack::PopAndDestroy(); //StandardTextLC
          body.Append(' ');
        }
        TInt fnIndex=fieldSet.Find(KUidContactFieldFamilyName);
        if(fnIndex>=0&&fieldSet[fnIndex].StorageType()==KStorageTypeText)
        {
          TPtrC name=fieldSet[fnIndex].TextStorage()->StandardTextLC();
          body.Append(name);
          CleanupStack::PopAndDestroy(); //StandardTextLC
        }
        body.Append(CEditableText::EParagraphDelimiter);
        body.AppendNumFixedWidth(bd.Day()+1,EDecimal,2);
        body.Append('.');
        body.AppendNumFixedWidth(bd.Month()+1,EDecimal,2);
        body.Append('.');
        body.AppendNumFixedWidth(bd.Year(),EDecimal,4);
        if(!aMsgCount)
        {
          RSharedDataClient sysap;
          User::LeaveIfError(sysap.Connect(0));
          CleanupClosePushL(sysap);
          User::LeaveIfError(sysap.AssignToTemporaryFile(KSysAppUid));
          sysap.SetInt(KKeyToneQuit,1);
          CleanupStack::PopAndDestroy(); //sysap
        }
        SendMessageL(KFrom,to,KDescription,body);
        aMsgCount++;
      }
    }
    CleanupStack::PopAndDestroy(2); //item, db->CloseContactL();
  }
  CleanupStack::PopAndDestroy(); //db
}
