/*
    FixQD.hpp
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

#ifndef __FIXQD_HPP__
#define __FIXQD_HPP__

#include <apmrec.h>

#define KUidRecogValue 0xa0000576

class CRecog: public CApaDataRecognizerType
{
  public:
    CRecog();
    TUint PreferredBufSize();
    TDataType SupportedDataTypeL(TInt aIndex) const;
  private:
    void DoRecognizeL(TDesC& aName,const TDesC8& aBuffer);
  public:
    static void StartThread(void);
};

#include <CommonEngine.hpp>

class CLoadNotifier: public CBase,public MSharedDataNotifyHandler
{
  public:
    static CLoadNotifier* NewLC(void);
    ~CLoadNotifier();
    void Wait(void);
  public:
    void HandleNotifyL(TUid anUid,const TDesC16& aKey,const TDesC16& aValue);
  private:
    CLoadNotifier();
    void ConstructL(void);
    void PatchL(void);
  private:
    RSharedDataClient iSysAp;
};

#endif
