/*
    ekern_ngage.cpp
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

__declspec(naked) EXPORT_C TUint32 TNgage::Register32(TUint anAddr)
{
  __asm("mov\tr2, #0x58000000");
  __asm("ldr\tr0, [r2, r0]");
  __asm("mov\tpc, lr");
}

__declspec(naked) EXPORT_C void TNgage::SetRegister32(TUint32 aValue,TUint anAddr)
{
  __asm("mov\tr2, #0x58000000");
  __asm("str\tr0, [r2, r1]");
  __asm("mov\tpc, lr");
}

__declspec(naked) EXPORT_C void TNgage::ModifyRegister32(TUint anAddr,TUint32 aSetMask,TUint32 aClearMask)
{
  __asm("stmfd\tsp!, {r4}");
  __asm("mrs\tr3, cpsr");
  __asm("orr\tr4, r3, #192");
  __asm("msr\tcpsr_all, r4");
  __asm("mov\tr4, #0x58000000");
  __asm("add\tr4, r4, r0");
  __asm("ldr\tr0, [r4]");
  __asm("bic\tr0, r0, r2");
  __asm("orr\tr0, r0, r1");
  __asm("str\tr0, [r4]");
  __asm("msr\tcpsr_all, r3");
  __asm("ldmfd\tsp!, {r4}");
  __asm("mov\tpc, lr");
}

__declspec(naked) EXPORT_C TUint16 TNgage::Register16(TUint anAddr)
{
  __asm("mov\tr2, #0x58000000");
  __asm("ldrh\tr0, [r2, r0]");
  __asm("mov\tpc, lr");
}

__declspec(naked) EXPORT_C void TNgage::SetRegister16(TUint16 aValue,TUint anAddr)
{
  __asm("mov\tr2, #0x58000000");
  __asm("strh\tr0, [r2, r1]");
  __asm("mov\tpc, lr");
}

__declspec(naked) EXPORT_C void TNgage::ModifyRegister16(TUint anAddr,TUint16 aSetMask,TUint16 aClearMask)
{
  __asm("stmfd\tsp!, {r4}");
  __asm("mrs\tr3, cpsr");
  __asm("orr\tr4, r3, #192");
  __asm("msr\tcpsr_all, r4");
  __asm("mov\tr4, #0x58000000");
  __asm("add\tr4, r4, r0");
  __asm("ldrh\tr0, [r4]");
  __asm("bic\tr0, r0, r2");
  __asm("orr\tr0, r0, r1");
  __asm("strh\tr0, [r4]");
  __asm("msr\tcpsr_all, r3");
  __asm("ldmfd\tsp!, {r4}");
  __asm("mov\tpc, lr");
}
