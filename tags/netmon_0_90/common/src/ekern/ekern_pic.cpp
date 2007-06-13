/*
    ekern_pic.cpp
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
#include <m32std.h>


void Pic::Init1()
{
  ImpPic::Init1();
}

void Pic::Init3()
{
  ImpPic::Init3();
}

void Pic::Dispatch(TInt anID) //FIXME: NOT IMPLEMENTED
{
}

TInt Pic::Bind(TInterrupt& anInterrupt, const TDesC8& aName) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

void Pic::UnBind(TInterrupt &anInterrupt) //FIXME: NOT IMPLEMENTED
{
}

void Pic::CheckedDisable(TInt anID) //FIXME: NOT IMPLEMENTED
{
}

void Pic::CheckedEnable(TInt anID) //FIXME: NOT IMPLEMENTED
{
}

void Pic::Disable(TInt anID) //FIXME: NOT IMPLEMENTED
{
}

void Pic::Enable(TInt anID) //FIXME: NOT IMPLEMENTED
{
}

void Pic::Clear(TInt anID) //FIXME: NOT IMPLEMENTED
{
}