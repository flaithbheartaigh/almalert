/*
    ezipCZipFileMember.cpp
    Copyright (C) 2007 zg

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

#include <zipfile.h>

EXPORT_C TUint32 CZipFileMember::CRC32(void) const
{
  return iCRC32;
}

EXPORT_C TUint32 CZipFileMember::CompressedSize(void) const
{
  return iCompressedSize;
}

EXPORT_C const TFileName* CZipFileMember::Name(void) const
{
  return iName;
}

EXPORT_C TUint32 CZipFileMember::UncompressedSize(void) const
{
  return iUncompressedSize;
}

EXPORT_C CZipFileMember::~CZipFileMember()
{
  delete iName;
}
