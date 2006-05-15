/*
    ekern_mstimer.cpp
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

#include <a32mstimer.hpp>

void TMsTimer::Init(void)
{
  if(Bind(_L8("IrqTimer1"))!=KErrNone) User::Invariant();
}

void TMsTimer::On(void) //FIXME: NOT IMPLEMENTED
{
  //
  Enable();
}

void TMsTimer::Off(void) //FIXME: NOT IMPLEMENTED
{
  //
  Disable();
}

void TMsTimer::Service(void) //FIXME: NOT IMPLEMENTED
{
}

TInt TMsTimer::Count(void) //FIXME: NOT IMPLEMENTED
{
  return 0;
}

void TMsTimer::SetCount(TInt aValue) //FIXME: NOT IMPLEMENTED
{
}

void TMsTimer::AddToCount(TInt aValue) //FIXME: NOT IMPLEMENTED
{
}

TInt TMsTimer::CountFromMilliSeconds(TInt aValue) //FIXME: NOT IMPLEMENTED
{
  return aValue*26000;
}
