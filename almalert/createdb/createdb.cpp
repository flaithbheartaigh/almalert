/*
    createdb.cpp
    Copyright (C) 2006-2007 zg

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

#include <microdialog.hpp>
#include <f32file.h>
#include <d32dbms.h>
#include <d32dbms_cleanup.hpp>

_LIT(KUpdateThreadName,"zg.updater");

class CInstallDialog: public CBase,public MMicroDialogCallback
{
  public:
    CInstallDialog();
    ~CInstallDialog();
  private: //MDialogCallback
    void MicroDialogStart(MMicroDialogInterface* anInterface);
    void MicroDialogFinish(void);
    void MicroDialogTimer(void);
    void MicroDialogKey(const TKeyEvent& aKey);
    void MicroDialogRequest(void);
  private:
    static TInt ThreadFunction(TAny* aDialog);
    void ThreadFunctionL(void);
    static void InsertInternalL(RDbDatabase& aDb,const TDesC& aValue);
    static void InsertL(RDbDatabase& aDb,const TDesC& aValue);
    static void LeaveIfRealError(TInt aReason);
  private:
    MMicroDialogInterface* iInterface;
    TInt iState;
    RThread iThread;
    TRequestStatus iStatus;
};

CInstallDialog::CInstallDialog(): CBase()
{
}

CInstallDialog::~CInstallDialog()
{
  if(iStatus.Int()==KRequestPending) iThread.LogonCancel(iStatus);
  iThread.Close();
}

void CInstallDialog::MicroDialogStart(MMicroDialogInterface* anInterface)
{
  iInterface=anInterface;
  iInterface->Context().SetBrushStyle(CGraphicsContext::ESolidBrush);
  iInterface->Context().SetBrushColor(TRgb::Color4K(0x000));
  iInterface->Context().Clear();
  iInterface->Context().SetPenStyle(CGraphicsContext::ESolidPen);
  iInterface->Context().SetPenColor(TRgb::Color4K(0x4f4));
  iInterface->Context().DrawRect(TRect(36,168,140,178));
  User::LeaveIfError(iThread.Create(KUpdateThreadName,ThreadFunction,KDefaultStackSize,KMinHeapSize*16*8,KMinHeapSize*16*8,this));
  iThread.Resume();
  iThread.Logon(iStatus);
}

void CInstallDialog::MicroDialogFinish(void)
{
  iInterface=NULL;
}

void CInstallDialog::MicroDialogTimer(void)
{
  _LIT(KText1,"updating phone");
  _LIT(KText2,"all fine");
  _LIT(KText3,"error %d");
  _LIT(KTextContinue,"press «5»");
  iInterface->Context().SetBrushStyle(CGraphicsContext::ESolidBrush);
  iInterface->Context().SetBrushColor(TRgb::Color4K(0x000));
  iInterface->Context().SetPenStyle(CGraphicsContext::ESolidPen);
  const TDesC* ptr=&KText1;
  TBuf<32> buffer;
  if(iState==5)
  {
    if(iThread.ExitType()==EExitPanic||iThread.ExitReason()<KErrNone)
    {
      buffer.Format(KText3,iThread.ExitReason());
      ptr=&buffer;
    }
    else
    {
      ptr=&KText2;
    }
  }
  TRect text(TPoint(0,148),TSize(KScreenWidth,20));
  iInterface->Context().DrawText(*ptr,text,(text.Height()-iInterface->Font().HeightInPixels())/2+1+iInterface->Font().AscentInPixels(),CGraphicsContext::ECenter);
  if(iState==5)
  {
    text.Move(0,30);
    iInterface->Context().DrawText(KTextContinue,text,(text.Height()-iInterface->Font().HeightInPixels())/2+1+iInterface->Font().AscentInPixels(),CGraphicsContext::ECenter);
  }
  iInterface->Context().SetBrushColor(TRgb::Color4K(0x4f4));
  iInterface->Context().SetPenStyle(CGraphicsContext::ENullPen);
  iInterface->Context().DrawRect(TRect(38,170,38+iState*20,176));
  iInterface->MicroDialogRefresh();
}

void CInstallDialog::MicroDialogKey(const TKeyEvent& aKey)
{
  if(aKey.iCode=='5'&&iState==5) iInterface->MicroDialogClose();
}

void CInstallDialog::MicroDialogRequest(void)
{
  if(iStatus.Int()!=KRequestPending)
  {
    iState=5;
  }
}

void CInstallDialog::LeaveIfRealError(TInt aReason)
{
  if(aReason!=KErrAlreadyExists) User::LeaveIfError(aReason);
}

void CInstallDialog::InsertInternalL(RDbDatabase& aDb,const TDesC& aValue)
{
  _LIT(KSQLInsert,"select id,name from categories");
  RDbView view;
  User::LeaveIfError(view.Prepare(aDb,TDbQuery(KSQLInsert),RDbView::EInsertOnly));
  CleanupClosePushL(view);
  view.InsertL();
  CleanupCancelPushL(view);
  view.SetColL(2,aValue);
  view.PutL();
  CleanupStack::Pop(); //view cancel
  CleanupStack::PopAndDestroy(); //view
}

void CInstallDialog::InsertL(RDbDatabase& aDb,const TDesC& aValue)
{
  TRAPD(err,InsertInternalL(aDb,aValue));
  LeaveIfRealError(err);
}

_LIT(KPanic,"createdb");

_LIT(KFieldId,"id");
_LIT(KFieldCid,"cid");
_LIT(KFieldName,"name");
_LIT(KFieldValue,"value");

_LIT(KIndexCategories,"categories_name");
_LIT(KIndexSettings,"settings_cid_name");

_LIT(KTableCategories,"categories");
_LIT(KTableSettings,"settings");

const TInt KNameLength=32;

_LIT(KSettings,"AlmAlert.db");
_LIT(KLibs,"\\System\\Libs\\");

void CInstallDialog::ThreadFunctionL(void)
{
  RDbNamedDatabase db;

  RFs fs;
  User::LeaveIfError(fs.Connect());
  CleanupClosePushL(fs);

  TFindFile find(fs);
  if(find.FindByDir(KSettings,KLibs)!=KErrNone)
  {
    TFileName filename=RProcess().FileName();
    TParse parse;
    parse.SetNoWild(KSettings,&KLibs,&filename);
    User::LeaveIfError(db.Create(fs,parse.FullName()));
    db.Close();
    User::LeaveIfError(find.FindByDir(KSettings,KLibs));
  }

  RDbs session;
  User::LeaveIfError(session.Connect());
  CleanupClosePushL(session);

  User::LeaveIfError(db.Open(session,find.File()));

  CleanupClosePushL(db);
  //создаётся таблица
  CDbColSet* columns=CDbColSet::NewLC();
  TDbCol id(KFieldId,EDbColUint32);
  id.iAttributes=TDbCol::ENotNull|TDbCol::EAutoIncrement;
  columns->AddL(id);
  TDbCol name(KFieldName,EDbColText16,KNameLength);
  name.iAttributes=TDbCol::ENotNull;
  columns->AddL(name);
  LeaveIfRealError(db.CreateTable(KTableCategories,*columns));
  User::LockedInc(iState); //1
  TDbCol cid(KFieldCid,EDbColUint32);
  cid.iAttributes=TDbCol::ENotNull;
  columns->AddL(cid);
  TDbCol value(KFieldValue,EDbColLongBinary);
  columns->AddL(value);
  LeaveIfRealError(db.CreateTable(KTableSettings,*columns));
  CleanupStack::PopAndDestroy(); //columns
  User::LockedInc(iState); //2
  //создаются индексы
  CDbKey* key=CDbKey::NewLC();
  TDbKeyCol iname(KFieldName);
  key->AddL(iname);
  key->MakeUnique();
  LeaveIfRealError(db.CreateIndex(KIndexCategories,KTableCategories,*key));
  User::LockedInc(iState); //3
  key->Clear();
  TDbKeyCol icid(KFieldCid);
  key->AddL(icid);
  key->AddL(iname);
  key->MakeUnique();
  LeaveIfRealError(db.CreateIndex(KIndexSettings,KTableSettings,*key));
  CleanupStack::PopAndDestroy(); //key
  User::LockedInc(iState); //4
  //категории
  _LIT(KCategory1,"Alarm");
  _LIT(KCategory2,"Calendar");
  _LIT(KCategory3,"Beep");
  _LIT(KCategory4,"Birthday");
  _LIT(KCategory5,"Common");
  _LIT(KCategory6,"Synchronization");
  User::LeaveIfError(db.Begin());
  CleanupRollbackPushL(db);
  InsertL(db,KCategory1);
  InsertL(db,KCategory2);
  InsertL(db,KCategory3);
  InsertL(db,KCategory4);
  InsertL(db,KCategory5);
  InsertL(db,KCategory6);
  User::LeaveIfError(db.Commit());
  CleanupStack::Pop(); //db rollback
  CleanupStack::PopAndDestroy(); //db

  CleanupStack::PopAndDestroy(); //session
  CleanupStack::PopAndDestroy(); //fs
  User::LockedInc(iState); //5
}

TInt CInstallDialog::ThreadFunction(TAny* aDialog)
{
  CTrapCleanup* cleanup=CTrapCleanup::New();
  if(cleanup)
  {
    TRAPD(err,static_cast<CInstallDialog*>(aDialog)->ThreadFunctionL());
    delete cleanup;
    return err;
  }
  return KErrNoMemory;
}

LOCAL_C void MainL()
{
  CInstallDialog* callback=new(ELeave)CInstallDialog;
  CleanupStack::PushL(callback);
  RWsSession ws;
  User::LeaveIfError(ws.Connect());
  TInt id=ws.GetFocusWindowGroup();
  ws.Close();
  CMicroDialog::RunLD(callback,TSize(KScreenWidth,KScreenHeight),id);
  CleanupStack::PopAndDestroy(); //callback
}

GLDEF_C TInt E32Main()
{
  CTrapCleanup* cleanup=CTrapCleanup::New();
  TRAPD(error,MainL());
  delete cleanup;
  return 0;
}
