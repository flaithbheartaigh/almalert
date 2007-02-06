/*
    ekern_sup.cpp
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

void S::RealTimeThreadCreateL(TInt& aHandle,const TDesC& aName,TThreadFunction aFunction,TInt aStackSize,TInt aHeapSize,TAny* aPtr,DThread*& aThread) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C TInt Sup::RealTimeThreadCreate(TInt& aHandle,const TDesC& aName,TThreadFunction aFunction,TInt aStackSize,TInt aHeapSize,TAny* aPtr)
{
  DThread* thread=NULL;
  TRAPD(err,S::RealTimeThreadCreateL(aHandle,aName,aFunction,aStackSize,aHeapSize,aPtr,thread));
  if(err!=KErrNone) S::CheckedClose(thread);
  return err;
}
