/*
    microdialog.hpp
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

#ifndef __MICRODIALOG_HPP__
#define __MICRODIALOG_HPP__

#include <e32base.h>
#include <w32std.h>
#include <apgwgnam.h>

const TInt KScreenWidth=176;
const TInt KScreenHeight=208;

class MMicroDialogInterface
{
  public:
    virtual void MicroDialogRefresh(void)=0;
    virtual void MicroDialogClose(void)=0;
    virtual CBitmapContext& Context(void)=0;
    virtual CFont& Font(void)=0;
};

class MMicroDialogCallback
{
  public:
    virtual void MicroDialogStart(MMicroDialogInterface* anInterface)=0;
    virtual void MicroDialogFinish(void)=0;
    virtual void MicroDialogTimer(void)=0;
    virtual void MicroDialogKey(const TKeyEvent& aKey)=0;
    virtual void MicroDialogRequest(void)=0;
};

class CMicroDialog: public CBase
{
  public:
    static void RunLD(MMicroDialogCallback* aCallback,const TSize& aSize,TInt anId);
  protected:
    ~CMicroDialog();
    CMicroDialog(MMicroDialogCallback* aCallback,const TSize& aSize,TInt anId);
    void ConstructL(void);
  private:
    static TInt MicroDialogThreadProc(TAny *aParam);
    void MicroDialogThreadProcL(void);
  private:
    TRequestStatus iStatus;
    RThread iThread;
    // устанавливаются в конструкторе и используются в создаваемом потоке
    MMicroDialogCallback* iCallback;
    TSize iSize;
    TInt iId;
  private:
    class CMicroDialogInt: public CBase,public MMicroDialogInterface
    {
      public:
        ~CMicroDialogInt();
        CMicroDialogInt(CMicroDialog* anOwner);
        void ConstructL(void);
        void Open(void);
        void Close(void);
        void EventLoopL(void);
      private: //MMicroDialogInterface
        void MicroDialogRefresh(void);
        void MicroDialogClose(void);
        CBitmapContext& Context(void);
        CFont& Font(void);
      private:
        TUint32 Handle(void);
      private:
        CMicroDialog* iOwner;
        TUint32 iHandle;
        RWsSession iWsSession;
        RWindowGroup iRootWin;
        RBackedUpWindow iWindow;
        CFbsBitmap* iBitmap;
        CFbsBitmapDevice* iBitmapDevice;
        CBitmapContext* iBitmapContext;
        CFbsFont* iFont;
        RTimer iTimer;
        TBool iDoLoop;
    };
  friend class CMicroDialogInt;
};

#endif
