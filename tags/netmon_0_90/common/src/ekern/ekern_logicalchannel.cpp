/*
    ekern_logicalchannel.cpp
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
#include <k32std.h>
#include <e32std.h>

EXPORT_C DLogicalChannel::DLogicalChannel(DLogicalDevice* aDevice): iDevice(aDevice)
{
  iDevice->iOpenChannels++;
}

EXPORT_C DLogicalChannel::~DLogicalChannel()
{
  iDevice->iOpenChannels--;
}

EXPORT_C void DLogicalChannel::Complete(TInt aReqNo,TInt aReason)
{
  iThread->RequestComplete(iRequestStatus[aReqNo],aReason);
}

EXPORT_C void DLogicalChannel::Complete(TInt aReqNo)
{
  Complete(aReqNo,KErrNone);
}

EXPORT_C void DLogicalChannel::CompleteAll(TInt aReason)
{
  for(TInt i=0;i<KMaxRequests;i++)
  {
    if(iRequestStatus[i]) iThread->RequestComplete(iRequestStatus[i],aReason);
  }
}

EXPORT_C void DLogicalChannel::Close()
{
  CObject::Close();
}

EXPORT_C void DLogicalChannel::DoCreateL(TInt aUnit,CBase* aDriver,const TDesC* anInfo, const TVersion& aVer)
{
}

EXPORT_C TInt DLogicalChannel::DoControl(TInt aFunction,TAny* a1,TAny* a2)
{
  return KErrNotSupported;
}

EXPORT_C void DLogicalChannel::SetBehaviour(TUint aValidRequests)
{
  iValidRequests=aValidRequests;
}
