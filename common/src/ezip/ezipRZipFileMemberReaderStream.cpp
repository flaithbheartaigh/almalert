/*
    ezipRZipFileMemberReaderStream.cpp
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

#include <zipfile.h>

RZipFileMemberReaderStream::RZipFileMemberReaderStream(CZipFile& aZipFile,TUint32 aDataOffset,TUint32 aCompressedSize,TUint32 aCompressionMethod): iZipFile(aZipFile),iCompressionMethod(aCompressionMethod),iCompressedSize(aCompressedSize),iFileOffset(aDataOffset),iOffset(0),iDone(EFalse),iBytesLength(0),iBytesOffset(0),iCompressedBytesOffset(0)
{
  iStream.next_in=0;
  iStream.avail_in=0;
  iStream.total_in=0;
  iStream.next_out=0;
  iStream.avail_out=0;
  iStream.total_out=0;
  iStream.msg=0;
  iStream.state=0;
  iStream.zalloc=0;
  iStream.zfree=0;
  iStream.opaque=0;
  iStream.data_type=0;
  iStream.adler=0;
  iStream.reserved=0;
  inflateInit2(&iStream,-MAX_WBITS);
}

EXPORT_C RZipFileMemberReaderStream::~RZipFileMemberReaderStream()
{
  inflateEnd(&iStream);
}

EXPORT_C TInt RZipFileMemberReaderStream::Read(TDes16& aDes,TInt aLength)
{
  TUint32 numBytesRead=0;
  TInt err=Read(CONST_CAST(TByte*,(const TByte*)aDes.Ptr()),2*aLength,&numBytesRead);
  if(err==-1)
  {
    aDes.SetLength(0);
    return KErrCorrupt;
  }
  else
  {
    if(err==-2)
    {
      aDes.SetLength(numBytesRead>>2);
      return KErrEof;
    }
  }
  aDes.SetLength(numBytesRead>>2);
  return KErrNone;
}

TInt RZipFileMemberReaderStream::Read(void)
{
  TByte b;
  TUint32 numRead=0;
  if(Read(&b,1,&numRead)==1)
  {
    return b;
  }
  else
  {
    return -1;
  }
}

TInt RZipFileMemberReaderStream::Read(TDes8& aDes,TInt aLength)
{
  TUint32 numBytesRead=0;
  TInt err=Read(CONST_CAST(TByte*,aDes.Ptr()),aLength,&numBytesRead);
  if(err==-1)
  {
    aDes.SetLength(0);
    return KErrCorrupt;
  }
  else
  {
    if(err==-2)
    {
      aDes.SetLength(numBytesRead);
      return KErrEof;
    }
  }
  aDes.SetLength(numBytesRead);
  return KErrNone;
}

void RZipFileMemberReaderStream::ReadL(TDes16& aDes,TInt aLength)
{
  User::LeaveIfError(Read(aDes,aLength));
}

void RZipFileMemberReaderStream::Release(void)
{
}

void RZipFileMemberReaderStream::Close(void)
{
}


TInt RZipFileMemberReaderStream::Read(TByte* aBytes,TUint32 aLength,TUint32* aRetLength)
{
  if(iCompressionMethod==CZipArchive::EDeflated)
  {
    return GetBytes(aBytes,aLength,aRetLength);
  }
  else
  {
    return GetStoredBytes(aBytes,aLength,aRetLength);
  }
}

TInt RZipFileMemberReaderStream::GetBytes(TByte* aBytes,TUint32 aLength,TUint32* aRetLength)
{
  TUint32 bytesLeftToRead;
  TUint32 bytesOffset;
  bytesOffset=0;
  bytesLeftToRead=aLength;
  while(bytesLeftToRead>0)
  {
    TUint32 bytesAvailable;
    TUint32 bytesToCopy;
    bytesAvailable=iBytesLength-iBytesOffset;
    while(bytesAvailable==0)
    {
      switch(GetBytes())
      {
        case KErrNone:
          break;
        case -2:
          *aRetLength=bytesOffset;
          return -2;
        default:
          return -1;
      }
      bytesAvailable=iBytesLength-iBytesOffset;
    }
    if(bytesAvailable>bytesLeftToRead)
    {
      bytesToCopy=bytesLeftToRead;
    }
    else
    {
      bytesToCopy=bytesAvailable;
    }
    for(TUint32 i=0;i<bytesToCopy;i++)
    {
      aBytes[bytesOffset++]=iBytes[iBytesOffset++];
    }
    bytesLeftToRead-=bytesToCopy;
  }
  *aRetLength=bytesOffset;
  return *aRetLength;
}

TInt RZipFileMemberReaderStream::GetBytes(void)
{
  TInt status;
  if(iStream.avail_in==0)
  {
    if(GetCompressedBytes()!=KErrNone)
    {
      return -1;
    }
  }
  iStream.next_out=iBytes;
  iStream.avail_out=sizeof(iBytes);
  status=inflate(&iStream,Z_SYNC_FLUSH);
  iBytesOffset=0;
  iBytesLength=sizeof(iBytes)-iStream.avail_out;
  switch(status)
  {
    case Z_OK:
      return KErrNone;
    case Z_STREAM_END:
      return (iBytesLength>0)?KErrNone:-2;
    default:
      return -1;
  }
}

TInt RZipFileMemberReaderStream::GetCompressedBytes(void)
{
  if(iOffset<iCompressedSize)
  {
    TUint32 nBytesLeft;
    TUint32 nBytesToRead;
    nBytesLeft=iCompressedSize-iOffset;
    nBytesToRead=(nBytesLeft>sizeof(iCompressedBytes))?sizeof(iCompressedBytes):nBytesLeft;
    if(iZipFile.Seek(iFileOffset)!=KErrNone)
    {
      return -1;
    }
    else
    {
      if(iZipFile.Read(iCompressedBytes,nBytesToRead)!=KErrNone)
      {
        return -1;
      }
    }
    iFileOffset+=nBytesToRead;
    iOffset+=nBytesToRead;
    iStream.next_in=iCompressedBytes;
    iStream.avail_in=nBytesToRead;
    return KErrNone;
  }
  else
  {
    if(iDone==EFalse)
    {
      iCompressedBytes[0]=0;
      iStream.avail_in=1;
      iStream.next_in=iCompressedBytes;
      iDone=ETrue;
      return KErrNone;
    }
    else
    {
      return -1;
    }
  }
}

TInt RZipFileMemberReaderStream::GetStoredBytes(TByte* aBytes,TUint32 aLength,TUint32* aRetLength)
{
  TInt status;
  if((iOffset+aLength)>iCompressedSize)
  {
    aLength=iCompressedSize-iOffset;
    if(aLength<=0)
    {
      return -1;
    }
  }
  if(iZipFile.Seek(iFileOffset)==-1)
  {
    return -1;
  }
  status=iZipFile.Read(aBytes,aLength);
  if(status==KErrNone)
  {
    iFileOffset+=aLength;
    iOffset+=aLength;
    *aRetLength=aLength;
  }
  return status;
}
