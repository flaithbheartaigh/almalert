/*
    phientry.hpp
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

#ifndef __PHIENTRY_HPP__
#define __PHIENTRY_HPP__

#include <e32std.h>

class TPhiEntry
{
  public:
    TPhiEntry();
    void Clear(void);
    TBool IsDir(void) const;
    TBool IsHidden(void) const;
    TBool IsVolume(void) const;
    TBool IsApp(void) const;
    TBool IsExe(void) const;
    TBool IsImage(void) const;
  public:
    TUint iAtt;
    TInt iSize;
    TTime iModified;
    TFileName iName;
};

#endif
