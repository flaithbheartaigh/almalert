/*
    ekern_kern.cpp
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

EXPORT_C TInt Kern::MessageComplete(TInt aHandle,TInt aReason) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C DChunk* Kern::ChunkFromHandle(TInt aHandle) //FIXME: NOT IMPLEMENTED
{
  return NULL;
}

EXPORT_C TInt Kern::ChunkFind(TInt &aFindHandle,const TDesC &aMatch,TFullName &aName) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Kern::ChannelFind(TInt &aFindHandle,const TDesC &aMatch,TFullName &aName) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C DLogicalChannel* Kern::ChannelFromHandle(TInt aHandle) //FIXME: NOT IMPLEMENTED
{
  return NULL;
}

EXPORT_C void Kern::EventCaptureHook(void) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void Kern::EventReleaseHook(void) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void Kern::EventRequest(TRawEventBuf &anEvent,TRequestStatus &aStatus) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void Kern::EventRequestCancel(void) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C DLogicalDevice* Kern::LogicalDeviceFromHandle(TInt aHandle) //FIXME: NOT IMPLEMENTED
{
  return NULL;
}

EXPORT_C TInt Kern::LogicalDeviceFind(TInt &aFindHandle,const TDesC &aMatch,TFullName &aName) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Kern::LibraryFind(TInt &aFindHandle,const TDesC &aMatch,TFullName &aName) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C DLibrary* Kern::LibraryFromHandle(TInt aHandle) //FIXME: NOT IMPLEMENTED
{
  return NULL;
}

EXPORT_C DMutex* Kern::MutexFromHandle(TInt aHandle) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Kern::MutexFind(TInt &aFindHandle,const TDesC &aMatch,TFullName &aName) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C DPhysicalDevice* Kern::PhysicalDeviceFromHandle(TInt aHandle) //FIXME: NOT IMPLEMENTED
{
  return NULL;
}

EXPORT_C TInt Kern::PhysicalDeviceFind(TInt &aFindHandle,const TDesC &aMatch,TFullName &aName) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C DProcess* Kern::ProcessFromHandle(TInt aHandle) //FIXME: NOT IMPLEMENTED
{
  return NULL;
}

EXPORT_C TInt Kern::ProcessFind(TInt &aFindHandle,const TDesC &aMatch,TFullName &aName) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TUint32 Kern::Random() //FIXME: NOT IMPLEMENTED
{
  return 0;
}

EXPORT_C void Kern::RandomSalt(TUint32 aBitOfSalt) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C DSemaphore* Kern::SemaphoreFromHandle(TInt aHandle) //FIXME: NOT IMPLEMENTED
{
  return NULL;
}

EXPORT_C TInt Kern::SemaphoreFind(TInt &aFindHandle,const TDesC &aMatch,TFullName &aName) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C DThread* Kern::ThreadFromHandle(TInt aHandle) //FIXME: NOT IMPLEMENTED
{
  return NULL;
}

EXPORT_C DThread* Kern::ThreadFromId(TThreadId aId) //FIXME: NOT IMPLEMENTED
{
  return NULL;
}

EXPORT_C TInt Kern::ThreadFind(TInt &aFindHandle,const TDesC &aMatch,TFullName &aName) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C DTimer* Kern::TimerFromHandle(TInt aHandle) //FIXME: NOT IMPLEMENTED
{
  return NULL;
}

EXPORT_C TTime Kern::HomeTime()
{
  return Plat::SystemTime()+Kern::HomeTimeOffset();
}

EXPORT_C TTimeIntervalSeconds Kern::HomeTimeOffset() //FIXME: NOT IMPLEMENTED
{
  return TTimeIntervalSeconds(0);
}

EXPORT_C RHeap& Kern::Heap()
{
  return *(Plat::CurrentThread().iHeap);
}

EXPORT_C RHeap& Kern::HeapSwitch(RHeap& aHeap) //FIXME: NOT IMPLEMENTED
{
  return aHeap;
}

EXPORT_C void Kern::NotifyChanges(TUint aChangesMask) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void Kern::NotifyThreadDeath(DThread* aThread) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void Kern::NameFromHandle(TInt aHandle,DThread *aThread,TDes &aName) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void Kern::FullNameFromHandle(TInt aHandle,DThread *aThread,TDes &aFullName) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void Kern::SetIdleCallBack(TIdleCallBack aCallback) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void Kern::ClearIdleCallBack() //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C DPeriphBusController* Kern::PBusController(TPBusNum aMachineBusNum) //FIXME: NOT IMPLEMENTED
{
  return NULL;
}

EXPORT_C CObjectCon* Kern::Processes() //FIXME: NOT IMPLEMENTED
{
  return NULL;
}

EXPORT_C CObjectCon* Kern::Threads() //FIXME: NOT IMPLEMENTED
{
  return NULL;
}

EXPORT_C CObjectCon* Kern::Chunks() //FIXME: NOT IMPLEMENTED
{
  return NULL;
}

EXPORT_C CObjectCon* Kern::LogicalDevices() //FIXME: NOT IMPLEMENTED
{
  return NULL;
}

EXPORT_C CObjectCon* Kern::PhysicalDevices() //FIXME: NOT IMPLEMENTED
{
  return NULL;
}

EXPORT_C CObjectCon* Kern::Channels() //FIXME: NOT IMPLEMENTED
{
  return NULL;
}

EXPORT_C CObjectCon* Kern::Libraries() //FIXME: NOT IMPLEMENTED
{
  return NULL;
}
