/*
    createdb.cpp
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

#include <e32base.h>
#include <e32cons.h>
#include <f32file.h>
#include <d32dbms.h>

_LIT(KPanic,"createdb");

_LIT(KDbName,"e:\\almalert.db");

_LIT(KFieldId,"id");
_LIT(KFieldName,"name");
_LIT(KFieldValue,"value");

_LIT(KIndex,"name");

_LIT(KTable,"settings");

const TInt KNameLength=32;

LOCAL_C void DoMainL(void)
{
  RFs fs;
  User::LeaveIfError(fs.Connect());
  CleanupClosePushL(fs);
  RDbNamedDatabase db;
  if(db.Create(fs,KDbName)==KErrNone)
  {
    CleanupClosePushL(db);
    //создаётся таблица
    CDbColSet* columns=CDbColSet::NewLC();

    TDbCol id(KFieldId,EDbColUint32);
    id.iAttributes=TDbCol::ENotNull|TDbCol::EAutoIncrement;
    columns->AddL(id);
    TDbCol name(KFieldName,EDbColText16,KNameLength);
    name.iAttributes=TDbCol::ENotNull;
    columns->AddL(name);
    TDbCol value(KFieldValue,EDbColLongBinary);
    value.iAttributes=TDbCol::ENotNull;
    columns->AddL(value);
    User::LeaveIfError(db.CreateTable(KTable,*columns));
    CleanupStack::PopAndDestroy(); //columns
    //создаётся индекс
    CDbKey* key=CDbKey::NewLC();
    TDbKeyCol iname(KFieldName);
    key->AddL(iname);
    key->MakeUnique();
    User::LeaveIfError(db.CreateIndex(KIndex,KTable,*key));
    CleanupStack::PopAndDestroy(); //key
    CleanupStack::PopAndDestroy(); //db
  }
  CleanupStack::PopAndDestroy(); //fs
}

LOCAL_C void MainL(void)
{
  CActiveScheduler* scheduler=new(ELeave)CActiveScheduler;
  CleanupStack::PushL(scheduler);
  CActiveScheduler::Install(scheduler);
  TRAPD(err,DoMainL());
  if(err!=KErrNone) User::Panic(KPanic,err);
  CleanupStack::PopAndDestroy(); //scheduler
}

GLDEF_C TInt E32Main(void)
{
  CTrapCleanup* cleanup=CTrapCleanup::New();
  TRAPD(error,MainL());
  delete cleanup;
  return 0;
}
