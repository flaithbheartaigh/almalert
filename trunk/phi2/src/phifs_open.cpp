/*
    phifs_open.cpp
    Copyright (C) 2008 zg

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
#include "philistbox.hpp"
#include <aknlistquerydialog.h>
#include <phien.rsg>
#include <eikfrlb.h>
#include <apgcli.h> //RApaLsSession

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

static TBool BlackApp(TInt32 iUid)
{
  TBool res=EFalse;
  switch(iUid)
  {
    case 0x10008d3f: //Z:\System\Apps\DdViewer\DdViewer.app
    case 0x10008d4a: //Z:\System\Apps\CodViewer\CodViewer.app
    case 0x101f466c: //Z:\System\Apps\videoui\videoui.app
    case 0x101f8c1b: //Z:\System\Apps\UnlockMMC\UnlockMMC.app
    case 0x100056cf: //Z:\System\Apps\ScreenSaver\ScreenSaver.app
    case 0x101f4cd3: //Z:\System\Apps\CbsUiApp\CbsUiApp.app
    case 0x10008d3c: //Z:\System\Apps\PushViewer\PushViewer.app
    case 0x101f4ce5: //Z:\System\Apps\bva\bva.app
    case 0x100058df: //Z:\System\Apps\MmsViewer\MmsViewer.app
    case 0x100058de: //Z:\System\Apps\MmsEditor\MmsEditor.app
    case 0x101f4ce4: //Z:\System\Apps\MsgMailViewer\MsgMailViewer.app
    case 0x101f4cd6: //Z:\System\Apps\MsgMailEditor\MsgMailEditor.app
    case 0x100058bd: //Z:\System\Apps\SmsViewer\SmsViewer.app
    case 0x100058bc: //Z:\System\Apps\SmsEditor\SmsEditor.app
    case 0x10005955: //Z:\System\Apps\Ussd\Ussd.app
    case 0x100058f4: //Z:\System\Apps\Startup\Startup.app
    case 0x100058f3: //Z:\System\Apps\SysAp\SysAp.app
    case 0x10005a10: //Z:\System\Apps\ppc\ppc.app
    case 0x100059d2: //Z:\System\Apps\CERTSAVER\CERTSAVER.app
    case 0x100059b5: //Z:\System\Apps\Autolock\Autolock.app
    case 0x101f4cd2: //Z:\System\Apps\Menu\Menu.app
    case 0x101f4cd1: //Z:\System\Apps\PhotoAlbum\PhotoAlbum.app
    case 0x10005907: //Z:\System\Apps\Notepad\Notepad.app
      res=ETrue;
      break;
  }
  return res;
}

void CPhiFs::OpenWithL(void)
{
  TPckgC16<TPhiEntry> item((*iFiles)[iListBox->CurrentItemIndex()]);
  if(Where()==EFiles&&!item().IsDir())
  {
    RApaLsSession session;
    User::LeaveIfError(session.Connect());
    CleanupClosePushL(session);
    User::LeaveIfError(session.GetAllApps());
    RArray<TInt> uids(10);
    CleanupClosePushL(uids);

    TInt index=0;
    CAknListQueryDialog* dlg=new(ELeave)CAknListQueryDialog(&index);
    dlg->PrepareLC(R_PHI_OPEN_WITH);
    CEikFormattedCellListBox* listbox=dlg->ListControl()->Listbox();
    CDesCArray* items=static_cast<CDesCArray*>(listbox->Model()->ItemTextArray());
    items->Delete(0);
    listbox->HandleItemRemovalL();

    TApaAppInfo info;
    while(session.GetNextApp(info)==KErrNone)
    {
      if(!BlackApp(info.iUid.iUid))
      {
        if(info.iUid.iUid==0x1000599d) //Z:\System\Apps\NpdViewer\NpdViewer.app
        {
          items->InsertL(0,info.iCaption);
          User::LeaveIfError(uids.Insert(info.iUid.iUid,0));
        }
        else
        {
          if(info.iCaption.Length()>=info.iShortCaption.Length())
            items->AppendL(info.iCaption);
          else
            items->AppendL(info.iShortCaption);
          User::LeaveIfError(uids.Append(info.iUid.iUid));
        }
      }
    }
    listbox->HandleItemAdditionL();

    if(dlg->RunLD()==EAknSoftkeyOk)
    {
      TFileName file(CurrentFolder());
      file.Append(item().iName);
      TDataType dataType(TUid::Uid(uids[index]));
      TThreadId threadId;
      session.StartDocument(file,dataType,threadId);
    }
    CleanupStack::PopAndDestroy(2); //uids,session
  }
}
