/*
    phiman.cpp
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

#include "phiman.hpp"
#include "phiscan.hpp"
#include "phipckg.hpp"
#include "phientry.hpp"
#include "phiutils.hpp"
#include <phien.rsg>
#include <eikprogi.h>
#include <aknglobalconfirmationquery.h>

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

static void Log(const TDesC& aBuffer,TInt aParam)
{
  TBuf8<1024> cnvBuf;
  cnvBuf.Copy(aBuffer);
  Log(cnvBuf,aParam);
}
// */

_LIT(KMutexName,"phi.fileop.mutex");

void CPhiMan::NewLD(CPhiSelection* aSelection,const TDesC& aFolder,TCallBack aCallBack)
{
  CPhiMan* self=new(ELeave)CPhiMan(aSelection,aFolder,aCallBack);
  CleanupStack::PushL(self);
  self->ConstructL();
  CleanupStack::Pop(); //self
}

CPhiMan::~CPhiMan()
{
  delete iRedrawTimer;
  TRAPD(err,CleanupL());
  Cancel();
  delete iScanner;
  delete iMan;
  iMutex.Close();
  iFs.Close();
  if(iOwned) delete iSelection;
}

void CPhiMan::DialogDismissedL(TInt aButtonId)
{
  if(IsActive())
  {
    iRedrawTimer->Cancel();
    iProgressDialog=NULL; //dont' call ProcessFinishedL in in CleanupL
    Cancel();
    delete this;
  }
}

void CPhiMan::DoCancel(void)
{
  if(iCurrent==-1)
  {
    iScanner->Cancel();
  }
  else
  {
    iCancel=ETrue;
    iStatus=KErrCancel;
  }
}

void CPhiMan::RunL(void)
{
  //Log(_L8("RunL"),iStatus.Int());
  TBool stop=EFalse;
  if((iStatus==KErrNone||iStatus==KErrAlreadyExists||iStatus==KErrInUse)&&iError==KErrNone)
  {
    if(iCurrent>=0)
    {
      iSubCurrent++;
      if(iSubCurrent>iSubCurrentMax)
      {
        User::Invariant();
      }
      if(iSubCurrent==iSubCurrentMax)
      {
        iSubCurrent=0;
        iCurrent++;
      }
    }
    else iCurrent++;
    if(!iCurrent&&!iSubCurrent)
    {
      switch(iSelection->Type())
      {
        case CPhiSelection::ECopy:
        case CPhiSelection::EMove:
          iProgressInfo->SetFinalValue(iScanner->Size()+(iSubCurrentMax==2?iScanner->Count():0));
          break;
        default:
          iProgressInfo->SetFinalValue(iScanner->Count());
          break;
      }
    }
    if(iCurrent<iSelection->Array()->MdcaCount()) NextL();
    else stop=ETrue;
  }
  else
  {
    PhiUtils::ShowErrorL(R_PHI_ERROR,(iStatus==KErrNone)?iError:iStatus.Int());
    stop=ETrue;
  }
  if(stop) delete this;
}

MFileManObserver::TControl CPhiMan::NotifyFileManStarted(void)
{
/*
  Log(_L8("CPhiMan::NotifyFileManStarted"),0);
  Log(_L8("GetLastError"),iMan->GetLastError());
  Log(_L8("GetMoreInfoAboutError"),(TInt)iMan->GetMoreInfoAboutError());
  Log(_L8("CurrentAction"),(TInt)iMan->CurrentAction());
  TFileName src,dst;
  iMan->GetCurrentSource(src);
  iMan->GetCurrentTarget(dst);
  Log(src,0);
  Log(dst,1);
// */
  RMutex mutex;
  ManWaitL(mutex);
  iMan->GetCurrentSource(iCurrentItem);
  ManSignal(mutex);
  return iCancel?MFileManObserver::EAbort:MFileManObserver::EContinue;
}

MFileManObserver::TControl CPhiMan::NotifyFileManOperation(void)
{
/*
  Log(_L8("CPhiMan::NotifyFileManOperation"),0);
  Log(_L8("GetLastError"),iMan->GetLastError());
  Log(_L8("GetMoreInfoAboutError"),(TInt)iMan->GetMoreInfoAboutError());
  Log(_L8("CurrentAction"),(TInt)iMan->CurrentAction());
  TFileName src,dst;
  iMan->GetCurrentSource(src);
  iMan->GetCurrentTarget(dst);
  Log(src,0);
  Log(dst,1);
// */
  RMutex mutex;
  ManWaitL(mutex);
  iAddProgress+=iMan->BytesTransferredByCopyStep();
  ManSignal(mutex);

  return iCancel?MFileManObserver::ECancel:MFileManObserver::EContinue;
}

