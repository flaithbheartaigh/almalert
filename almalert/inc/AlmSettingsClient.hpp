/*
    AlmSettingsClient.hpp
    Copyright (C) 2006-2007 zg

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

#ifndef __ALMSETTINGSCLIENT_HPP__
#define __ALMSETTINGSCLIENT_HPP__

#include <e32std.h>

class RAlmSettings: public RSessionBase
{
  public:
    RAlmSettings();
    TInt Connect(void);
    TInt Size(const TDesC& aCategory,const TDesC& aName,TInt& aSize);
    TInt Get(const TDesC& aCategory,const TDesC& aName,TDes8& aValue);
    TInt Get(const TDesC& aCategory,const TDesC& aName,TDes& aValue);
    TInt Get(const TDesC& aCategory,const TDesC& aName,TInt& aValue);
    TInt Set(const TDesC& aCategory,const TDesC& aName,const TDesC8& aValue);
    TInt Set(const TDesC& aCategory,const TDesC& aName,const TDesC& aValue);
    TInt Set(const TDesC& aCategory,const TDesC& aName,const TInt& aValue);
    TInt Compact(void);
    void Notify(TRequestStatus& aStatus);
    void NotifyCancel(void);
};

#endif
