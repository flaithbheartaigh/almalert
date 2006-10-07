/*
    clockappdoc.cpp
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

#include "clockapp.hpp"
#include <s32file.h>

_LIT(KConfig,"c:\\system\\data\\CLOCKAPP.dat");

CClkDocument* CClkDocument::NewL(CEikApplication& aApp)
{
  CClkDocument* self=new(ELeave)CClkDocument(aApp);
  CleanupStack::PushL(self);
  self->ConstructL();
  CleanupStack::Pop(); //self
  return self;
}

CClkDocument::~CClkDocument()
{
  delete iAlm;
  delete iNitz;
}

void CClkDocument::StoreDataL(void)
{
  RFileWriteStream stream;
  CleanupReleasePushL(stream);
  User::LeaveIfError(stream.Replace(Process()->FsSession(),KConfig,EFileShareExclusive));
  stream.WriteUint32L(iDate.High());
  stream.WriteUint32L(iDate.Low());
  stream.CommitL();
  CleanupStack::PopAndDestroy();
}

void CClkDocument::RestoreDataL(void)
{
  RFileReadStream stream;
  CleanupReleasePushL(stream);
  User::LeaveIfError(stream.Open(Process()->FsSession(),KConfig,EFileShareReadersOnly));
  TUint32 high;
  high=stream.ReadUint32L();
  iDate.Set(high,stream.ReadUint32L());
  CleanupStack::PopAndDestroy();
}

TTime CClkDocument::GetLastAlarmTime(void)
{
  return iDate;
}

void CClkDocument::SetLastAlarmTime(TTime& anAlarm)
{
  iDate=anAlarm.Int64();
}

CClkDocument::CClkDocument(CEikApplication& aApp): CAknDocument(aApp)
{
}

void CClkDocument::ConstructL(void)
{
  iAlm=CClkAlmModel::NewL(NULL,-5);
  iNitz=CClkNitzModel::NewL(NULL,-3);
  TRAPD(err,RestoreDataL());
  if(err==KErrNotFound)
  {
    TTime time;
    time.HomeTime();
    TDateTime dt=time.DateTime();
    dt.SetHour(8);
    dt.SetMinute(0);
    dt.SetSecond(0);
    time=dt;
    iDate=time.Int64();
  }
  TRAPD(err2,StoreDataL());
}

CEikAppUi* CClkDocument::CreateAppUiL()
{
  CEikAppUi* appUi=new(ELeave)CClkAppUi;
  return appUi;
}
