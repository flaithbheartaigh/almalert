/*
    phiconst.hpp
    Copyright (C) 2007-2008 zg

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __PHICONST_HPP__
#define __PHICONST_HPP__

#include <e32std.h>

const TInt KPhiStatusHeight=12; //(height=12)
const TInt KPhiItemHeight=16; //(height=12)+KPhiItemVMargin*4
const TInt KPhiItemVMargin=1;
const TInt KPhiItemHMargin=2;
const TInt KPhiItemBaseLine=10; //(ascent=10)
const TInt KPhiSortAreaWidth=12;

const TInt KPhiColorShadow=0xde;
const TInt KPhiColorSelection=0x23;
const TInt KPhiColorExe=0xc5;
const TInt KPhiColorImage=0xd4;
const TInt KPhiColorHidden=0xdd;

#define sizeofa(array) (sizeof(array)/sizeof(array[0]))

#endif
