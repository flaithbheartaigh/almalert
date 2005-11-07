/*
    isi_utils.cpp
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

#include <e32std.h>

void Des162Des8(const TDesC16& aSource,TDes8& aTarget)
{
  TInt length=aSource.Length()*2;
  aTarget.SetLength(length);
  TUint8* src=(TUint8*)&aSource[0];
  TUint8* dest=&aTarget[0];
  for(TInt i=0;i<length;i++)
  {
    TUint8* dest2=dest++;
    if(i&1)
    {
      *dest2=*(src++-1);
    }
    else
    {
      *dest2=*(++src);
    }
  }
}
