/*
    phiscan.hpp
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

#ifndef __PHISCAN_HPP__
#define __PHISCAN_HPP__

#include "phiselection.hpp"
#include <f32file.h>

class CPhiScanner: public CBase
{
  public:
    static CPhiScanner* NewLC(const CPhiSelection* aSelection,TRequestStatus& aStatus);
    static CPhiScanner* NewSyncroLC(const TDesC& aDir);
    ~CPhiScanner();
    void Cancel(void);
    inline TInt Size(void) {return iSize;};
    inline TInt Count(void) {return iCount;};
  private:
    CPhiScanner(const CPhiSelection* aSelection,TRequestStatus& aStatus);
    CPhiScanner();
    void ConstructL(void);
    void ScanDirL(const TDesC& aDir);
    static TInt ThreadFunction(TAny* aPtr);
    void ThreadFunctionL(void);
  private:
    const CPhiSelection* iSelection;
    TRequestStatus& iStatus;
    TInt iSize;
    TInt iCount;
    RFs iFs;
    RThread iThread;
    TBool iThreadStarted;
};

#endif
