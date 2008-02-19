/*
    phifs_list.cpp
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

#include "phifs.hpp"
#include "philistbox.hpp"
#include "phiconst.hpp"

_LIT(KObjectsRoot,"\x3c9:");

const CPhiFs::SPhiEntry CPhiFs::KObjects[]=
{
#define READ_OBJ_L(name) {_S(#name),CPhiFs::EObj##name},
#include "phifs.inl"
#undef READ_OBJ_L(name)
};

void CPhiFs::UpdateWhere(void)
{
  TPtrC path=CurrentFolder();
  if(path.Length())
  {
    iWhere=EFiles;
    if(path.Left(2).CompareF(KObjectsRoot)==0)
    {
      iWhere=EObjRoot;
      if(path.Length()>3)
      {
        TPtrC ptr=path.Mid(3);
        TInt pos=ptr.Locate('\\');
        TPtrC folder(pos>=0?ptr.Left(pos):ptr);
        for(TUint ii=0;ii<sizeofa(KObjects);ii++)
        {
          TPtrC target((const TUint16*)KObjects[ii].iName);
          if(folder.CompareF(target)==0)
          {
            iWhere=KObjects[ii].iWhere;
            break;
          }
        }
      }
    }
  }
  else
  {
    iWhere=ERoot;
  }
}

void CPhiFs::ReadListL(const TDesC* aPos)
{
  iPosStr=aPos;
  iPos=-1;
  iFiles->Reset();
  iListBox->Reset();
  if(Where()!=ERoot) ReadUpL();
  switch(Where())
  {
    case ERoot:
      ReadRootL();
      break;
    case EFiles:
      ReadFilesL();
      break;
    case EObjRoot:
      ReadObjRootL();
      break;
#define READ_OBJ_L(name) case EObj##name: Read##name##L(); break;
#include "phifs.inl"
#undef READ_OBJ_L(name)
    default:
      break;
  }
  iListBox->HandleItemAdditionL();
  if(iPos!=-1) iListBox->SetCurrentItemIndexAndDraw(iPos);
  UpdateFileL();
}

void CPhiFs::ReadPos(const TDesC& aName)
{
  if(iPosStr&&aName.CompareF(*iPosStr)==0) iPos=iFiles->Count();
}

void CPhiFs::ReadUpL(void)
{
  iBuffer().Clear();
  iBuffer().iName.Copy(Up());
  iBuffer().iAtt=KEntryAttDir;
  iFiles->AppendL(iBuffer);
}

void CPhiFs::ReadFilesL(void)
{
  TParse parse;
  parse.SetNoWild(iFileValue,NULL,NULL);
  CDir* list;
  User::LeaveIfError(iFs.GetDir(parse.DriveAndPath(),KEntryAttMaskSupported,iSortMode|EDirsFirst,list));
  CleanupStack::PushL(list);
  for(TInt i=0;i<list->Count();i++)
  {
    iBuffer().Clear();
    const TDesC& name=(*list)[i].iName;
    iBuffer().iName.Copy(name);
    ReadPos(name);
    iBuffer().iAtt=(*list)[i].iAtt;
    iBuffer().iSize=(*list)[i].iSize;
    iBuffer().iModified=(*list)[i].iModified;
    iFiles->AppendL(iBuffer);
  }
  CleanupStack::PopAndDestroy(); //list
}

void CPhiFs::ReadRootL(void)
{
  TDriveList drives;
  User::LeaveIfError(iFs.DriveList(drives));
  for(TInt drive=EDriveA;drive<=EDriveZ;drive++)
  {
    if(drives[drive])
    {
      iBuffer().Clear();
      iBuffer().iName.Append(drive+'a');
      iBuffer().iName.Append(':');
      ReadPos(iBuffer().iName);
      iBuffer().iAtt=KEntryAttDir|KEntryAttVolume;
      iFiles->AppendL(iBuffer);
    }
  }
  iBuffer().Clear();
  iBuffer().iName.Copy(KObjectsRoot);
  ReadPos(KObjectsRoot);
  iBuffer().iAtt=KEntryAttDir;
  iFiles->AppendL(iBuffer);
}

void CPhiFs::ReadObjRootL(void)
{
  for(TUint ii=0;ii<sizeofa(KObjects);ii++)
  {
    iBuffer().Clear();
    TPtrC name((const TUint16*)KObjects[ii].iName);
    iBuffer().iName.Copy(name);
    ReadPos(name);
    iBuffer().iAtt=KEntryAttDir;
    iFiles->AppendL(iBuffer);
  }
}

#define READ_OBJ_L(name) void CPhiFs::Read##name##L(void) { TFullName name; TFind##name find; while(find.Next(name)==KErrNone) { iBuffer().Clear(); READ_OBJ_L_THUNK_01 iBuffer().iName.Copy(name); ReadPos(name); iFiles->AppendL(iBuffer); } }
#include "phifs.inl"
#undef READ_OBJ_L(name)
