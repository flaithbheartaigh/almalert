/*
    ekern_static.cpp
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
#include <m32std.h>

Custom* A::TheVariant; //0x8000038C

CObjectCon* K::Threads; //0x8000039C
CObjectCon* K::Chunks; //0x800003A0
CObjectCon* K::Timers; //0x800003C0

DProcess* K::SvProcess; //0x800003DC
DThread* K::SvThread; //0x800003E0

TUint K::TickCounter; //0x80000488
TUint K::RtcTickRef; //0x8000048C
TUint K::TickCountRef; //0x80000490

TInt ImpMmu::PageSizeInBytes; //0x8000673C

TDfcQue K::DfcQ; //0x80006BB8
DPowerModel* K::PowerModel; //0x80006BC4
RHeapK* K::Heap; //0x80006C34
