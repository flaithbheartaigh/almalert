/*
    ClkNitzMdl.hpp
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

#ifndef __CLKNITZMDL_HPP__
#define __CLKNITZMDL_HPP__

#include <etelagsm.h>

class RClkNitzMdlServer: public RSessionBase
{
  public:
    IMPORT_C TInt GetNitzSetting(TInt& aSetting) const;
    IMPORT_C TInt Connect(void);
    IMPORT_C TInt GetCurrentNITZInfo(MAdvGsmPhoneNitz::TNITZInfo& aInfo);
    IMPORT_C TInt ChangeNitzSetting(TInt aSetting);
    IMPORT_C void NotifyOnChangeCancel(void);
    IMPORT_C void NotifyOnChange(TRequestStatus& aStatus);
    IMPORT_C TVersion Version(void) const;
};

#endif
