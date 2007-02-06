/*
    lcdpartialmode.hpp
    Copyright (C) 2005 zg

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

#ifndef __LCDPARTIALMODE_HPP__
#define __LCDPARTIALMODE_HPP__

#include <e32base.h>

class CLcdPartialMode: public CBase
{
  public:
    IMPORT_C static CLcdPartialMode* NewL(void);
    IMPORT_C ~CLcdPartialMode();
  public:
    IMPORT_C void ExitPartialMode(void);
    IMPORT_C TInt PartialModeActivated(void);
    /*
     * aTo must be greater then aFrom.
     * possible values: from 1 to 208.
     * aFrom and aTo lines includes into visible area.
     */
    IMPORT_C void SetPartialModeActiveArea(TUint8 aFrom,TUint8 aTo);
  private:
    void ConstructL(void);
  private:
    class CLcdDev* iLcdDev;
};

#endif
