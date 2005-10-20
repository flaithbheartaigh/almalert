/*
    FixSS.hpp
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

#ifndef __FIXSS_HPP__
#define __FIXSS_HPP__

#include <apmrec.h>

#define KUidRecogValue 0x1020810B

class CRecog: public CApaDataRecognizerType
{
  public:
    CRecog();
    TUint PreferredBufSize();
    TDataType SupportedDataTypeL(TInt aIndex) const;
  private:
    void DoRecognizeL(TDesC& aName,const TDesC8& aBuffer);
  public:
    static void StartThread(void);
};

#endif
