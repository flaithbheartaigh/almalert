/*
    AknNotifyPlugin.hpp
    Copyright (C) 2005 zg

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

#ifndef __AKNNOTIFYPLUGIN_HPP__
#define __AKNNOTIFYPLUGIN_HPP__

#include <e32base.h>
#include <aknnotedialog.h>
#include <vwsdef.h>

class MNotifierDialogObserver
{
  public:
    virtual void NoteCompleted(TInt aNoteId,TInt aCommand)=0;
    virtual TBool DisplayDialogL(TInt aPriority)=0;
    virtual TBool CancelDialog(TInt aPriority)=0;
};

void NotifierArray(void); //FIXME: incompleted

class CNotifierDialogController //FIXME: incompleted
{
  public:
    void CancelNote(TInt aParam);
    void DisplayNonNoteDialogL(TInt aParam);
    TInt LaunchNoteL(TInt aParam1,TInt aParam2,TInt aParam3);
    TInt LaunchNoteL(TInt aParam1,TInt aParam2,TInt aParam3,CAknNoteDialog::TTimeout aTimeout,CAknNoteDialog::TTone aTone);
    void SetNoteObserver(MNotifierDialogObserver* anObserver);
};

class CAknSoftNotificationParameters //FIXME: incompleted
{
  public:
    CAknSoftNotificationParameters::~CAknSoftNotificationParameters(void);
    CAknSoftNotificationParameters* CAknSoftNotificationParameters::NewL(void);
    CAknSoftNotificationParameters* CAknSoftNotificationParameters::NewL(TDesC16 const& aParam,TInt aParam1,TInt aParam2,TInt aParam3,CAknNoteDialog::TTone aTone);
    CAknSoftNotificationParameters* CAknSoftNotificationParameters::NewL(TDesC16 const& aParam,TInt aParam1,TInt aParam2,TInt aParam3,CAknNoteDialog::TTone aTone,TVwsViewId aViewId,TUid anUid,TInt aParam4,TDesC8 const& aStr);
};

#endif
