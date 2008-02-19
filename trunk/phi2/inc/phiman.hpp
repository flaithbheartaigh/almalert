/*
    phiman.hpp
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

#ifndef __PHIMAN_HPP__
#define __PHIMAN_HPP__

#include <aknprogressdialog.h>
#include <f32file.h>

class CPhiSelection;
class CPhiScanner;
class CPhiMan: public CActive,public MProgressDialogCallback,public MFileManObserver
{
  public:
    static void NewLD(CPhiSelection* aSelection,const TDesC& aFolder,TCallBack aCallBack);
    ~CPhiMan();
  public: //MProgressDialogCallback
    void DialogDismissedL(TInt aButtonId);
  protected: //CActive
    void DoCancel(void);
    void RunL(void);
  public: //MFileManObserver
    TControl NotifyFileManStarted(void);
    TControl NotifyFileManOperation(void);
    TControl NotifyFileManEnded(void);
  private:
    CPhiMan(CPhiSelection* aSelection,const TDesC& aFolder,TCallBack aCallBack);
    void ConstructL(void);
    void CleanupL(void);
    void NextL(void);
  private:
    CPhiSelection* iSelection;
    TBool iOwned;
    TFileName iDestination;
    RFs iFs;
    CFileMan* iMan;
    TBool iCancel;
    TInt iCurrent;
    TInt iSubCurrent;
    TInt iSubCurrentMax;
    CPhiScanner* iScanner;
    CAknProgressDialog* iProgressDialog;
    CEikProgressInfo* iProgressInfo;
    RMutex iMutex;
    CPeriodic* iRedrawTimer;
    TInt iAddProgress;
    TInt iAddCount;
    TFileName iCurrentItem;
    TCallBack iCallBack;
    TBuf<64> iOverwrite;
    TInt iError;
  private:
    static TInt RedrawCallBack(TAny* aPhiMan);
    TInt DoRedraw(void);
    void ManWaitL(RMutex& aMutex);
    void ManSignal(RMutex& aMutex);
    void CopyLockedL(void);
    void ClearROL(TBool aTarget);
    void DeleteL(const TDesC& aFileName);
};

#endif
