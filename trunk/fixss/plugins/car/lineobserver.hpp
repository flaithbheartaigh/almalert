/*
    lineobserver.hpp
    Copyright (C) 2008 zg

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

#ifndef __LINEOBSERVER_HPP__
#define __LINEOBSERVER_HPP__

#include <e32base.h>
#include <etelagsm.h>

class CLineObserver: public CActive
{
  public:
    static void NewLC(void);
    ~CLineObserver();
  protected:
    void ConstructL(void);
    CLineObserver();
    void Close(void);
    void Vibrate(void);
    void VibrateL(void);
  protected:
    void RunL(void);
    void DoCancel(void);
  protected:
    RTelServer iServer;
    RAdvGsmPhone iPhone;
    RAdvGsmLine iLine;
    MAdvGsmLineStatusAndCallInfo::TAdvGsmLineStatus iPrevLineStatus;
    MAdvGsmLineStatusAndCallInfo::TAdvGsmLineStatus iLineStatus;
};

#endif
