/*
    phifiles.hpp
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

#ifndef __PHIFILES_HPP__
#define __PHIFILES_HPP__

#include <aknview.h>
#include "phifs.hpp"

class CPhiListPane;
class CPhiFilesView: public CAknView
{
  public:
    static CPhiFilesView* NewLC(void);
    ~CPhiFilesView();
  public: //CAknView
    TUid Id(void) const;
    void HandleCommandL(TInt aCommand);
  public: //MEikMenuObserver
    void DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane);
  private: //CAknView
    void DoActivateL(const TVwsViewId& aPrevViewId,TUid aCustomMessageId,const TDesC8& aCustomMessage);
    void DoDeactivate(void);
  private:
    CPhiFilesView();
    void ConstructL(void);
  private:
    CPhiListPane* iControl;
    TFileName iPath;
    CPhiFs::TWhere iWhere;
    TInt iSortMode;
};

const TUid KPhiFileViewId={1};

#define iPhiAppUi ((CPhiAppUi*)AppUi())

#endif
