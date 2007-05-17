/*
    AlmUsb.hpp
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

#ifndef __ALMUSB_HPP__
#define __ALMUSB_HPP__

#include <saclient.h>
#include "AlmLock.hpp"

class CUsbNotifier: public CActive
{
  public:
    static CUsbNotifier* NewLC(CLockNotifier& aNotifier);
    ~CUsbNotifier();
  protected:
    void RunL(void);
    void DoCancel(void);
  private:
    CUsbNotifier(CLockNotifier& aNotifier);
    void ConstructL(void);
    void NotifyL(void);
  private:
    CLockNotifier& iNotifier;
    RSystemAgent iSysAgnt;
    TSysAgentEvent iEvent;
};

#endif
