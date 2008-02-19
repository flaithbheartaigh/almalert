/*
    phiselection.hpp
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

#ifndef __PHISELECTION_HPP__
#define __PHISELECTION_HPP__

#include <badesca.h>

class CPhiSelection: public CBase
{
  public:
    enum TType
    {
      ENone,
      ECopy,
      EMove,
      EDelete,
      EAttr
    };
  public:
    static CPhiSelection* NewLC(void);
    ~CPhiSelection();
    const MDesCArray* Array(void) const;
    const TDesC& Folder(void) const;
    TType Type(void) const;
  private:
    void ConstructL(void);
  private:
    TType iType;
    TFileName iFolder;
    TUint iSetMask;
    TUint iClearMask;
    CDesCArrayFlat* iFiles;
  friend class CPhiListBox;
};

#endif
