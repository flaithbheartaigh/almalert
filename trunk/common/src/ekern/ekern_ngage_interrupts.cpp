/*
    ekern_ngage_interrupts.cpp
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

#include <a32ngage.hpp>

//163 irq
//160 irq
//161 irq
//164 irq
//162 irq

EXPORT_C void TNgage::EnableIrq(TInt anId)
{
  if(anId<32)
  {
    SetRegister32(1<<anId,0x120108);
  }
  else if(anId<64)
  {
    SetRegister32(1<<(anId-32),0x120148);
  }
  else
  {
    SetRegister32(1<<(anId-64),0x120188);
  }
}

EXPORT_C void TNgage::DisableIrq(TInt anId)
{
  if(anId<32)
  {
    SetRegister32(1<<anId,0x12010c);
  }
  else if(anId<64)
  {
    SetRegister32(1<<(anId-32),0x12014c);
  }
  else
  {
    SetRegister32(1<<(anId-64),0x12018c);
  }
}

//130 fiq
//131 fiq
//129 fiq

EXPORT_C void TNgage::EnableFiq(TInt anId)
{
  if(anId<32)
  {
    SetRegister32(1<<anId,0x1201c8);
  }
  else if(anId<64)
  {
    SetRegister32(1<<(anId-32),0x120208);
  }
  else
  {
    SetRegister32(1<<(anId-64),0x120248);
  }
}

EXPORT_C void TNgage::DisableFiq(TInt anId)
{
  if(anId<32)
  {
    SetRegister32(1<<anId,0x1201cc);
  }
  else if(anId<64)
  {
    SetRegister32(1<<(anId-32),0x12020c);
  }
  else
  {
    SetRegister32(1<<(anId-64),0x12024c);
  }
}
