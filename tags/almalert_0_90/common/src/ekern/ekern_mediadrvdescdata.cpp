/*
    ekern_mediadrvdescdata.cpp
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

EXPORT_C TMediaDrvDescData::TMediaDrvDescData(): iPtr(NULL),iHandle(0),iOffset(0),iThread(NULL)
{
}

EXPORT_C TMediaDrvDescData::TMediaDrvDescData(TLocalDriveMessageData aData) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C TBool TMediaDrvDescData::IsCurrentThread(void)
{
  return (iHandle==KCurrentThreadHandle)?ETrue:EFalse;
}

EXPORT_C TInt TMediaDrvDescData::CurrentThreadDescCheck(TInt aLen)
{
  if(iHandle==KCurrentThreadHandle)
  {
    TUint32* ptr=(TUint32*)iPtr;
    TInt size=ptr[1];
    TInt pos=iOffset+aLen;
    if(size>=pos) return KErrNone;
  }
  return KErrBadDescriptor;
}
