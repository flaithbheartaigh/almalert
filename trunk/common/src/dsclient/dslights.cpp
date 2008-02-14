/*
    dslights.cpp
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

#include <DosClient.hpp>

TInt RDosLights::Open(RDosServer& aSession)
{
  const TAny* p[KMaxMessageArguments];
  p[2]=(TAny*)1;
  return CreateSubSession(aSession,0x68,p);
}

TInt RDosLights::SwitchLights(TInt aType,TInt aState) const
{
  const TAny* p[KMaxMessageArguments];
  p[0]=(TAny*)aType;
  p[1]=(TAny*)aState;
  p[2]=(TAny*)1;
  return SendReceive(0x258,p);
}
