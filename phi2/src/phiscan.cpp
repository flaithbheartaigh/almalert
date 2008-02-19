/*
    phiscan.cpp
    Copyright (C) 2007-2008 zg

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "phiscan.hpp"
#include "phipckg.hpp"
#include "phientry.hpp"

/*
#include <f32file.h>
static void Log(const TDesC8& aBuffer,TInt aParam)
{
  RFs fs;
  RFile file;
  fs.Connect();
  TInt err=file.Open(fs,_L("e:\\zg_phi_log.txt"),EFileWrite|EFileShareAny);
  if(err==KErrNotFound) err=file.Create(fs,_L("e:\\zg_phi_log.txt"),EFileWrite|EFileShareAny);
  TInt pos=0;
  err=file.Seek(ESeekEnd,pos);
  TBuf8<1024> logBuf;
  TTime localTime; RThread thread;
  localTime.HomeTime();
  logBuf.Format(_L8("0x%08.8d|%02.2d:%02.2d:%02.2d.%06.6d: %S: 0x%x\n"),thread.Id(),localTime.DateTime().Hour(),localTime.DateTime().Minute(),localTime.DateTime().Second(),localTime.DateTime().MicroSecond(),&aBuffer,aParam);
  file.Write(logBuf);
  file.Close();
  fs.Close();
}

static void Log2(const TDesC8& aBuffer,TInt aParam)
{
  RFs fs;
  RFile file;
  fs.Connect();
  TInt err=file.Open(fs,_L("e:\\zg_phi_log2.txt"),EFileWrite|EFileShareAny);
  if(err==KErrNotFound) err=file.Create(fs,_L("e:\\zg_phi_log2.txt"),EFileWrite|EFileShareAny);
  TInt pos=0;
  err=file.Seek(ESeekEnd,pos);
  TBuf8<1024> logBuf;
  TTime localTime; RThread thread;
  localTime.HomeTime();
  logBuf.Format(_L8("0x%08.8d|%02.2d:%02.2d:%02.2d.%06.6d: %S: 0x%x\n"),thread.Id(),localTime.DateTime().Hour(),localTime.DateTime().Minute(),localTime.DateTime().Second(),localTime.DateTime().MicroSecond(),&aBuffer,aParam);
  file.Write(logBuf);
  file.Close();
  fs.Close();
}
*/

CPhiScanner* CPhiScanner::NewLC(const CPhiSelection* aSelection,TRequestStatus& aStatus)
{
  CPhiScanner* self=new(ELeave)CPhiScanner(aSelection,aStatus);
  CleanupStack::PushL(self);
  self->ConstructL();
  return self;
}

CPhiScanner* CPhiScanner::NewSyncroLC(const TDesC& aDir)
{
  CPhiScanner* self=new(ELeave)CPhiScanner();
  CleanupStack::PushL(self);
  User::LeaveIfError(self->iFs.Connect());
  CleanupClosePushL(self->iFs);
  self->ScanDirL(aDir);
  CleanupStack::PopAndDestroy(); //iFs
  return self;
}

CPhiScanner::~CPhiScanner()
{
  Cancel();
  iThread.Close();
}

void CPhiScanner::Cancel(void)
{
  if(iThreadStarted&&iStatus.Int()==KRequestPending)
  {
    iThread.LogonCancel(iStatus);
    iThread.Kill(KErrCancel);
  }
}

CPhiScanner::CPhiScanner(): CBase(),iStatus(*(TRequestStatus*)0)
{
}

CPhiScanner::CPhiScanner(const CPhiSelection* aSelection,TRequestStatus& aStatus): CBase(),iSelection(aSelection),iStatus(aStatus)
{
}

void CPhiScanner::ConstructL(void)
{
  _LIT(KThreadName,"phi.scan.thread");
  User::LeaveIfError(iThread.Create(KThreadName,ThreadFunction,KDefaultStackSize,KMinHeapSize,KMinHeapSize,this));
  iThread.Resume();
  iThread.Logon(iStatus);
  iThreadStarted=ETrue;
}

void CPhiScanner::ScanDirL(const TDesC& aDir)
{
  TFileName mask(aDir);
  mask.Append('*');
  RDir dir;
  User::LeaveIfError(dir.Open(iFs,aDir,KEntryAttMaskSupported));
  CleanupClosePushL(dir);
  TEntry entry;
  while(dir.Read(entry)==KErrNone)
  {
    if(entry.IsDir())
    {
      TFileName dir(aDir);
      dir.Append(entry.iName);
      dir.Append('\\');
      ScanDirL(dir);
    }
    else
    {
      iCount++;
      iSize+=entry.iSize;
    }
  }
  CleanupStack::PopAndDestroy(); //dir
}

TInt CPhiScanner::ThreadFunction(TAny* aPtr)
{
  TInt err=KErrNoMemory;
  CTrapCleanup* cleanup=CTrapCleanup::New();
  if(cleanup)
  {
    TRAP(err,static_cast<CPhiScanner*>(aPtr)->ThreadFunctionL());
    delete cleanup;
  }
  return err;
}

void CPhiScanner::ThreadFunctionL(void)
{
  User::LeaveIfError(iFs.Connect());
  CleanupClosePushL(iFs);
  TInt count=iSelection->Array()->MdcaCount();
  for(TInt ii=0;ii<count;ii++)
  {
    TPckgC16<TPhiEntry> item(iSelection->Array()->MdcaPoint(ii));
    if(item().IsDir())
    {
      TFileName dir(iSelection->Folder());
      dir.Append(item().iName);
      dir.Append('\\');
      ScanDirL(dir);
    }
    else
    {
      iCount++;
      iSize+=item().iSize;
    }
  }
  CleanupStack::PopAndDestroy(); //iFs
}
