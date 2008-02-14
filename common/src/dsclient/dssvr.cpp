/*
    dssvr.cpp
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

_LIT(KServerName,"DosServer");

void RDosServer::Close(void)
{
  RHandleBase::Close();
}

TInt RDosServer::Connect(void)
{
  TVersion ver(1,0,0);
  return CreateSession(KServerName,ver,-1);
}

//on new phones - different implementation
/*void RDosSubSession::Close(void)
{
  CloseSubSession(0x6e);
}*/
