/*
    ekern_ngage_timers.cpp
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

EXPORT_C TUint32 TNgage::TimerMode(TUint anId)
{
  return TNgage::Register32(anId+0x120a04);
}

EXPORT_C void TNgage::SetTimerMode(TUint anId,TUint32 aValue)
{
  TNgage::SetRegister32(aValue,anId+0x120a04);
  for(TUint i=0;i<5;i++)
  {
    TNgage::SetRegister32(aValue,anId+0x120a04);
    TNgage::TimerMode(anId);
  }
}

EXPORT_C void TNgage::SetTimerPreScaler(TUint anId,TUint32 aValue)
{
  TNgage::SetRegister32(aValue,anId+0x120a0c);
}

EXPORT_C TUint32 TNgage::TimerPostScaler(TUint anId)
{
  return TNgage::Register32(anId+0x120a18);
}

EXPORT_C void TNgage::SetTimerPostScaler(TUint anId,TUint32 aValue)
{
  TNgage::SetRegister32(aValue,anId+0x120a18);
}

EXPORT_C TUint32 TNgage::TimerCounter(TUint anId)
{
  return TNgage::Register32(anId+0x120a10);
}

EXPORT_C void TNgage::SetTimerCounter(TUint anId,TUint32 aValue)
{
  TNgage::SetRegister32(aValue,anId+0x120a10);
}

//417

EXPORT_C void TNgage::SetTimerServed(TUint anId,TUint32 aValue)
{
  TNgage::SetRegister32(aValue,anId+0x120a08);
}

EXPORT_C TUint32 TNgage::TimerServed(TUint anId)
{
  return TNgage::Register32(anId+0x120a08);
}