MFileManObserver::TControl CPhiMan::NotifyFileManEnded(void)
{
/*
  Log(_L8("CPhiMan::NotifyFileManEnded"),0);
  Log(_L8("GetLastError"),iMan->GetLastError());
  Log(_L8("GetMoreInfoAboutError"),(TInt)iMan->GetMoreInfoAboutError());
  Log(_L8("CurrentAction"),(TInt)iMan->CurrentAction());
  TFileName src,dst;
  iMan->GetCurrentSource(src);
  iMan->GetCurrentTarget(dst);
  Log(src,0);
  Log(dst,1);
// */
  MFileManObserver::TControl ok=MFileManObserver::EContinue;
  CFileMan::TAction action=iMan->CurrentAction();
  TInt err=iMan->GetLastError();
  if(err==KErrInUse&&iMan->GetMoreInfoAboutError()==ESrcOpenFailed)
  {
    TRAP(err,CopyLockedL());
  }
  else if(err==KErrAccessDenied&&(action==CFileMan::EDelete||action==CFileMan::ERmDir))
  {
    TRAP(err,ClearROL(EFalse));
    if(err==KErrNone) ok=MFileManObserver::ERetry;
  }
  else if(err==KErrAlreadyExists)
  {
    CAknGlobalConfirmationQuery* query=CAknGlobalConfirmationQuery::NewLC();
    TRequestStatus status;
    TFileName dst; TBuf<320> message;
    iMan->GetCurrentTarget(dst);
    message.Format(iOverwrite,&dst);
    query->ShowConfirmationQueryL(status,message,R_AVKON_SOFTKEYS_YES_NO);
    User::WaitForRequest(status);
    CleanupStack::PopAndDestroy(); //query
    if(status==EAknSoftkeyYes)
    {
      TRAP(err,DeleteL(dst));
      if(err==KErrNone) ok=MFileManObserver::ERetry;
    }
    else err=KErrNone;
  }
  if(ok==MFileManObserver::EContinue) User::LockedInc(iAddCount);
  if(ok!=MFileManObserver::ERetry&&iError==KErrNone) iError=err;
  return iCancel?MFileManObserver::EAbort:ok;
}

CPhiMan::CPhiMan(CPhiSelection* aSelection,const TDesC& aFolder,TCallBack aCallBack): CActive(EPriorityStandard),iSelection(aSelection),iDestination(aFolder),iCurrent(-1),iSubCurrent(0),iSubCurrentMax(1),iCallBack(aCallBack)
{
}

void CPhiMan::ConstructL(void)
{
  if(iSelection->Array()->MdcaCount()<=0) User::Invariant();
  CActiveScheduler::Add(this);
  CCoeEnv::Static()->ReadResourceAsDes16(iOverwrite,R_PHI_ASK_OVERWRITE);
  if(iSelection->Type()==CPhiSelection::EMove&&iDestination.Left(1).CompareF(iSelection->Folder().Left(1)))
  {
    iSubCurrentMax=2;
  }
  iRedrawTimer=CPeriodic::NewL(CActive::EPriorityStandard);
  iRedrawTimer->Start(100000,1000000,TCallBack(RedrawCallBack,this));
  User::LeaveIfError(iFs.Connect());
  User::LeaveIfError(iMutex.CreateGlobal(KMutexName));
  iMan=CFileMan::NewL(iFs,this);
  iScanner=CPhiScanner::NewLC(iSelection,iStatus);
  CleanupStack::Pop(); //iScanner
  SetActive();
  iProgressDialog=new(ELeave)CAknProgressDialog(reinterpret_cast<CEikDialog**>(&iProgressDialog),ETrue);
  iProgressDialog->SetTone(CAknNoteDialog::ENoTone);
  iProgressDialog->SetCallback(this);
  iProgressDialog->ExecuteLD(R_PHI_OPERATION_PROGRESS_NOTE_SOFTKEY_CANCEL);
  iProgressInfo=iProgressDialog->GetProgressInfoL();
  iOwned=ETrue;
}

