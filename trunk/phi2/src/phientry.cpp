/*
    phientry.cpp
    Copyright (C) 2007-2008 zg

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

#include "phientry.hpp"
#include <f32file.h>

TPhiEntry::TPhiEntry(): iAtt(0),iSize(0)
{
  TInt64 time(0);
  iModified=time;
}

void TPhiEntry::Clear(void)
{
  iAtt=0;
  iSize=0;
  TInt64 time(0);
  iModified=time;
  iName.Zero();
}

TBool TPhiEntry::IsDir(void) const
{
  return iAtt&KEntryAttDir;
}

TBool TPhiEntry::IsVolume(void) const
{
  return iAtt&KEntryAttVolume;
}

TBool TPhiEntry::IsHidden(void) const
{
  return iAtt&(KEntryAttSystem|KEntryAttHidden);
}

TBool TPhiEntry::IsApp(void) const
{
  _LIT(KAppMask,"*.app");
  return !IsDir()&&iName.MatchF(KAppMask)>=0;
}

TBool TPhiEntry::IsExe(void) const
{
  _LIT(KExeMask,"*.exe");
  return !IsDir()&&iName.MatchF(KExeMask)>=0;
}

TBool TPhiEntry::IsImage(void) const
{
  _LIT(KBmpMask,"*.bmp");
  _LIT(KJpgMask,"*.jpg");
  _LIT(KPngMask,"*.png");
  return !IsDir()&&(iName.MatchF(KBmpMask)>=0||iName.MatchF(KJpgMask)>=0||iName.MatchF(KPngMask)>=0);
}
