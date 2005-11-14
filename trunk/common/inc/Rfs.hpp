/*
    Rfs.hpp
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

#ifndef __RFS_HPP__
#define __RFS_HPP__

#include <e32std.h>

/*
 * allowed commands:
 * DEL,RD,CP,SD,SPEC,DOS,FORMAT,MD,CD
 */

class RRfsClient: public RSessionBase
{
  public:
    enum TRfsType
    {
      ERestoreSettings,
      EFormat
    };
  public:
    IMPORT_C RRfsClient();
    IMPORT_C void Close(void);
    IMPORT_C TInt Connect(void);
    IMPORT_C TInt PerformPostEikonRfs(TRfsType aType);
    IMPORT_C TInt PerformRfs(TRfsType aType,TInt aValue);
    IMPORT_C TInt RunScriptInDescriptor(const TDesC16& aScript);
    IMPORT_C TInt RunScriptInFile(const TDesC16& aFileName);
};

#endif
