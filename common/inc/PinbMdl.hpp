/*
    PinbMdl.hpp
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

#ifndef __PINBMDL_HPP__
#define __PINBMDL_HPP__

#include <apgcli.h>

class CPinbLinkBase;
class CPinbDirMonitor;
class MPinbEntryChangedCallBack;
class CEikMenuPane;
class CCoeAppUi;
class CEikonEnv;
class CDocumentHandler;
class CCoeControl;
class CNotepadApi;
class CPinbModelResource;

class CPinbModel: public CBase
{
  public:
    IMPORT_C static CPinbModel* NewL(MPinbEntryChangedCallBack* aCallBack=NULL);
    IMPORT_C ~CPinbModel();
  public:
    IMPORT_C TBool AddLinkMenuItemL(CEikMenuPane& aMenuPane,TInt aPreviousId);
    IMPORT_C void CreateLinkDocumentL(CPinbLinkBase& aLink,TBool aRequireConfirmation=EFalse);
    IMPORT_C TBool ValidateLinkL(const TDesC& aLinkName);
    IMPORT_C TThreadId StartLinkL(const TDesC& aLinkName);
    IMPORT_C void UpdateLinkL(const TDesC& aOldFileName,const TDesC& aNewFileName);
    IMPORT_C void GetLinkFileDirectory(TDes& aPathName);
    IMPORT_C void GetDefaultLinkFileDirectory(TDes& aPathName);
    IMPORT_C void GetLinkFileExtension(TDes& aExtension);
    IMPORT_C void GetMMCLinkFileDirectory(TDes& aPathName) const;
    IMPORT_C CPinbLinkBase* RestoreLinkFileL(const TDesC& aFileName);
    IMPORT_C void DoDirMonitorNotify(void);
    IMPORT_C void SetApplicationControl(CCoeAppUi* aAppUi);
    IMPORT_C void SetConatinerControl(CCoeControl* aContainerControl);
    IMPORT_C void RemoveNotepadApi(void);
  private:
    void ConstructL(MPinbEntryChangedCallBack* aCallBack);
    CPinbModel();
  private:
    RApaLsSession iAppArcSession;
    CPinbModelResource* iModelResource;
    CPinbDirMonitor* iMonitor;
    CCoeAppUi* iAppUi;
    CEikonEnv* iEnv;
    CDocumentHandler* iDocHandler;
    CCoeControl* iContainerControl;
    TBool iExistPinbApp;
    CNotepadApi* iNotepadApi;
    CPinbDirMonitor* iMMCMonitor;
};

#endif