void CPhiMan::CleanupL(void)
{
  if(iProgressDialog) iProgressDialog->ProcessFinishedL();
  iCallBack.CallBack();
}

void CPhiMan::NextL(void)
{
  TPckgC16<TPhiEntry> item(iSelection->Array()->MdcaPoint(iCurrent));
  TFileName source(iSelection->Folder()),destination(iDestination);
  source.Append(item().iName);
  TUint flags=(item().IsDir()?CFileMan::ERecurse:0);
  switch(iSelection->Type())
  {
    case CPhiSelection::ECopy:
      if(iSubCurrent) User::Invariant();
do_copy:
      if(item().IsDir())
      {
        destination.Append(item().iName);
        destination.Append('\\');
        iFs.MkDirAll(destination);
      }
      iMan->Copy(source,destination,flags,iStatus);
      break;
    case CPhiSelection::EMove:
      switch(iSubCurrentMax)
      {
        case 1:
          iMan->Move(source,destination,flags,iStatus);
          break;
        case 2:
          switch(iSubCurrent)
          {
            case 0:
              goto do_copy;
              break;
            case 1:
              goto do_delete;
              break;
            default:
              User::Invariant();
              break;
          }
          break;
        default:
          User::Invariant();
          break;
      }
      break;
    case CPhiSelection::EDelete:
      if(iSubCurrent) User::Invariant();
do_delete:
      if(item().IsDir())
      {
        source.Append('\\');
        iMan->RmDir(source,iStatus);
      }
      else
      {
        iMan->Delete(source,0,iStatus);
      }
      break;
    case CPhiSelection::EAttr:
      break;
    default:
      User::Invariant();
      break;
  }
  SetActive();
}

TInt CPhiMan::RedrawCallBack(TAny* aPhiMan)
{
  return ((CPhiMan*)aPhiMan)->DoRedraw();
}

TInt CPhiMan::DoRedraw(void)
{
  iMutex.Wait();
  switch(iSelection->Type())
  {
    case CPhiSelection::ECopy:
    case CPhiSelection::EMove:
      iProgressInfo->IncrementAndDraw(iAddProgress);
      break;
    default:
      iProgressInfo->IncrementAndDraw(iAddCount);
      break;
  }
  iAddProgress=0;
  iAddCount=0;
  TRAPD(err,iProgressDialog->SetTextL(iCurrentItem));
  iMutex.Signal();
  return 0;
}

void CPhiMan::ManWaitL(RMutex& aMutex)
{
  User::LeaveIfError(aMutex.OpenGlobal(KMutexName));
  aMutex.Wait();
}

void CPhiMan::ManSignal(RMutex& aMutex)
{
  aMutex.Signal();
  aMutex.Close();
}

void CPhiMan::CopyLockedL(void)
{
  TFileName src,dst;
  iMan->GetCurrentSource(src);
  iMan->GetCurrentTarget(dst);
  RFs fs;
  User::LeaveIfError(fs.Connect());
  CleanupClosePushL(fs);
  TEntry entry;
  User::LeaveIfError(fs.Entry(src,entry));
  HBufC8* buffer=HBufC8::NewLC(entry.iSize);
  TPtr8 ptr(buffer->Des());
  fs.ReadFileSection(src,0,ptr,entry.iSize);
  RFile file;
  User::LeaveIfError(file.Create(fs,dst,EFileWrite));
  CleanupClosePushL(file);
  User::LeaveIfError(file.Write(ptr));
  CleanupStack::PopAndDestroy(3); //file,buffer,fs
}

void CPhiMan::ClearROL(TBool aTarget)
{
  TFileName file;
  if(aTarget) iMan->GetCurrentTarget(file);
  else iMan->GetCurrentSource(file);
  RFs fs;
  User::LeaveIfError(fs.Connect());
  CleanupClosePushL(fs);
  TUint att;
  User::LeaveIfError(fs.Att(file,att));
  if(att&KEntryAttReadOnly==0) User::Leave(KErrGeneral);
  User::LeaveIfError(fs.SetAtt(file,0,KEntryAttReadOnly));
  CleanupStack::PopAndDestroy(); //fs
}

void CPhiMan::DeleteL(const TDesC& aFileName)
{
  RFs fs;
  User::LeaveIfError(fs.Connect());
  CleanupClosePushL(fs);
  User::LeaveIfError(fs.Delete(aFileName));
  CleanupStack::PopAndDestroy(); //fs
}
