/*
    AlmSettingsCommon.hpp
    Copyright (C) 2006 zg

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

#ifndef __ALMSETTINGSCOMMON_HPP__
#define __ALMSETTINGSCOMMON_HPP__

#include <e32base.h>

_LIT(KSettingsServerName,"AlmSettingsServer");
_LIT(KSettingsServerSemaphoreName,"AlmSettingsServerSemaphore");

const TUint KSettingsServerMajorVersionNumber=1;
const TUint KSettingsServerMinorVersionNumber=0;
const TUint KSettingsServerBuildVersionNumber=1;

enum TSettingsServerRequest
{
  ESettingsServerRequestGetData,
  ESettingsServerRequestGetLength,
  ESettingsServerRequestSet,
  ESettingsServerRequestLast
};

#endif
