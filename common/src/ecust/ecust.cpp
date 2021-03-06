/*
    ecust.cpp
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

#include <v32ngage.hpp>

TBool Variant::TestBootSequence(void)
{
  return EFalse;
}

void Variant::Init1(void)
{
}

void Variant::Init2(void)
{
}

void Variant::Init3(void) //FIXME: NOT IMPLEMENTED
{
}

void Variant::Init4(void) //FIXME: NOT IMPLEMENTED
{
}

void Variant::Init5(void) //FIXME: NOT IMPLEMENTED
{
}

TBool Variant::SetInterface(Custom::TInterfaces anId,TAny* anInterface)
{
  if(anId<Custom::ECount&&anId>=Custom::ECBus)
  {
    if(!iInterfaces[anId])
    {
      iInterfaces[anId]=anInterface;
      return ETrue;
    }
  }
  return EFalse;
}

TAny* Variant::Interface(Custom::TInterfaces anId)
{
  if(anId<Custom::ECount&&anId>=Custom::ECBus) return iInterfaces[anId];
  else return NULL;
}
