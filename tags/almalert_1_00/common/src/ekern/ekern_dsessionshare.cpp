/*
    ekern_dsessionshare.cpp
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

DSessionShare* DSessionShare::NewL(DSession& aSession,DThread& aClient,DServer& aServer)
{
  DSessionShare* share=new(ELeave)DSessionShare(aSession);
  TInt res=aClient.Open();
  if(res==KErrNone)
  {
    share->iClient=&aClient;
    res=S::MakeHandleAndOpen(&aClient,aServer.iOwningThread->iOwningProcess);
    if(res>=0)
    {
      share->iClientHandle=res;
      share->iExitHandler=new(ELeave)DThreadExit(*share);
      if(share->iExitHandler) return share;
      res=KErrNoMemory;
    }
  }
  delete share;
  User::Leave(res);
  return NULL;
}

DSessionShare::~DSessionShare()
{
  if(iExitHandler) iExitHandler->Discard();
  S::Close(iClientHandle,iSession->iServer->iOwningThread->iOwningProcess,NULL);
  TDblQueIter<TMessageSlot> outstandingIter(iOutstandingQ);
  TMessageSlot* item;
  while((item=outstandingIter++)) item->Release();
  S::CheckedClose(iClient);
}

void DSessionShare::Close(void)
{
  TDblQueIter<TMessageSlot> outstandingIter(iOutstandingQ);
  TMessageSlot* item;
  while((item=outstandingIter++))
  {
    if(item->iDelivery.iNext!=&(item->iDelivery))
    {
      item->iDelivery.Deque();
      item->Release();
    }
  }
  if(iOutstandingQ.IsEmpty()) Discard();
}

void DSessionShare::ServerTerminated(void)
{
  TDblQueIter<TMessageSlot> outstandingIter(iOutstandingQ);
  TMessageSlot* item;
  while((item=outstandingIter++))
  {
    item->Release();
    if(item->iStatus) iClient->RequestComplete(item->iStatus,KErrServerTerminated);
  }
  Discard();
}

void DSessionShare::Discard(void) //FIXME: NOT IMPLEMENTED
{
}

DSessionShare::DThreadExit::DThreadExit(DSessionShare& aShare): DCleanup(EShare),iShare(&aShare)
{
  aShare.iClient->AddToCleanup(*this);
}

DSessionShare::DThreadExit::~DThreadExit()
{
  if(iShare) iShare->ThreadExit();
}

void DSessionShare::DThreadExit::Discard(void)
{
  iLink.Deque();
  iShare=NULL;
  if(this) delete this;
}
