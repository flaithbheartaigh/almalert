/*
    microdialog.cpp
    Copyright (C) 2007 zg

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

#include "microdialog.hpp"
#include <fontids.hrh> //KScreenFontUidAlp12

_LIT(KMicroDialogThreadName,"zg.microdialog");

void CMicroDialog::RunLD(MMicroDialogCallback* aCallback,const TSize& aSize,TInt anId)
{
  CMicroDialog* self=new(ELeave)CMicroDialog(aCallback,aSize,anId);
  CleanupStack::PushL(self);
  self->ConstructL();
  CleanupStack::PopAndDestroy(); //self
}

CMicroDialog::~CMicroDialog()
{
  iThread.Close();
}

CMicroDialog::CMicroDialog(MMicroDialogCallback* aCallback,const TSize& aSize,TInt anId): CBase(),iCallback(aCallback),iSize(aSize),iId(anId)
{
}

void CMicroDialog::ConstructL(void)
{
  User::LeaveIfError(iThread.Create(KMicroDialogThreadName,MicroDialogThreadProc,KDefaultStackSize,KMinHeapSize,KMinHeapSize,this,EOwnerThread));
  iThread.Resume();
  iThread.Logon(iStatus);
  User::WaitForRequest(iStatus);
}

TInt CMicroDialog::MicroDialogThreadProc(TAny *aParam)
{
  TInt err=KErrNoMemory;
  CTrapCleanup *cleanup=CTrapCleanup::New();
  if(cleanup)
  {
    TRAP(err,STATIC_CAST(CMicroDialog*,aParam)->MicroDialogThreadProcL());
    delete cleanup;
  }
  return err;
}

void CMicroDialog::MicroDialogThreadProcL(void)
{
  CMicroDialogInt* dlg=new(ELeave)CMicroDialogInt(this);
  CleanupStack::PushL(dlg);
  dlg->ConstructL();
  dlg->Open();
  dlg->EventLoopL();
  dlg->Close();
  CleanupStack::PopAndDestroy(); //dlg
}

CMicroDialog::CMicroDialogInt::~CMicroDialogInt()
{
  iOwner->iCallback->MicroDialogFinish();
  iTimer.Close();
  if(iFont)
  {
    iBitmapContext->DiscardFont();
    iBitmapDevice->ReleaseFont(iFont);
  }
  delete iBitmapContext;
  delete iBitmapDevice;
  delete iBitmap;
  iWindow.Close();
  iRootWin.Close();
  iWsSession.Close();
}

CMicroDialog::CMicroDialogInt::CMicroDialogInt(CMicroDialog* anOwner): CBase(),iOwner(anOwner)
{
}

void CMicroDialog::CMicroDialogInt::ConstructL(void)
{
  User::LeaveIfError(iWsSession.Connect());
  iRootWin=RWindowGroup(iWsSession);
  User::LeaveIfError(iRootWin.Construct(Handle(),EFalse));

  CApaWindowGroupName* groupName=CApaWindowGroupName::NewLC(iWsSession);
  groupName->SetHidden(ETrue);
  groupName->SetWindowGroupName(iRootWin);
  CleanupStack::PopAndDestroy(); //groupName

  iWindow=RBackedUpWindow(iWsSession);
  User::LeaveIfError(iWindow.Construct(iRootWin,EColor4K,Handle()));
  User::LeaveIfError(iWindow.SetExtentErr(TPoint((KScreenWidth-iOwner->iSize.iWidth)/2,(KScreenHeight-iOwner->iSize.iHeight)/2),iOwner->iSize));
  iWindow.MaintainBackup();
  iBitmap=new(ELeave)CFbsBitmap;
  User::LeaveIfError(iBitmap->Duplicate(iWindow.BitmapHandle()));

  iBitmapDevice=CFbsBitmapDevice::NewL(iBitmap);
  iBitmapDevice->CreateBitmapContext(iBitmapContext);

  TAlgStyle style;
  User::LeaveIfError(iBitmapDevice->GetFontById(iFont,TUid::Uid(KScreenFontUidAlp12),style));
  iBitmapContext->UseFont(iFont);

  iWindow.Activate();
  iTimer.CreateLocal();

  iOwner->iCallback->MicroDialogStart(this);
}

void CMicroDialog::CMicroDialogInt::Open(void)
{
  iWindow.SetVisible(ETrue);
  iRootWin.SetOrdinalPosition(0);
  iRootWin.EnableReceiptOfFocus(ETrue);
  iRootWin.EnableFocusChangeEvents();
}

void CMicroDialog::CMicroDialogInt::Close(void)
{
  iRootWin.DisableFocusChangeEvents();
  iRootWin.EnableReceiptOfFocus(EFalse);
  iRootWin.SetOrdinalPosition(-1);
  iWindow.SetVisible(EFalse);
}

void CMicroDialog::CMicroDialogInt::EventLoopL(void)
{
  TRequestStatus eventStatus,timerStatus;

  iWsSession.EventReady(&eventStatus);
  iTimer.After(timerStatus,100000);
  iDoLoop=ETrue;
  TInt focus=EFalse;
  while(iDoLoop)
  {
    User::WaitForAnyRequest();
    if(eventStatus.Int()!=KRequestPending)
    {
      User::LeaveIfError(eventStatus.Int());
      TWsEvent event;
      iWsSession.GetEvent(event);
      switch(event.Type())
      {
        case EEventFocusLost:
          focus=EFalse;
          break;
        case EEventFocusGained:
          focus=ETrue;
          break;
        case EEventFocusGroupChanged:
          if(iWsSession.GetFocusWindowGroup()==iOwner->iId)
            iRootWin.SetOrdinalPosition(0);
          break;
        case EEventKey:
          iOwner->iCallback->MicroDialogKey(*(event.Key()));
          break;
      }
      iWsSession.EventReady(&eventStatus);
    }
    else if(timerStatus.Int()!=KRequestPending)
    {
      User::LeaveIfError(timerStatus.Int());
      if(focus) iOwner->iCallback->MicroDialogTimer();
      iTimer.After(timerStatus,100000);
    }
    else
    {
      iOwner->iCallback->MicroDialogRequest();
    }
  }
  iTimer.Cancel();
  iWsSession.EventReadyCancel();
}

void CMicroDialog::CMicroDialogInt::MicroDialogRefresh(void)
{
  iWindow.UpdateScreen();
  iWsSession.Flush();
}

void CMicroDialog::CMicroDialogInt::MicroDialogClose(void)
{
  iDoLoop=EFalse;
}

CBitmapContext& CMicroDialog::CMicroDialogInt::Context(void)
{
  return *iBitmapContext;
}

CFont& CMicroDialog::CMicroDialogInt::Font(void)
{
  return *iFont;
}

TUint32 CMicroDialog::CMicroDialogInt::Handle(void)
{
  return iHandle++;
}
