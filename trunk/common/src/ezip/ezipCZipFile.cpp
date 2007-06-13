/*
    ezipCZipFile.cpp
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

#include <utf.h>
#include <zipfile.h>

EXPORT_C CZipFile* CZipFile::NewL(RFs& aFs,const TDesC& aFileName)
{
  CZipFile* self = new(ELeave)CZipFile(aFs, aFileName);
  CleanupStack::PushL(self);
  self->ConstructL(aFileName);
  CleanupStack::Pop(); //self
  return self;
}

EXPORT_C CZipFile::~CZipFile()
{
  DeleteMemberPointers();
}

EXPORT_C CZipFile::CZipFile(RFs& aFs,const TDesC& aFileName): iFileName(aFileName),iFs(aFs)
{
}

EXPORT_C TInt CZipFile::OpenL(void)
{
  if(!iMemberPointers)
  {
    ConstructL(iFileName);
  }
  return KErrNone;
}

EXPORT_C void CZipFile::Close(void)
{
  DeleteMemberPointers();
}

EXPORT_C void CZipFile::ConstructL(const TDesC& aFileName)
{
  TInt err;
  TUint32 offset;
  TRAP(err,OpenFileL(aFileName));
  if(err)
  {
    User::Leave(KZipArchiveError);
  }
  else
  {
    if((err=FindCentralDirectoryTrailer(offset))!=KErrNone)
    {
      DeleteMemberPointers();
      User::Leave(err);
    }
    else
    {
      if((err=ReadCentralDirectoryTrailer(offset,iTrailer))!=KErrNone)
      {
        DeleteMemberPointers();
        User::Leave(err);
      }
      else
      {
        if(iTrailer.iStartDiskNumber!=iTrailer.iDiskNumber)
        {
          DeleteMemberPointers();
          User::Leave(KMultiDiskArchivesNotSupported);
        }
        else
        {
          if((iTrailer.iOffset>iFileLength)||((iTrailer.iOffset+iTrailer.iSize)>iFileLength))
          {
            DeleteMemberPointers();
            User::Leave(KCentralDirectoryTrailerInvalid);
          }
          else
          {
            if(LoadMemberPointersL()!=KErrNone)
            {
              User::Leave(KZipFileIOError);
            }
          }
        }
      }
    }
  }
}

EXPORT_C TInt CZipFile::Size(TInt& aSize) const
{
  TInt err=KErrNotReady;
  if(iDataOpened)
  {
    err=iData.Size(aSize);
  }
  return err;
}


EXPORT_C CZipFileMember* CZipFile::MemberL(const TDesC& aName)
{
  TLocalHeader header;
  const TMemberPointer* memberPointer;
  HBufC* localName=aName.AllocL();
  TInt loop=0;
  while(loop<localName->Length())
  {
    if((*localName)[loop]=='\\')
    {
      (localName->Des())[loop]='/';
    }
    loop++;
  }
  memberPointer=FindMemberPointer(*localName,EFalse);
  if(memberPointer==NULL)
  {
    delete localName;
    return NULL;
  }
  else
  {
    if(ReadLocalHeader(memberPointer->iLocalHeaderOffset,header)!=KErrNone)
    {
      delete localName;
      return NULL;
    }
    else
    {
      CleanupStack::PushL(localName);
      CZipFileMember* thisMember=MakeMemberL(*memberPointer,header);
      CleanupStack::PopAndDestroy(); //localName
      return thisMember;
    }
  }
}

EXPORT_C CZipFileMember* CZipFile::CaseInsensitiveMemberL(const TDesC& aName)
{
  TLocalHeader header;
  const TMemberPointer* memberPointer;
  HBufC* localName=aName.AllocL();
  TInt loop=0;
  while(loop<localName->Length())
  {
    if((*localName)[loop]=='\\')
    {
      (localName->Des())[loop]='/';
    }
    loop++;
  }

  memberPointer=FindMemberPointer(*localName, ETrue);
  if(memberPointer==NULL)
  {
    delete localName;
    return NULL;
  }
  else
  {
    if(ReadLocalHeader(memberPointer->iLocalHeaderOffset,header)!=KErrNone)
    {
      delete localName;
      return NULL;
    }
    else
    {
      CleanupStack::PushL(localName);
      CZipFileMember* thisMember=MakeMemberL(*memberPointer,header);
      CleanupStack::PopAndDestroy(); //localName
      return thisMember;
    }
  }
}

EXPORT_C CZipFileMember* CZipFile::CaseSensitiveOrCaseInsensitiveMemberL(const TDesC& aName)
{
  CZipFileMember* member;
  member=MemberL(aName);
  if(member)
  {
    return member;
  }
  else
  {
    return CaseInsensitiveMemberL(aName);
  }
}

EXPORT_C TInt CZipFile::GetInputStreamL(const CZipFileMember* aMember, RZipFileMemberReaderStream*& aStream)
{
  TUint32 compressionMethod;
  compressionMethod=aMember->iCompressionMethod;
  if((compressionMethod!=EStored)&&(compressionMethod!=EDeflated))
  {
    return KCompressionMethodNotSupported;
  }
  aStream=new(ELeave)RZipFileMemberReaderStream(*this,aMember->iDataOffset,aMember->iCompressedSize,compressionMethod);
  return KErrNone;
}


EXPORT_C CZipFileMemberIterator* CZipFile::GetMembersL()
{
  return new(ELeave)CZipFileMemberIterator(this);
}

TInt CZipFile::FindCentralDirectoryTrailer(TUint32& offset)
{
  TBuf8<KSignatureLength> signature;
  if(iFileLength<=KCentralDirectoryTrailerFixedLength)
  {
    return KZipArchiveError;
  }
  offset=iFileLength-KCentralDirectoryTrailerFixedLength;
  if(Seek(offset)!=KErrNone)
  {
    return KZipFileIOError;
  }
  TInt err=iData.Read(signature);
  if(err!=KErrNone)
  {
    return KZipFileIOError;
  }

  if((signature[0]==0x50)&&(signature[1]==0x4b)&&(signature[2]==0x05)&&(signature[3]==0x06))
  {
    return KErrNone;
  }
  else
  {
    TUint32 initialOffset=offset;
    while(offset>=initialOffset-KMaxTrailerSearchLength)
    {
      if(Seek(offset)!=KErrNone)
      {
        return KZipFileIOError;
      }
      if(iData.Read(signature)!=KErrNone)
      {
        return KZipFileIOError;
      }
      if((signature[0]==0x50)&&(signature[1]==0x4b)&&(signature[2]==0x05)&&(signature[3] == 0x06))
      {
        return KErrNone;
      }
      --offset;
    }
    return KCentralDirectoryTrailerNotFound;
  }
}

TInt CZipFile::ReadCentralDirectoryTrailer(TUint32 offset,struct TCentralDirectoryTrailer& r)
{
  if(Seek(offset+KSignatureLength)!=KErrNone)
  {
    return KZipFileIOError;
  }
  else
  {
    if(Read(r.iDiskNumber)!=KErrNone)
    {
      return KZipFileIOError;
    }
    else
    {
      if(Read(r.iStartDiskNumber)!=KErrNone)
      {
        return KZipFileIOError;
      }
      else
      {
        if(Read(r.iLocalEntryCount)!=KErrNone)
        {
          return KZipFileIOError;
        }
        else
        {
          if(Read(r.iTotalEntryCount)!=KErrNone)
          {
            return KZipFileIOError;
          }
          else
          {
            if(Read(r.iSize)!=KErrNone)
            {
              return KZipFileIOError;
            }
            else
            {
              if(Read(r.iOffset)!=KErrNone)
              {
                return KZipFileIOError;
              }
              else
              {
                return KErrNone;
              }
            }
          }
        }
      }
    }
  }
}

TInt CZipFile::LoadMemberPointersL(void)
{
  TCentralDirectoryHeader header;
  TUint32 i;
  TUint32 memberPointerCount;

  if(Seek(iTrailer.iOffset)!=KErrNone)
  {
    return KZipFileIOError;
  }
  memberPointerCount=iTrailer.iTotalEntryCount;
  iMemberPointers=new(ELeave)TMemberPointer[memberPointerCount];
  for(i=0;i<memberPointerCount;i++)
  {
    iMemberPointers[i].iName=NULL;
  }
  for(i=0;i<memberPointerCount;i++)
  {
    if(ReadCentralDirectoryHeaderL(header,iMemberPointers[i])!=KErrNone)
    {
      return KZipFileError;
    }
  }
  return KErrNone;
}

TInt CZipFile::ReadCentralDirectoryHeaderL(TCentralDirectoryHeader& aHeader,TMemberPointer& aMemberPointer)
{
  if (Read(aHeader.iSignature) != KErrNone)
  {
    return KZipFileIOError;
  }
  else
  {
    if(aHeader.iSignature!=KCentralDirectoryHeaderSignature)
    {
      return KZipFileIOError;
    }
    else
    {
      if(Read(aHeader.iMadeBy)!=KErrNone)
      {
        return KZipFileIOError;
      }
      else
      {
        if(Read(aHeader.iRequired)!=KErrNone)
        {
          return KZipFileIOError;
        }
      }
    }
  }
  if(Read(aHeader.iFlags)!=KErrNone)
  {
    return KZipFileIOError;
  }
  else
  {
    if(Read(aHeader.iCompressionMethod)!=KErrNone)
    {
      return KZipFileIOError;
    }
    else
    {
      if(Read(aHeader.iLastModifiedFileTime)!=KErrNone)
      {
        return KZipFileIOError;
      }
      else
      {
        if(Read(aHeader.iLastModifiedFileDate)!=KErrNone)
        {
          return KZipFileIOError;
        }
        else
        {
          if(Read(aHeader.iCRC32)!=KErrNone)
          {
            return KZipFileIOError;
          }
          else
          {
            if(Read(aHeader.iCompressedSize)!=KErrNone)
            {
              return KZipFileIOError;
            }
            else
            {
              if(Read(aHeader.iUncompressedSize)!=KErrNone)
              {
                return KZipFileIOError;
              }
              else
              {
                if(Read(aHeader.iFileNameLength)!=KErrNone)
                {
                  return KZipFileIOError;
                }
                else
                {
                  if(Read(aHeader.iExtraFieldLength)!=KErrNone)
                  {
                    return KZipFileIOError;
                  }
                  else
                  {
                    if(Read(aHeader.iFileCommentLength)!=KErrNone)
                    {
                      return KZipFileIOError;
                    }
                    else
                    {
                      if(Read(aHeader.iDiskNumberStart)!=KErrNone)
                      {
                        return KZipFileIOError;
                      }
                      else
                      {
                        if(Read(aHeader.iInternalFileAttributes)!=KErrNone)
                        {
                          return KZipFileIOError;
                        }
                        else
                        {
                          if(Read(aHeader.iExternalFileAttributes)!=KErrNone)
                          {
                            return KZipFileIOError;
                          }
                          else
                          {
                            if(Read(aHeader.iLocalHeaderOffset)!=KErrNone)
                            {
                              return KZipFileIOError;
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }


  aMemberPointer.iCRC32=aHeader.iCRC32;
  aMemberPointer.iCompressedSize=aHeader.iCompressedSize;
  aMemberPointer.iUncompressedSize=aHeader.iUncompressedSize;
  aMemberPointer.iLocalHeaderOffset=aHeader.iLocalHeaderOffset;
  aMemberPointer.iName=new(ELeave)TFileName;

  TBuf8<KMaxFileName> input;
  if(iData.Read(input,aHeader.iFileNameLength)!=KErrNone)
  {
    return KZipFileIOError;
  }
  CnvUtfConverter::ConvertToUnicodeFromUtf8(*aMemberPointer.iName,input);

  if(aHeader.iExtraFieldLength!=0)
  {
    for(TUint32 i=0;i<aHeader.iExtraFieldLength;i++)
    {
      TByte b;
      if(Read(&b,1)==-1)
      {
        return -1;
      }
    }
  }
  if(aHeader.iFileCommentLength!=0)
  {
    for(TUint32 i=0;i<aHeader.iFileCommentLength;i++)
    {
      TByte b;
      if(Read(&b,1)==-1)
      {
        return -1;
      }
    }
  }
  return KErrNone;
}

TInt CZipFile::ReadLocalHeader(TUint32 aOffset,TLocalHeader& aHeader)
{
  if (Seek(aOffset) != KErrNone)
  {
    return KZipFileIOError;
  }
  else
  {
    if(Read(aHeader.iSignature)!=KErrNone)
    {
      return KZipFileIOError;
    }
    else
    {
      if(aHeader.iSignature!=KLocalHeaderSignature)
      {
        return KLocalHeaderSignatureInvalid;
      }
      else
      {
        if(Read(aHeader.iVersionNeeded)!=KErrNone)
        {
          return KZipFileIOError;
        }
        else
        {
          if(Read(aHeader.iFlags)!=KErrNone)
          {
            return KZipFileIOError;
          }
          else
          {
            if(Read(aHeader.iCompressionMethod)!=KErrNone)
            {
              return KZipFileIOError;
            }
            else
            {
              if(Read(aHeader.iLastModifiedFileTime)!=KErrNone)
              {
                return KZipFileIOError;
              }
              else
              {
                if(Read(aHeader.iLastModifiedFileDate)!=KErrNone)
                {
                  return KZipFileIOError;
                }
                else
                {
                  if(Read(aHeader.iCRC32)!=KErrNone)
                  {
                    return KZipFileIOError;
                  }
                  else
                  {
                    if(Read(aHeader.iCompressedSize)!=KErrNone)
                    {
                      return KZipFileIOError;
                    }
                    else
                    {
                      if(Read(aHeader.iUncompressedSize)!=KErrNone)
                      {
                        return KZipFileIOError;
                      }
                      else
                      {
                        if(Read(aHeader.iFileNameLength)!=KErrNone)
                        {
                          return KZipFileIOError;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  if(Read(aHeader.iExtraFieldLength)!=KErrNone)
  {
    return KZipFileIOError;
  }
  else
  {
    return KErrNone;
  }
}

const CZipFile::TMemberPointer* CZipFile::FindMemberPointer(const TDesC& aName, TBool aCaseInsensitive)
{
  for(TUint32 i=0;i<iTrailer.iTotalEntryCount;i++)
  {
    if(aCaseInsensitive&&(!aName.CompareF(*iMemberPointers[i].iName)))
    {
      return iMemberPointers+i;
    }
    else if(aName==*iMemberPointers[i].iName)
    {
      return iMemberPointers+i;
    }
  }
  return NULL;
}

RZipFileMemberReaderStream* CZipFile::MakeInputStreamL(TUint32 aDataOffset,TUint32 aCompressedSize,TUint32 aCompressionMethod)
{
  return new(ELeave)RZipFileMemberReaderStream(*this,aDataOffset,aCompressedSize,aCompressionMethod);
}

CZipFileMember* CZipFile::MakeMemberL(TInt aMemberIndex)
{
  TLocalHeader header;
  TMemberPointer* memberPointer;
  if(aMemberIndex>=iTrailer.iTotalEntryCount)
  {
    return NULL;
  }
  memberPointer=iMemberPointers+aMemberIndex;
  if(ReadLocalHeader(memberPointer->iLocalHeaderOffset,header)!=KErrNone)
  {
    return NULL;
  }
  else
  {
    return MakeMemberL(*memberPointer,header);
  }
}

CZipFileMember* CZipFile::MakeMemberL(const TMemberPointer& aMemberPointer,const TLocalHeader& aHeader)
{
  CZipFileMember* member;
  member=new(ELeave)CZipFileMember;
  CleanupStack::PushL(member);
  member->iCRC32=aMemberPointer.iCRC32;
  member->iCompressedSize=aMemberPointer.iCompressedSize;
  member->iCompressionMethod=aHeader.iCompressionMethod;
  member->iName=new(ELeave)TFileName(*aMemberPointer.iName);
  TInt loop=0;
  while(loop < member->iName->Length())
  {
    if((*member->iName)[loop]=='/')
    {
      (*member->iName)[loop]='\\';
    }
    loop++;
  }
  member->iUncompressedSize=aMemberPointer.iUncompressedSize;
  member->iDataOffset=aMemberPointer.iLocalHeaderOffset+KLocalHeaderFixedLength+aHeader.iFileNameLength+aHeader.iExtraFieldLength;
  CleanupStack::Pop();
  return member;
}

void CZipFile::DeleteMemberPointers(void)
{
  if(iMemberPointers)
  {
    for(TUint32 i=0;i<iTrailer.iTotalEntryCount;i++)
    {
      delete iMemberPointers[i].iName;
    }
    delete[] iMemberPointers;
    iMemberPointers = 0;
  }
  iData.Close();
  iDataOpened=EFalse;
}

void CZipFile::OpenFileL(const TDesC& aFileName)
{
  User::LeaveIfError(iData.Open(iFs,aFileName,EFileShareAny|EFileStream|EFileRead));
  iDataOpened=ETrue;
  TInt length=0;
  User::LeaveIfError(iData.Seek(ESeekEnd,length));
  iFileLength=length;
}

TInt CZipFile::Read(TUint16& aUs)
{
  TPckgBuf<TUint16> temp(aUs);
  if(iData.Read(temp)!=KErrNone)
  {
    return KZipFileIOError;
  }
  aUs=temp();
  return KErrNone;
}

TInt CZipFile::Read(TUint32& aUl)
{
  TPckgBuf<TUint32> temp;
  if(iData.Read(temp)!=KErrNone)
  {
    return KZipFileIOError;
  }
  aUl=temp();
  return KErrNone;
}

TInt CZipFile::Read(TByte* aBytes,TUint32 aLength)
{
  TPtr8 ptr(aBytes,aLength);
  if(iData.Read(ptr,aLength))
  {
    return KZipFileIOError;
  }
  else
  {
    return KErrNone;
  }
}

TInt CZipFile::Seek(TInt aOffset)
{
  if(iData.Seek(ESeekStart,aOffset)<0)
  {
    return KZipFileIOError;
  }
  else
  {
    return KErrNone;
  }
}
