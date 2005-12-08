/*
    LoadNotifier.hpp
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

#ifndef __LOADNOTIFIER_HPP__
#define __LOADNOTIFIER_HPP__

#include <CommonEngine.hpp>

class CLoadNotifier: public CBase,public MSharedDataNotifyHandler
{
  public:
    static CLoadNotifier* NewLC(void);
    ~CLoadNotifier();
    void Wait(void);
  public:
    void SharedDataNotify(TUid anUid,const TDesC16& aKey,const TDesC16& aValue);
  private:
    CLoadNotifier();
    void ConstructL(void);
    void ProcessStateL(TUint8 aState);
    void Patch1stL(void);
    void Patch2ndL(void);
    void LoadPluginsL(TUint8 aState);
    static TInt LoadPlugin(TAny* aPtr);
  private:
    struct SThreadData
    {
      const TDesC& iName;
      const TDesC& iPath;
    };
  private:
    RSharedDataClient iSysAp;
};

#define w(W) __(D##W)
#define ww(_g) ___(_g##g)
#define _(bb) w(bb##b)
#define __(v) ww(v##u)
#define ___(____) R##____
#define user _(e)
#define op(a,b,c,d) a##Memory(b,(TUint)(c),d##P,sizeof(d))

#endif
