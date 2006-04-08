/*
    ekern_ticklink.cpp
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

EXPORT_C TTickLink::TTickLink() //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void TTickLink::PeriodicInMicroSeconds(TTimeIntervalMicroSeconds32 aPeriod,TTickCallBack aCallBack,TAny* aPtr,TBool aRegulated) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void TTickLink::OneShotInMicroSeconds(TTimeIntervalMicroSeconds32 aTime,TTickCallBack aCallBack,TAny* aPtr) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void TTickLink::Lock(TInt aTicks,TTickCallBack aCallBack,TAny* aPtr) //FIXME: NOT IMPLEMENTED
{
}

TInt TTickLink::GetNextLock(TTimerLockSpec aMark, TInt &aTickCount) const //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C void TTickLink::Cancel(void) //FIXME: NOT IMPLEMENTED
{
}
