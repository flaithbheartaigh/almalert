/*
    AlmSettingsServer.cpp
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

#include "AlmSettingsServer.hpp"
#include "AlmSettingsCommon.hpp"
#include <f32file.h>
#include <d32dbms_cleanup.hpp>

_LIT(KSettings,"AlmAlert.db");
_LIT(KLibs,"\\System\\Libs\\");

CAlmSettingsServer* CAlmSettingsServer::NewLC(void)
{
  CAlmSettingsServer* server=new(ELeave)CAlmSettingsServer(EPriorityNormal);
  CleanupStack::PushL(server);
  server->ConstructL();
  return server;
}

CAlmSettingsServer::~CAlmSettingsServer()
{
  if(iBase.InTransaction()) iBase.Rollback();
  iBase.Close();
  iSession.Close();
}

CAlmSettingsServer::CAlmSettingsServer(TInt aPriority): CServer(aPriority)
{
}

void CAlmSettingsServer::ConstructL(void)
{
  RFs fs;
  User::LeaveIfError(fs.Connect());
  CleanupClosePushL(fs);
  TFindFile find(fs);
  User::LeaveIfError(find.FindByDir(KSettings,KLibs));
  User::LeaveIfError(iSession.Connect());
  User::LeaveIfError(iBase.Open(iSession,find.File()));
  CleanupStack::PopAndDestroy(); //fs
  StartL(KSettingsServerName);
}

CSharableSession* CAlmSettingsServer::NewSessionL(const TVersion& aVersion) const
{
  if(!User::QueryVersionSupported(TVersion(KSettingsServerMajorVersionNumber,KSettingsServerMinorVersionNumber,KSettingsServerBuildVersionNumber),aVersion))
    User::Leave(KErrNotSupported);
  RThread client=Message().Client();
  return CAlmSettingsSession::NewL(client,*const_cast<CAlmSettingsServer*>(this));
}

void CAlmSettingsServer::IncrementSessions(void)
{
  ++iSessionCount;
}

void CAlmSettingsServer::DecrementSessions(void)
{
  --iSessionCount;
  if(iSessionCount<=0) CActiveScheduler::Stop();
}

void CAlmSettingsServer::ThreadFunctionL(void)
{
  CActiveScheduler* scheduler=new(ELeave)CActiveScheduler;
  CleanupStack::PushL(scheduler);
  CActiveScheduler::Install(scheduler);
  CAlmSettingsServer::NewLC();
  RSemaphore semaphore;
  User::LeaveIfError(semaphore.OpenGlobal(KSettingsServerSemaphoreName));
  semaphore.Signal();
  semaphore.Close();
  CActiveScheduler::Start();
  CleanupStack::PopAndDestroy(2); //CAlmSettingsServer,scheduler
}

TInt CAlmSettingsServer::ThreadFunction(TAny* aNone)
{
  CTrapCleanup* cleanup=CTrapCleanup::New();
  if(cleanup)
  {
    TRAPD(err,ThreadFunctionL());
    delete cleanup;
    return err;
  }
  return KErrNoMemory;
}

CAlmSettingsSession* CAlmSettingsSession::NewL(RThread& aClient,CAlmSettingsServer& aServer)
{
  CAlmSettingsSession* self=new(ELeave)CAlmSettingsSession(aClient,aServer);
  CleanupStack::PushL(self);
  self->ConstructL();
  CleanupStack::Pop(); //self
  return self;
}

CAlmSettingsSession::~CAlmSettingsSession()
{
  iServer.DecrementSessions();
}

void CAlmSettingsSession::ServiceL(const RMessage& aMessage)
{
  TRAPD(err,DispatchMessageL(aMessage));
  aMessage.Complete(err);
}

CAlmSettingsSession::CAlmSettingsSession(RThread& aClient,CAlmSettingsServer& aServer): CSession(aClient),iServer(aServer)
{
}

void CAlmSettingsSession::ConstructL(void)
{
  iServer.IncrementSessions();
}

HBufC8* CAlmSettingsSession::ValueLC(const TAny* aSrc)
{
  TInt len=Message().Client().GetDesLength(aSrc);
  HBufC8* result=HBufC8::NewLC(len);
  TPtr8 dst=result->Des();
  Message().ReadL(aSrc,dst);
  return result;
}

TUint32 CAlmSettingsSession::CategoryL(const TAny* aSrc)
{
  _LIT(KSQL,"select id from categories where name='%S'");
  RDbView view;
  TBuf<128> sql;
  HBufC8* param0=ValueLC(aSrc);
  TPtrC name((const TUint16*)param0->Ptr(),param0->Length()/2);
  sql.Format(KSQL,&name);
  User::LeaveIfError(view.Prepare(iServer.Db(),TDbQuery(sql),RDbView::EReadOnly));
  CleanupClosePushL(view);
  User::LeaveIfError(view.EvaluateAll());
  if(!view.FirstL()) User::Leave(KErrNotFound);
  view.GetL();
  TUint32 cid=view.ColUint32(1);
  CleanupStack::PopAndDestroy(2); //view,param1
  return cid;
}

void CAlmSettingsSession::DispatchMessageL(const RMessage& aMessage)
{
  TInt func=aMessage.Function();
  if(func<0||func>=ESettingsServerRequestLast) User::Leave(KErrNotSupported);
  _LIT(KSQL,"select id,name,cid,value from settings where cid=%u and name='%S'");
  RDbView view;
  TBuf<128> sql;
  TUint32 cid=CategoryL(Message().Ptr0());
  HBufC8* param1=ValueLC(Message().Ptr1());
  TPtrC name((const TUint16*)param1->Ptr(),param1->Length()/2);
  sql.Format(KSQL,cid,&name);
  User::LeaveIfError(view.Prepare(iServer.Db(),TDbQuery(sql),(func==ESettingsServerRequestSet)?RDbView::EUpdatable:RDbView::EReadOnly));
  CleanupClosePushL(view);
  User::LeaveIfError(view.EvaluateAll());
  TBool first=view.FirstL();
  if(func==ESettingsServerRequestSet)
  {
    if(first) view.UpdateL();
    else view.InsertL();
    CleanupCancelPushL(view);
    if(!first)
    {
      view.SetColL(2,name);
      view.SetColL(3,cid);
    }
    HBufC8* value=ValueLC(Message().Ptr2());
    view.SetColL(4,*value);
    view.PutL();
    CleanupStack::Pop(); //view cancel
    CleanupStack::PopAndDestroy(); //value
  }
  else
  {
    if(!first) User::Leave(KErrNotFound);
    view.GetL();
    TInt len=view.ColSize(4);
    if(func==ESettingsServerRequestGetData)
    {
      HBufC8* data=HBufC8::NewLC(len);
      TPtr8 ptr(data->Des());
      RDbColReadStream stream;
      stream.OpenLC(view,4);
      stream.ReadL(ptr,len);
      Message().WriteL(Message().Ptr2(),ptr);
      CleanupStack::PopAndDestroy(2); //stream,data
    }
    else
    {
      TPckgC<TInt> size(len);
      Message().WriteL(Message().Ptr2(),size);
    }
  }
  CleanupStack::PopAndDestroy(2); //view,param1
}

void StartAlmSettingsServerL(void)
{
  TFindServer find(KSettingsServerName);
  TFullName name;
  if(find.Next(name)==KErrNone) User::Leave(KErrGeneral);
  RSemaphore semaphore;
  User::LeaveIfError(semaphore.CreateGlobal(KSettingsServerSemaphoreName,0));
  CleanupClosePushL(semaphore);
  RThread thread;
  User::LeaveIfError(thread.Create(KSettingsServerName,CAlmSettingsServer::ThreadFunction,KDefaultStackSize,KMinHeapSize*16,KMinHeapSize*16,NULL));
  CleanupClosePushL(thread);
  thread.Resume();
  semaphore.Wait();
  CleanupStack::PopAndDestroy(2); //thread,semaphore
}

#include "AlmSettingsClient.hpp"
#include "AlmSettingsClientImplementation.hpp"