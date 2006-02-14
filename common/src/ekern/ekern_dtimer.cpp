/*
    ekern_dthread.cpp
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

EXPORT_C void DTimer::Cancel() //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void DTimer::Abort() //yes, this method do nothing
{
}

EXPORT_C void DTimer::AbortLocked() //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void DTimer::After(TRequestStatus &aStatus,TTimeIntervalMicroSeconds32 anInterval) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void DTimer::At(TRequestStatus &aStatus,const TTime &aTime) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void DTimer::Lock(TRequestStatus &aStatus, TTimerLockSpec aLock) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void DTimer::Inactivity(TRequestStatus& aStatus, TInt aSeconds) //FIXME: NOT IMPLEMENTED
{
}
