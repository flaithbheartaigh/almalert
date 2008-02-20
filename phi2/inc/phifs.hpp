/*
    phifs.hpp
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

#ifndef __PHIFS_HPP__
#define __PHIFS_HPP__

#include <badesca.h>
#include <f32file.h>
#include "phientry.hpp"
#include "phipckg.hpp"

class MPhiPaneInterface;
class CPhiSelection;
class CPhiListBox;
class CPhiRefresh;
class CPhiFs: public CBase
{
  public:
    static CPhiFs* NewL(MPhiPaneInterface* anInterface,CPhiListBox* aListBox,CDesCArrayFlat* aFiles);
    ~CPhiFs();
  public:
    void FolderUpL(void);
    void FolderDownL(void);
    void UpdateFileL(void);
    const TFileName CurrentFolder(void) const;
    void CopyL(void);
    void CutL(void);
    void PasteL(void);
    void DeleteL(void);
    void StartItemL(void);
    void NewFolderL(void);
    void RenameL(void);
    void RefreshL(void);
    void AddToPinboardL(void);
    void ShowInfoL(void);
    void SendL(void);
    void SetAttrL(void);
  public: //properties
    void SetSortModeL(TInt aMode);
    TInt SortMode(void);
  private:
    CPhiFs(MPhiPaneInterface* anInterface,CPhiListBox* aListBox,CDesCArrayFlat* aFiles);
    void ConstructL(void);
  public:
    enum TWhere
    {
      ERoot,
      EFiles,
      EObjRoot,
#define READ_OBJ_L(name) EObj##name,
#include "phifs.inl"
#undef READ_OBJ_L(name)
    };
  private:
    struct SPhiEntry
    {
      const TText* iName;
      CPhiFs::TWhere iWhere;
    };
  private:
    static const SPhiEntry KObjects[];
  private:
    void SetFolderL(const TDesC& aFolder);
    static TInt RefreshCallBack(TAny* aPhiFs);
    void UpdateWhere(void);
    inline TWhere Where(void) {return iWhere;};
    const TDesC& Up(void) const;
    TBool EditNameL(TInt aLabel,TDes& aValue);
    static void FormatAtt(TInt anAtt,TDes& aResult);
    static void AppendInfoResource(TDes& aTarget,TInt aResourceID);
    static void AppendInfoSize(TDes& aTarget,TInt aResourceID,TInt aSize);
  private: //ReadListL data
    TInt iPos;
    const TDesC* iPosStr;
    TPckgBuf16<TPhiEntry> iBuffer;
  private:
    void ReadListL(const TDesC* aPos=NULL);
    void ReadPos(const TDesC& aName);
    void ReadUpL(void);
    void ReadFilesL(void);
    void ReadRootL(void);
    void ReadObjRootL(void);
#define READ_OBJ_L(name) void Read##name##L(void);
#include "phifs.inl"
#undef READ_OBJ_L(name)
  private:
    void KillProcessesL(void);
    void KillThreadsL(void);
  private:
    MPhiPaneInterface* iInterface; //not owned
    CPhiListBox* iListBox; //not owned
    CDesCArrayFlat* iFiles; //not owned
    RFs iFs;
    TFileName iFileValue;
    CPhiSelection* iSelection;
    TWhere iWhere;
    TInt iSortMode;
};

#define PHIFSChunkSize

#endif
