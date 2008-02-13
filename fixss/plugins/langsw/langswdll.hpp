/*
    langswdll.hpp
    Copyright (C) 2007 zg

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

#ifndef __LANGSWDLL_HPP__
#define __LANGSWDLL_HPP__

#include <w32adll.h>
#include <CommonEngine.hpp>

class CLangDll: public CAnimDll
{
  public:
    IMPORT_C CAnim* CreateInstanceL(TInt aType);
};

class CLang: public CWindowAnim
{
  public:
    ~CLang();
  public:
    TInt CommandReplyL(TInt aOpcode,TAny* aArgs);
    void Command(TInt aOpcode,TAny* aArgs);
    void Animate(TDateTime* aDateTime);
    TBool OfferRawEvent(const TRawEvent& aRawEvent);
    void ConstructL(TAny* aArgs,TBool aHasFocus);
    void Redraw(void);
    void FocusChanged(TBool aState);
  private:
    TBool iShift;
    TBool iKey;
    RSharedDataClient iSettings;
};

#endif
