/*
    shutdown.hpp
    Copyright (C) 2008 zg

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

#ifndef __SHUTDOWN_HPP__
#define __SHUTDOWN_HPP__

#include <CommonEngine.hpp>

class CShutdown: public CBase,public MSharedDataNotifyHandler
{
  public:
    static CShutdown* NewLC(void);
    ~CShutdown();
    void Wait(void);
  public:
    void HandleNotifyL(TUid anUid,const TDesC16& aKey,const TDesC16& aValue);
  private:
    CShutdown();
    void ConstructL(void);
  private:
    RSharedDataClient iSysAp;
};

#endif
