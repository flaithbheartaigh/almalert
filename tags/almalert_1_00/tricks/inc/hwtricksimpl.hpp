/*
    hwtricks_impl.hpp
    Copyright (C) 2005-2006 zg

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

#ifndef __HWTRICKSIMPL_HPP__
#define __HWTRICKSIMPL_HPP__

#include "hwtricks.hpp"
#include "hwtricksextender.hpp"
#include <phonet.hpp>

class CHWServerImpl: public CHWServer
{
  public:
    ~CHWServerImpl();
  public: //CHWServer
    void SendL(CIsiMsg& aMsg);
    void ReceiveL(TRequestStatus& aStatus,CIsiMsg& aMsg,TPckgBuf<TUint16>& aLen);
    void ReceiveCancelL(void);
  protected:
    CHWServerImpl();
    void ConstructL(TBool anExtended);
    static const TDesC8& Copyright(void);
  private:
    TBool iCaptured;
    RMutex iMutex;
    RPhoNet iPhoNet;
    CHWExtender* iExtender;
  friend class CHWServer;
};

#endif
