/*
    ekern_impdma.cpp
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

#include <a32ngage.hpp>

TPtrC8 DmaChannels[7]=
{
  TPtrC8(KIrqDmaChannel0),
  TPtrC8(KIrqDmaChannel1),
  TPtrC8(KIrqDmaChannel2),
  TPtrC8(KIrqDmaChannel3),
  TPtrC8(KIrqDmaChannel4),
  TPtrC8(KIrqDmaChannel5),
  TPtrC8(KIrqDmaChannel6)
};

void ImpDma::Init1(void) //FIXME: NOT IMPLEMENTED
{
}

void ImpDma::Init3(void) //FIXME: NOT IMPLEMENTED
{
}

TInt ImpDma::MaxBlockSize()
{
  return 0x400000;
}

TInt ImpDma::GetChannel(TDmaDevice aDevice) //FIXME: NOT IMPLEMENTED
{
  return 0;
}

TPtrC8 ImpDma::GetInterrupt(TInt aChannel)
{
  if(aChannel>6||aChannel<0) PP::Panic(PP::EDmaInvalidChannel);
  return DmaChannels[aChannel];
}

void ImpDma::ClearInterrupt(TInt aChannel) //FIXME: NOT IMPLEMENTED
{
}

void ImpDma::InitialiseCopy(TInt aChannel, TDmaDevice aDevice, TPhysAddr aSrc, TPhysAddr aDest) //FIXME: NOT IMPLEMENTED
{
}

void ImpDma::InitialiseTransmit(TInt aChannel, TDmaDevice aDevice) //FIXME: NOT IMPLEMENTED
{
}

void ImpDma::InitialiseReceive(TInt aChannel, TDmaDevice aDevice) //FIXME: NOT IMPLEMENTED
{
}

void ImpDma::BlockCopy(TInt aChannel, TPhysAddr aSrc, TPhysAddr aDest, TInt aCount) //FIXME: NOT IMPLEMENTED
{
}

void ImpDma::BlockTransmit(TInt aChannel, TDmaDevice aDevice, TPhysAddr aBuffer, TInt aCount) //FIXME: NOT IMPLEMENTED
{
}

void ImpDma::BlockReceive(TInt aChannel, TDmaDevice aDevice, TPhysAddr aBuffer, TInt aCount) //FIXME: NOT IMPLEMENTED
{
}

void ImpDma::BlockFill(TInt aChannel, TDmaDevice aDevice, TPhysAddr aBuffer, TInt aCount) //FIXME: NOT IMPLEMENTED
{
}

void ImpDma::FinalBlock(TInt aChannel)
{
}

ImpDma::TDmaState ImpDma::GetState(TInt aChannel) //FIXME: NOT IMPLEMENTED
{
  return EBufferEmpty;
}

ImpDma::TDmaBufferMode ImpDma::BufferMode() //FIXME: NOT IMPLEMENTED
{
  return ESingleBuffer;
}

EXPORT_C TInt ImpDma61::GetDmaChannel(TDma* aDma,TInt& aChannel)
{
  for(TUint i=0;i<6;i++)
  {
    if(aDma==PP::DmaChannelTable[i])
    {
      aChannel=i*64;
      return KErrNone;
    }
  }
  aChannel=0;
  return KErrNotFound;
}
