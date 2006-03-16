/*
    ekern_dprocess.cpp
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

EXPORT_C TInt DProcess::Rename(const TDesC &aName) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C void DProcess::FileName(TDes& aFileName) const
{
  aFileName.Copy(*iFileName);
}

EXPORT_C void DProcess::CommandLine(TDes &aCommand) const //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C TProcessPriority DProcess::Priority() const
{
  return iPriority;
}

EXPORT_C void DProcess::SetPriority(TProcessPriority aPriority) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C TBool DProcess::System() const
{
  return iFlags&4;
}

EXPORT_C void DProcess::SetSystem(TBool aState)
{
  iFlags&=~4;
  if(aState) iFlags|=4;
}

EXPORT_C TBool DProcess::Protected() const
{
  return iFlags&8;
}

EXPORT_C void DProcess::SetProtected(TBool aState)
{
  iFlags&=~8;
  if(aState) iFlags|=8;
}

EXPORT_C TInt DProcess::MakeHandleToOwningProcess(TInt& aHandle,DProcess* anOwningProcess) const //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C void DProcess::SetOwningProcess(TInt aHandle)
{
  if(!aHandle) iOwningProcess=NULL;
  else iOwningProcess=Kern::ProcessFromHandle(aHandle);
}

EXPORT_C void DProcess::Logon(TRequestStatus* aStatus,DThread* anOwningThread) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C TInt DProcess::LogonCancel(TRequestStatus* aStatus,DThread* anOwningThread) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}
