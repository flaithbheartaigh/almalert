/*
    ekern_dmedia.cpp
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

DMedia::DMedia(TInt aMediaNum): iMediaNum(aMediaNum)
{
}

void DMedia::CreateL(TMediaDevice aDevice)
{
  iDevice=aDevice;
}

void DMedia::Destroy(void)
{
  Close();
}

EXPORT_C void DMedia::Start(TMediaDevice aDevice,DThread *anOwningThread,TRequestStatus *aStatus) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C TInt DMedia::Open(TMediaDevice aDevice) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C void DMedia::CleanUp(TMediaDevice aDevice) //FIXME: NOT IMPLEMENTED
{
}

void DMedia::Close(void) //FIXME: NOT IMPLEMENTED
{
}

TInt DMedia::Enlarge(TInt aPartition,TInt aLength) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

TInt DMedia::ReduceSize(TInt aPartition,TInt64 &aPos,TInt aLength) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

DMediaDriver* DMedia::Driver(void)
{
  if(iStatus!=EStatClosed) return iDriver;
  else return NULL;
}

DPrimaryMediaBase::DPrimaryMediaBase(TInt aMediaNum): DMedia(aMediaNum),iMediaChangeDfc(TCallBack(MediaChangeDfc,this)),iPowerDownDfc(TCallBack(PowerDownDfc,this)),iChangedArray(20,2),iPartitionInfoCallBack(PartitionInfoCallBack,this),iPartitionInfoDfc(TCallBack(PartitionInfoDfc,this))
{
}

void DPrimaryMediaBase::CreateL(TMediaDevice aDevice)
{
  DMedia::CreateL(aDevice);
}

void DPrimaryMediaBase::Destroy(void)
{
  iChangedArray.Reset();
  DMedia::Destroy();
}

void DPrimaryMediaBase::AddChangeFlagsL(TDriveChangedFlags aChangedFlags)
{
  iChangedArray.Compress();
  TInt err=iChangedArray.Append(aChangedFlags);
  if(err!=KErrNone) User::Leave(err);
}

void DPrimaryMediaBase::RemoveChangeFlags(DThread* anOwningThread,TInt* aMountStatus)
{
  for(TInt i=0;i<iChangedArray.Count();i++)
  {
    if(iChangedArray[i].IsMatchingEntry(anOwningThread,aMountStatus))
    {
      iChangedArray.Remove(i);
      continue;
    }
  }
  iChangedArray.Compress();
}

void DPrimaryMediaBase::UpdateFlagsForOpenRequest(DThread* anOwningThread,TInt* aMountStatus)
{
  for(TInt i=iChangedArray.Count()-1;i>=0;i--)
  {
    if(iChangedArray[i].IsMatchingEntry(anOwningThread,aMountStatus))
    {
      iChangedArray[i].SetOpenRequestPending();
    }
  }
}

void DPrimaryMediaBase::QueueNotifyDfc(TBool aMediaChangeEvent)
{
  TDfc& dfc=aMediaChangeEvent?iMediaChangeDfc:iPowerDownDfc;
  if(!dfc.Queued()) Kern::Add(dfc);
}

TInt DPrimaryMediaBase::MediaChangeDfc(TAny *aPrimaryMedia)
{
  static_cast<DPrimaryMediaBase*>(aPrimaryMedia)->NotifyMediaStatusChange(ETrue);
  return KErrNone;
}

TInt DPrimaryMediaBase::PowerDownDfc(TAny *aPrimaryMedia)
{
  static_cast<DPrimaryMediaBase*>(aPrimaryMedia)->NotifyMediaStatusChange(EFalse);
  return KErrNone;
}

void DPrimaryMediaBase::NotifyMediaStatusChange(TBool aMediaChangeFlags) //FIXME: NOT IMPLEMENTED
{
}

void DPrimaryMediaBase::ReadPartitionInfo() //FIXME: NOT IMPLEMENTED
{
}

TInt DPrimaryMediaBase::PartitionInfoCallBack(TAny *aPrimaryMedia)
{
  TDfc& dfc=static_cast<DPrimaryMediaBase*>(aPrimaryMedia)->iPartitionInfoDfc;
  if(!dfc.Queued())
    Kern::Add(dfc);
  return KErrNone;
}

TInt DPrimaryMediaBase::PartitionInfoDfc(TAny *aPrimaryMedia) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

void DPrimaryMediaBase::ResetMediaDriverCurrent(DMediaDriver* aDriver)
{
}
