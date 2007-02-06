/*
    ekern_dserver.cpp
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

#include <k32std61.hpp>

inline void* operator new(TUint aSize,TMessageSlot* aPtr) {return aPtr;}

TMessageSlot* TMessagePool::Allocate(TInt aSize)
{
  TMessageSlot* slots=(TMessageSlot*)User::Alloc(aSize*sizeof(TMessageSlot));
  if(slots)
  {
    TMessageSlot* curr=slots+aSize;
    while(--curr>=slots) new(curr)TMessageSlot(this);
  }
  return slots;
}

TMessageSlot::TMessageSlot(TMessagePool* aPool): TMessageBase(),iStatus(0),iPool(0)
{
  iFunction=0;
  if(aPool!=iPool)
  {
    TDblQueLink* link=&(aPool->Get().iDelivery);
    aPool->Put(*this);
    iOwner.iNext=link;
  }
}

void TMessageSlot::Release(void)
{
  iDelivery.iNext=0;
  iOwner.Deque();
  if(iPool)
  {
    TDblQueLink* link=&(iPool->Get().iDelivery);
    iPool->Put(*this);
    iOwner.iNext=link;
    iShare->iSession->iAvailable++;
  }
}


DServer::DServer(): iSessionQ(_FOFF(DSession,iServerLink)),iDeliveredQ(_FOFF(TMessageBase,iDelivery))
{
}

DServer::~DServer() //FIXME: NOT IMPLEMENTED
{
}

void DServer::Create(const TDesC& aName)
{
  SetNameL(&aName);
  iOwningThread=K::SvThread;
}

void DServer::Cancel(void)
{
  if(iStatus) iOwningThread->RequestComplete(iStatus,KErrCancel);
}

void S::ServerCreateL(TInt& aHandle,DThread* anOwningThread,const TDesC& aName,DServer*& aServer)
{
  aServer=new(ELeave)DServer;
  aServer->Create(aName);
  K::Servers->AddL(aServer);
  aHandle=S::MakeHandle(aServer,anOwningThread);
}

TInt S::ServerCreate(TInt& aHandle,DThread* anOwningThread,const TDesC& aName)
{
  DServer* server=NULL;
  TRAPD(err,S::ServerCreateL(aHandle,anOwningThread,aName,server));
  if(err!=KErrNone) S::CheckedClose(server);
  return err;
}

EXPORT_C TInt Kern::ServerFind(TInt& aFindHandle,const TDesC& aMatch,TFullName& aName)
{
  return K::Servers->FindByFullName(aFindHandle,aMatch,aName);
}

EXPORT_C DServer* Kern::ServerFromHandle(TInt aHandle)
{
  return (DServer*)K::ObjectFromHandle(aHandle,NULL,K::Servers);
}
