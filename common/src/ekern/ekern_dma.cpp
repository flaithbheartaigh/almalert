/*
    ekern_dma.cpp
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

class TProcessMemoryInfo; //don't exists in symbian 6.1
#include <m32std.h>

void TDmaInterrupt::Service(void) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C TDma::TDma(): iChannel(-1),iInterrupt(this)
{
}

EXPORT_C TInt TDma::Bind(Dma::TDmaDevice aDevice) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C void TDma::UnBind(void) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void TDma::Cancel(void) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C TInt TDma::Transmit(const TUint8* aLogicalSource,TInt aCount) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt TDma::Receive(TUint8* aLogicalDest,TInt aCount) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt TDma::Copy(const TUint8* aLogicalSource,TUint8* aLogicalDest,TInt aCount) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt TDma::Fill(const TUint8* aBuffer, TInt aCount) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt TDma::Count(void)
{
  return iCount;
}
