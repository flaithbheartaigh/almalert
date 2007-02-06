/*
    ekern_dchunk.cpp
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

class TProcessMemoryInfo; //don't exists in symbian 6.1
#include <k32std.h>

DChunk::DChunk(): CObject(),iHeapOffset(-1)
{
}

void DChunk::Create(CObject* anOwner,DProcess* anOwningProcess,const TDesC* aName,TInt aMaxSize,TChunkType aChunkType)
{
  SetNameL(aName);
  SetOwner(anOwner);
  DoCreate(aMaxSize,aChunkType);
}

void DChunk::Close(void)
{
  K::SvProcess->RemoveChunk(this);
  CObject::Close();
}

DChunk::~DChunk()
{
  delete iDlls;
}

EXPORT_C DChunk* Kern::ChunkFromHandle(TInt aHandle)
{
  return (DChunk*)K::ObjectFromHandle(aHandle,NULL,K::Chunks);
}

DChunk& S::ChunkFromHandleL(TInt aHandle)
{
  return *(DChunk*)S::ObjectFromHandleL(aHandle,K::SvThread,K::Chunks);
}

DChunk* S::ChunkFromHandle(TInt aHandle) //FIXME: NOT IMPLEMENTED
{
  return NULL;
}

void S::ChunkCreateL(TInt &aHandle,DProcess *anOwningProcess,DThread* anOwningThread,const TDesC *aName,TInt aMaxSize,DChunk * &aChunk,TOwnerType aType,TChunkType aChunkType) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C TInt S::ChunkCreate(TInt &aHandle,DProcess *anOwningProcess,DThread* anOwningThread,const TDesC *aName,TPckgBuf<TChunkCreate> &aChunkCreateInfo,TOwnerType aType) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

void S::ChunkCreateDoubleEndedL(TInt &aHandle,DProcess *anOwningProcess,DThread* anOwningThread,const TDesC *aName,TInt aMaxSize,DChunk * &aChunk,TOwnerType aType,TChunkType aChunkType) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C TInt S::ChunkCreateDoubleEnded(TInt& aHandle,DProcess* anOwningProcess,DThread* anOwningThread,const TDesC* aName,TPckgBuf<TDoubleEndedChunkCreate>& aChunkCreateInfo,TOwnerType aType) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

TInt S::ChunkOpen(TInt& aHandle,DProcess* anOwningProcess,DThread* anOwningThread,const TDesC& aName,TBool aIsReadOnly,TOwnerType aType) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

TInt S::ChunkCompressAll(void) //FIXME: NOT IMPLEMENTED
{
  return KErrNone;
}

EXPORT_C TInt Kern::ChunkFind(TInt &aFindHandle,const TDesC &aMatch,TFullName &aName)
{
  return K::Chunks->FindByFullName(aFindHandle,aMatch,aName);
}

EXPORT_C void DChunk::Read(TInt aPos,TDes8* aDes,TInt aMode) const
{
  TInt size=Size();
  if(aPos>=size||aPos<0)
  {
    K::PanicCurrentThread(6);
  }
  else
  {
    TInt len=aDes->Length();
    if(aMode<0) len*=2;
    TInt len2=((size-aPos)>=len)?len:(size-aPos);
    if(aMode<0)
    {
      if(len2&1) len2&=~1;
    }
    Read(aPos,(TUint8*)aDes->Ptr(),len2);
  }
}

EXPORT_C void DChunk::Write(TInt aPos,const TDesC8* aDes,TInt aMode)
{
  TInt size=Size(),len=aDes->Length(),len2=len*2;
  if(aMode<0) len=len2;
  if(aPos>=size||aPos<0||(aPos+len)>size)
  {
    K::PanicCurrentThread(7);
  }
  else
  {
    Write(aPos,aDes->Ptr(),len);
  }
}
