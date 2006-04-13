/*
    ekern_dchangenotifier.cpp
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

class TProcessMemoryInfo; //don't exists in symbian 6.1
#include <k32std.h>
#include <e32std.h>

DChangeNotifier::DChangeNotifier(): iChanges(0x7f)
{
}

DChangeNotifier::~DChangeNotifier()
{
  if(iStatus) iThread->RequestComplete(iStatus,KErrGeneral);
}

EXPORT_C void DChangeNotifier::Notify(TUint aChanges)
{
  aChanges|=iChanges;
  iChanges=aChanges;
  if(iStatus)
  {
    iChanges=0;
    iThread->RequestComplete(iStatus,aChanges);
  }
}

EXPORT_C TInt DChangeNotifier::Logon(TRequestStatus& aStatus,const DThread& aThread)
{
  if(iStatus) return KErrInUse;
  aStatus=KRequestPending;
  iThread=&aThread;
  iStatus=&aStatus;
  if(iChanges!=0)
  {
    iThread->RequestComplete(iStatus,KErrNone);
    iChanges=0;
  }
  return KErrNone;
}

EXPORT_C TInt DChangeNotifier::LogonCancel(const DThread& aThread)
{
  if(!iStatus||iThread!=&aThread) return KErrGeneral;
  iThread->RequestComplete(iStatus,KErrCancel);
  iChanges=0;
  return KErrNone;
}
