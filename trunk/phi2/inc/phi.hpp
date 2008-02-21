/*
    phi.hpp
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

#ifndef __PHI_HPP__
#define __PHI_HPP__

#include <aknapp.h>
#include <akndoc.h>
#include <aknviewappui.h>

class CPhiApplication: public CAknApplication
{
  public: //CAknApplication
    TUid AppDllUid() const;
  protected: //CAknApplication
    CApaDocument* CreateDocumentL();
};

class CPhiDocument: public CAknDocument
{
  public:
    static CPhiDocument* NewL(CEikApplication& aApp);
  public: //from CAknDocument
    CEikAppUi* CreateAppUiL();
  private:
    CPhiDocument(CEikApplication& aApp);
};

class CPhiAppUi: public CAknViewAppUi
{
  public:
    void CmdShowImageL(const TDesC& aFileName);
    void CmdBackL(void);
  private: //CAknAppUi
    void ConstructL(void);
    void HandleCommandL(TInt aCommand);
};

#define iPhiViewAppUi ((CPhiAppUi*)CEikonEnv::Static()->EikAppUi())

#endif
