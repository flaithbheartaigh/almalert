/*
    ekern_dsession.cpp
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

DSession::DSession(): iAvailable(EMaxMessages)
{
}

DSession::~DSession()
{
  if(iServer) iServerLink.Deque();
  if(iAttributes&EShared)
  {
    DSessionShare** shares=iShares,**last=iShares+iClients;
    while(shares<last) delete *shares++;
    User::Free(iShares);
  }
  else delete iPrimary;
  User::Free(iMessages);
}

void DSession::Create(DServer* aServer,DThread* aClient,TInt aMessageSlots)
{
  if(aMessageSlots>=0)
  {
    iAvailable=aMessageSlots;
    if(aMessageSlots>0)
    {
      if(aMessageSlots>EMaxMessages) User::Leave(KErrArgument);
      iMessages=iLocalPool.Allocate(aMessageSlots);
      User::LeaveIfNull(iMessages);
    }
    iAttributes|=ELocalPool;
  }
  iPrimary=DSessionShare::NewL(*this,*aClient,*aServer);
  Open();
  iServer=aServer;
  iServer->iSessionQ.AddLast(*this);
}

void DSession::SetSessionPtr(const TAny* aUserSession,TBool aSharable)
{
  iUserSession=aUserSession;
  if(aSharable) iAttributes|=ESharable;
}

void DSession::ShareL(RSessionBase::TAttachMode aAttachMode)
{
  DSessionShare** shares=(DSessionShare**)User::AllocL(EShareGranularity*sizeof(DSessionShare*));
  shares[0]=iPrimary;
  iShares=shares;
  iAlloc=EShareGranularity;
  iClients=1;
  iAttributes|=EShared;
  if(aAttachMode==RSessionBase::EAutoAttach) iAttributes|=EAutoAttach|EShared;
}

DSessionShare** DSession::FindEntry(DThread& aClient) const
{
  DSessionShare** shares=iShares;
  TInt top=iClients,i=0;
  while(i<top)
  {
    TUint index=(i+top)>>1;
    DThread* item=shares[index]->iClient;
    if(&aClient<item)
    {
      top=index;
      continue;
    }
    if(&aClient<=item) return (shares+index);
    i=index+1;
  }
  return NULL;
}

void DSession::AttachL(DThread& aClient)
{
  if(FindEntry(aClient)) return;
  if(!iServer) User::Leave(KErrServerTerminated);
  DSessionShare** shares=iShares;
  TInt top=iClients;
  if(top==iAlloc)
  {
    if(top>(EMaxShares-EShareGranularity)) User::Leave(KErrNotSupported);
    TInt newClients=top+EShareGranularity;
    shares=(DSessionShare**)User::ReAllocL(shares,newClients*sizeof(DSessionShare*));
    iShares=shares;
    iAlloc=newClients;
  }
  TInt i=0;
  while(i<top)
  {
    TUint index=(i+top)>>1;
    DThread* item=shares[index]->iClient;
    if(&aClient<item)
    {
      top=index;
      continue;
    }
    i=index+1;
  }
  DSessionShare* share=DSessionShare::NewL(*this,aClient,*iServer);
  Mem::Move(shares+i+1,shares+i,(iClients++-i)*sizeof(DSessionShare*));
  shares[i]=share;
}

void DSession::DiscardShare(DSessionShare& aShare)
{
  if(iAttributes&EShared)
  {
    DSessionShare** share=FindEntry(*aShare.iClient);
    Mem::Move(share,share+1,(TUint8*)(iShares+--iClients)-(TUint8*)share);
  }
  else iPrimary=NULL;
}

void DSession::CloseFromServer(void)
{
  if(iAttributes&EShared)
  {
    DSessionShare** first=iShares,**curr=iShares+iClients;
    while(curr>first) (*--curr)->ServerTerminated();
  }
  else if(iPrimary) iPrimary->ServerTerminated();
  iServerLink.Deque();
  iServer=NULL;
  CObject::Close();
}

void DSession::Close(void) //FIXME: NOT IMPLEMENTED
{
  if(iServer&&AccessCount()==2)
  {
    if(iAttributes&EShared)
    {
      DSessionShare** first=iShares,**curr=iShares+iClients;
      while(curr>first) (*--curr)->ServerTerminated();
    }
    else if(iPrimary) iPrimary->ServerTerminated();
    iDisconnect.iSession=this;
    //iServer->Deliver(*iMessages);
  }
  CObject::Close();
}
