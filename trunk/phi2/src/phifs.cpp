/*
    phifs.cpp
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

#include "phifs.hpp"
#include "phipaneinterface.hpp"
#include <aknnotewrappers.h>
#include "philistbox.hpp"
#include "phiman.hpp"
#include <eikdll.h>
#include <apgcli.h> //RApaLsSession
#include <PinbMdl.hpp>
#include <PinbLnkApp.hpp>
#include <phi.rsg>
#include <aknmessagequerydialog.h> //CAknMessageQueryDialog
#include "phiconst.hpp"
#include "phiscan.hpp"
#include "phiutils.hpp"

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

const TUint KAttr[]={KEntryAttArchive,KEntryAttReadOnly,KEntryAttHidden,KEntryAttSystem};

CPhiFs* CPhiFs::NewL(MPhiPhaneInterface* anInterface,CPhiListBox* aListBox,CDesCArrayFlat* aFiles)
{
  CPhiFs* self=new(ELeave)CPhiFs(anInterface,aListBox,aFiles);
  CleanupStack::PushL(self);
  self->ConstructL();
  CleanupStack::Pop(); //self
  return self;
}

CPhiFs::~CPhiFs()
{
  delete iSelection;
  iFs.Close();
}

CPhiFs::CPhiFs(MPhiPhaneInterface* anInterface,CPhiListBox* aListBox,CDesCArrayFlat* aFiles): CBase(),iInterface(anInterface),iListBox(aListBox),iFiles(aFiles),iWhere(ERoot),iSortMode(ESortByName)
{
}

void CPhiFs::ConstructL(void)
{
  User::LeaveIfError(iFs.Connect());
  ReadListL(NULL);
}

const TDesC& CPhiFs::Up(void) const
{
  _LIT(KFolderUp,"..");
  return KFolderUp;
}

TInt CPhiFs::RefreshCallBack(TAny* aPhiFs)
{
  TRAPD(err,((CPhiFs*)aPhiFs)->RefreshL());
  return err;
}

void CPhiFs::SetFolderL(const TDesC& aFolder)
{
  TParse parse; TFileName file; TDesC* pos=NULL;
  parse.SetNoWild(iFileValue,NULL,NULL);
  if(aFolder.CompareF(Up())==0)
  {
    if(iFileValue.Length())
    {
      if(parse.PopDir()==KErrNone)
      {
        TParse parse2;
        parse2.SetNoWild(iFileValue,NULL,NULL);
        TPtrC ptr=parse2.DriveAndPath();
        file=ptr.Left(ptr.Length()-1);
        parse2.SetNoWild(file,NULL,NULL);
        file=parse2.NameAndExt();
        iFileValue=parse.DriveAndPath();
      }
      else
      {
        file=parse.Drive();
        iFileValue.Zero();
      }
      pos=&file;
    }
  }
  else
  {
    iFileValue=parse.DriveAndPath();
    iFileValue.Append(aFolder);
    iFileValue.Append('\\');
  }
  UpdateWhere();
  ReadListL(pos);
  iInterface->NotifyFsEvent(MPhiPhaneInterface::EFsEventFolderChanged);
}

void CPhiFs::UpdateFileL(void)
{
  if(iFileValue.Length())
  {
    TPckgC16<TPhiEntry> item((*iFiles)[iListBox->CurrentItemIndex()]);
    TParse parse;
    parse.SetNoWild(iFileValue,NULL,NULL);
    iFileValue=parse.DriveAndPath();
    iFileValue.Append(item().iName);
  }
  iInterface->NotifyFsEvent(MPhiPhaneInterface::EFsEventFileChanged);
}

const TFileName CPhiFs::CurrentFolder(void) const
{
  TParse parse;
  parse.SetNoWild(iFileValue,NULL,NULL);
  return parse.DriveAndPath();
}

void CPhiFs::FolderUpL(void)
{
  SetFolderL(Up());
}

void CPhiFs::FolderDownL(void)
{
  TPckgC16<TPhiEntry> item((*iFiles)[iListBox->CurrentItemIndex()]);
  if(item().IsDir())
  {
    SetFolderL(item().iName);
  }
}

void CPhiFs::CopyL(void)
{
  if(iSelection)
  {
    delete iSelection;
    iSelection=NULL;
  }
  iSelection=iListBox->SelectionLC(CurrentFolder());
  CleanupStack::Pop(); //iSelection
  PhiUtils::ShowMessageL(R_PHI_STRING_COPY);
}

void CPhiFs::CutL(void)
{
  if(iSelection)
  {
    delete iSelection;
    iSelection=NULL;
  }
  iSelection=iListBox->SelectionLC(CurrentFolder(),EFalse,CPhiSelection::EMove);
  CleanupStack::Pop(); //iSelection
  PhiUtils::ShowMessageL(R_PHI_STRING_CUT);
}

void CPhiFs::PasteL(void)
{
  if(iSelection)
  {
    CPhiMan::NewLD(iSelection,CurrentFolder(),TCallBack(RefreshCallBack,this));
    iSelection=NULL;
  }
}

void CPhiFs::DeleteL(void)
{
  switch(Where())
  {
    case EFiles:
      if(PhiUtils::ShowConfirmationL(R_PHI_DELETE_FILES))
      {
        CPhiSelection* selection=iListBox->SelectionLC(CurrentFolder(),EFalse,CPhiSelection::EDelete);
        CPhiMan::NewLD(selection,KNullDesC,TCallBack(RefreshCallBack,this));
        CleanupStack::Pop(); //selection
      }
      break;
    case EObjProcess:
      if(PhiUtils::ShowConfirmationL(R_PHI_KILL_PROCESSES)) KillProcessesL();
      break;
    case EObjThread:
      if(PhiUtils::ShowConfirmationL(R_PHI_KILL_THREADS)) KillThreadsL();
      break;
    default:
      break;
  }
}

void CPhiFs::StartItemL(void)
{
  TPckgC16<TPhiEntry> item((*iFiles)[iListBox->CurrentItemIndex()]);
  if(item().IsDir())
  {
    SetFolderL(item().iName);
  }
  else
  {
    TFileName file(CurrentFolder());
    file.Append(item().iName);
    if(item().IsExe())
    {
      EikDll::StartExeL(file);
    }
    else if(item().IsApp())
    {
      CApaCommandLine* cmd=CApaCommandLine::NewLC();
      cmd->SetLibraryNameL(file);
      cmd->SetCommandL(EApaCommandRun);
      EikDll::StartAppL(*cmd);
      CleanupStack::PopAndDestroy(cmd);
    }
    else
    {
      RApaLsSession session;
      User::LeaveIfError(session.Connect());
      CleanupClosePushL(session);

      TUid uid;
      TDataType dataType;
      if(session.AppForDocument(file,uid,dataType)==KErrNone)
      {
        TThreadId threadId;
        //User::LeaveIfError(
        session.StartDocument(file,dataType,threadId);
      }
      CleanupStack::PopAndDestroy(); //session
    }
  }
}

TBool CPhiFs::EditNameL(TInt aLabel,TDes& aValue)
{
  CAknTextQueryDialog* dlg=CAknTextQueryDialog::NewL(aValue);
  dlg->PrepareLC(R_PHI_DIALOG_EDIT_NAME);
  TBuf<32> title;
  CCoeEnv::Static()->ReadResourceAsDes16(title,aLabel);
  dlg->SetPromptL(title);
  return (dlg->RunLD()==EAknSoftkeyOk);
}

void CPhiFs::NewFolderL(void)
{
  TFileName newfolder;
  if(EditNameL(R_PHI_STRING_NEW_FOLDER,newfolder))
  {
    TFileName path(CurrentFolder());
    path.Append(newfolder);
    path.Append('\\');
    User::LeaveIfError(iFs.MkDir(path));
    RefreshL();
  }
}

void CPhiFs::RenameL(void)
{
  if(Where()==EFiles)
  {
    TPckgC16<TPhiEntry> item((*iFiles)[iListBox->CurrentItemIndex()]);
    if(item().iName.CompareF(Up())!=0)
    {
      TFileName newname(item().iName);
      if(EditNameL(R_PHI_STRING_RENAME,newname))
      {
        TFileName src(CurrentFolder()),dst(src);
        src.Append(item().iName);
        dst.Append(newname);
        User::LeaveIfError(iFs.Rename(src,dst));
        iFileValue.Copy(dst);
        RefreshL();
      }
    }
  }
}

void CPhiFs::RefreshL(void)
{
  TParse parse;
  parse.SetNoWild(iFileValue,NULL,NULL);
  TPckgC16<TPhiEntry> item((*iFiles)[iListBox->CurrentItemIndex()]);
  TPtrC ptr=(iWhere==ERoot)?item().iName.Mid(0):parse.NameAndExt();
  ReadListL(&ptr);
}

void CPhiFs::AddToPinboardL(void)
{
  if(Where()==EFiles)
  {
    TPckgC16<TPhiEntry> item((*iFiles)[iListBox->CurrentItemIndex()]);
    if(item().IsApp())
    {
      TFileName fullname(CurrentFolder());
      fullname.Append(item().iName);
      TEntry entry;
      User::LeaveIfError(iFs.Entry(fullname,entry));
      CPinbModel* model=CPinbModel::NewL();
      CleanupStack::PushL(model);
      CPinbLinkApp* app=CPinbLinkApp::NewL();
      CleanupStack::PushL(app);
      app->SetApplicationUidL(entry[2]);
      app->SetAppIconL();
      model->CreateLinkDocumentL(*app,ETrue);
      CleanupStack::PopAndDestroy(2); //app,model
    }
  }
}

void CPhiFs::ShowInfoL(void)
{
  TPckgC16<TPhiEntry> item((*iFiles)[iListBox->CurrentItemIndex()]);
  HBufC* message=HBufC::NewLC(8192);
  TBuf<128> buffer;
  TPtr ptr=message->Des();
  ptr.Append(item().iName);
  ptr.Append('\n');
  if(Where()!=ERoot) AppendInfoSize(ptr,R_PHI_INFO_SIZE,item().iSize);
  if(item().IsDir()&&(Where()==EFiles||(Where()==ERoot&&item().IsVolume())))
  {
    TFileName dir=CurrentFolder();
    dir.Append(item().iName);
    dir.Append('\\');
    CPhiScanner* scanner=CPhiScanner::NewSyncroLC(dir);
    AppendInfoSize(ptr,R_PHI_INFO_CONTENT,scanner->Size());
    CleanupStack::PopAndDestroy(); //scanner
    if(Where()==ERoot)
    {
      TInt drive; TVolumeInfo info;
      User::LeaveIfError(RFs::CharToDrive(item().iName[0],drive));
      User::LeaveIfError(iFs.Volume(info,drive));
      AppendInfoSize(ptr,R_PHI_INFO_SIZE,info.iSize.Low());
      AppendInfoSize(ptr,R_PHI_INFO_FREE,info.iFree.Low());
      if(info.iName.Length())
      {
        ptr.Append(info.iName);
        ptr.Append('\n');
      }
    }
  }
  FormatAtt(item().iAtt,buffer);
  if(buffer.Length())
  {
    AppendInfoResource(ptr,R_PHI_INFO_ATTR);
    ptr.Append(buffer);
    ptr.Append('\n');
  }
  if(item().iModified.Int64()>0)
  {
    AppendInfoResource(ptr,R_PHI_INFO_DATE);
    item().iModified.FormatL(buffer,_L("%D%M%Y%/0%1%/1%2%/2%3%/3 %:0%H%:1%T%:2%S%:3"));
    ptr.Append(buffer);
    ptr.Append('\n');
  }
  CAknMessageQueryDialog* dlg=CAknMessageQueryDialog::NewL(*message);
  CleanupStack::PopAndDestroy(); //message
  dlg->PrepareLC(R_AVKON_MESSAGE_QUERY_DIALOG);
  dlg->RunLD();
}

void CPhiFs::FormatAtt(TInt anAtt,TDes& aResult)
{
  const TUint res[]={R_PHI_ARCHIVE,R_PHI_READONLY,R_PHI_HIDDEN,R_PHI_SYSTEM};
  TBuf<1> attrValue;
  aResult.Zero();
  for(TUint i=0;i<sizeofa(KAttr);i++)
  {
    if(anAtt&KAttr[i])
    {
      CCoeEnv::Static()->ReadResourceAsDes16(attrValue,res[i]);
      aResult.Append(attrValue);
    }
  }
}

void CPhiFs::AppendInfoResource(TDes& aTarget,TInt aResourceID)
{
  TBuf<16> res;
  CCoeEnv::Static()->ReadResourceAsDes16(res,aResourceID);
  aTarget.Append(res);
  aTarget.Append(':');
  aTarget.Append(' ');
}

void CPhiFs::AppendInfoSize(TDes& aTarget,TInt aResourceID,TInt aSize)
{
  TBuf<64> buffer;
  AppendInfoResource(aTarget,aResourceID);
  PhiUtils::FormatSize(aSize,buffer);
  aTarget.Append(buffer);
  aTarget.Append(' ');
  aTarget.Append('(');
  aTarget.AppendNum(aSize);
  aTarget.Append(')');
  aTarget.Append('\n');
}

void CPhiFs::SetAttrL(void)
{
  if(Where()==EFiles)
  {
    TPckgC16<TPhiEntry> item((*iFiles)[iListBox->CurrentItemIndex()]);
    CArrayFixFlat<TInt>* indexArray=new(ELeave)CArrayFixFlat<TInt>(6);
    CAknListQueryDialog* dlg=new(ELeave)CAknListQueryDialog(indexArray);
    dlg->PrepareLC(R_PHI_SET_ATTR_QUERY);
    for(TUint i=0;i<sizeofa(KAttr);i++)
    {
      if(item().iAtt&KAttr[i])
      {
        dlg->ListBox()->View()->SelectItemL(i);
      }
    }
    if(dlg->RunLD()==EAknSoftkeyDone)
    {
      TUint att=0;
      for(TInt ii=0;ii<indexArray->Count();ii++)
      {
        att|=KAttr[indexArray->At(ii)];
      }
      TFileName fullname(CurrentFolder());
      fullname.Append(item().iName);
      iFs.SetAtt(fullname,att,(KEntryAttArchive|KEntryAttReadOnly|KEntryAttHidden|KEntryAttSystem)&~att);
      RefreshL();
    }
  }
}
