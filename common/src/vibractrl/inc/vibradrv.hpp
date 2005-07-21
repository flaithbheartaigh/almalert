/*
    vibradrv.hpp
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

#ifndef __VIBRADRV_HPP__
#define __VIBRADRV_HPP__

#include <e32base.h>
#include <phonet.hpp>
#include <zg_vibra.hpp>

class CVibraDriver: public CBase
{
  public:
    static CVibraDriver* NewL(void);
    ~CVibraDriver();
    TInt State(void);
    TInt On(void);
    TInt Off(void);
  public:
    const TDesC8& Copyright(void) const;
  private:
    CVibraDriver();
    void ConstructL(void);
    void SetL(TBool aState);
  private:
    RPhoNet iPhoNet;
    TInt iState;
    TInt iSupported;
};

#endif
