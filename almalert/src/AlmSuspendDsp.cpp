/*
    AlmSuspendDsp.cpp
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

#include "AlmSuspendDsp.hpp"

_LIT(KAudioServer,"AudioServer");

TInt RSuspendDsp::Open(void)
{
  if(iSuspended) return KErrInUse;
  TVersion version(0,0,0);
  TInt err=CreateSession(KAudioServer,version,2);
  if(KErrNone==err)
  {
    err=SendReceive(0x3f,NULL);
    if(KErrNone==err) iSuspended=ETrue;
    else RSessionBase::Close();
  }
  return err;
}

void RSuspendDsp::Close(void)
{
  if(iSuspended)
  {
    iSuspended=EFalse;
    SendReceive(0x40,NULL);
    RSessionBase::Close();
  }
}
