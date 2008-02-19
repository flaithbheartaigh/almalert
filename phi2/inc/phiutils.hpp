/*
    phiutils.hpp
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

#ifndef __PHIUTILS_HPP__
#define __PHIUTILS_HPP__

#include <e32std.h>
#include <aknnotewrappers.h>

class PhiUtils
{
  public:
    static void FormatSize(TInt aSize,TDes& aResult);
    static void ShowMessageL(TInt aResourceID);
    static void ShowMessageL(const TDesC& aMessage,CAknNoteDialog::TTimeout aTimeout);
    static TBool ShowConfirmationL(TInt aResourceID);
    static void ShowErrorL(TInt aResourceID,TInt anError);
};

#endif
