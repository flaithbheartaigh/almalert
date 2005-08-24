/*
    AlmBirthdayTimer.hpp
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

#ifndef __ALMBIRTHDAYTIMER_HPP__
#define __ALMBIRTHDAYTIMER_HPP__

#include <e32base.h>

class CBirthdayTimer: public CTimer
{
  public:
    ~CBirthdayTimer();
    static CBirthdayTimer* NewL(TUint8 aHour,TCallBack aCallback);
  protected:
    virtual void RunL(void);
  private:
    CBirthdayTimer(TUint8 aHour,TCallBack aCallback);
    void ConstructL(void);
    void NearTime(void);
  private:
    TCallBack iCallback;
    TTime iTime;
    TUint8 iHour;
};

#endif
