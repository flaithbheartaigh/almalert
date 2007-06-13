/*
    zipfile.h
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

#ifndef __ZIPFILE_H__
#define __ZIPFILE_H__

#include <e32std.h>
#include <ezlib.h>
#include <s32file.h>

typedef TUint8 TByte;

class CZipFile;

class CZipArchive: public CBase
{
  public:
    enum
    {
      KZipArchiveError=-256,
      KCentralDirectoryTrailerNotFound=KZipArchiveError-1,
      KCentralDirectoryTrailerInvalid=KZipArchiveError-3,
      KCompressionMethodNotSupported=KZipArchiveError-4,
      KLocalHeaderSignatureInvalid=KZipArchiveError-5,
      KMultiDiskArchivesNotSupported=KZipArchiveError-6,
      KMemberNotFound=KZipArchiveError-7,
      KZipArchiveMinError=KZipArchiveError-8
    };
    enum TCompressionMethod
    {
      EStored=0,
      EShrunk=1,
      EReducedCFOne=2,
      EReducedCFTwo=3,
      EReducedCFThree=4,
      EReducedCFFour=5,
      EImploded=6,
      EReservedOne=7,
      EDeflated=8,
      EReservedTwo=9,
      EPKWAREImploded=10
    };
  protected:
    enum
    {
      KCentralDirectorySignature=0x06054b50,
      KCentralDirectoryHeaderSignature=0x02014b50,
      KLocalHeaderSignature=0x04034b50
    };
    enum
    {
      KCentralDirectoryTrailerFixedLength=22,
      KLocalHeaderFixedLength=30,
      KSignatureLength=4,
      KMaxTrailerSearchLength=65536
    };
    enum
    {
      KCentralFileHeaderFileNameLengthOffset=28,
      KCentralFileHeaderExtraFieldLengthOffset=30,
      KCentralFileHeaderFileNameOffset=46
    };
    struct TCentralDirectoryTrailer
    {
      TUint32 iSignature;
      TUint16 iDiskNumber;
      TUint16 iStartDiskNumber;
      TUint16 iLocalEntryCount;
      TUint16 iTotalEntryCount;
      TUint32 iSize;
      TUint32 iOffset;
      TUint16 iCommentLength;
    };
    struct TCentralDirectoryHeader
    {
      TUint32 iSignature;
      TUint16 iMadeBy;
      TUint16 iRequired;
      TUint16 iFlags;
      TUint16 iCompressionMethod;
      TUint16 iLastModifiedFileTime;
      TUint16 iLastModifiedFileDate;
      TUint32 iCRC32;
      TUint32 iCompressedSize;
      TUint32 iUncompressedSize;
      TUint16 iFileNameLength;
      TUint16 iExtraFieldLength;
      TUint16 iFileCommentLength;
      TUint16 iDiskNumberStart;
      TUint16 iInternalFileAttributes;
      TUint32 iExternalFileAttributes;
      TUint32 iLocalHeaderOffset;
    };
    struct TLocalHeader
    {
      TUint32 iSignature;
      TUint16 iVersionNeeded;
      TUint16 iFlags;
      TUint16 iCompressionMethod;
      TUint16 iLastModifiedFileTime;
      TUint16 iLastModifiedFileDate;
      TUint32 iCRC32;
      TUint32 iCompressedSize;
      TUint32 iUncompressedSize;
      TUint16 iFileNameLength;
      TUint16 iExtraFieldLength;
    };
};

class RZipFileMemberReaderStream: public CBase
{
  public:
    IMPORT_C ~RZipFileMemberReaderStream();
    IMPORT_C TInt Read(TDes16& aDes,TInt aLength);
    virtual TInt Read(TDes8& aDes,TInt aLength);
    virtual void ReadL(TDes16& aDes,TInt aLength);
    void Release(void);
    void Close(void);
  private:
    RZipFileMemberReaderStream(CZipFile&,TUint32,TUint32,TUint32);
    virtual TInt Read(void);
    virtual TInt Read(TByte*,TUint32,TUint32*);
    TInt GetBytes(TByte*,TUint32,TUint32*);
    TInt GetBytes(void);
    TInt GetCompressedBytes(void);
    TInt GetStoredBytes(TByte*,TUint32,TUint32*);
    void ReadL(TDes8&);
    void ReadL(TDes16&);
    void ReadL(TDes8&,TChar);
    void ReadL(TDes16&,TChar);
    void ReadL(TUint8*,TInt);
    void ReadL(TUint16*,TInt);
    void ReadL(RWriteStream&);
    void ReadL(RWriteStream&,TInt);
    void ReadL(TInt);
    TInt8 ReadInt8L(void);
    TInt16 ReadInt16L(void);
    TInt32 ReadInt32L(void);
    TUint8 ReadUint8L(void);
    TUint16 ReadUint16L(void);
    TUint32 ReadUint32L(void);
    TReal32 ReadReal32L(void);
    TReal64 ReadReal64L(void);
    void PushL(void);
    void Pop(void);
    void Attach(MStreamBuf*);
    void Detach(void);
    MStreamBuf* Source(void);
  private:
    enum
    {
      KBytesSize=192,
      kCompressedBytesSize=64
    };
  private:
    CZipFile& iZipFile;
    TUint32 iCompressionMethod;
    z_stream iStream;
    TUint32 iCompressedSize;
    TUint32 iFileOffset;
    TUint32 iOffset;
    TBool iDone;
    TByte iBytes[KBytesSize];
    TUint32 iBytesLength;
    TUint32 iBytesOffset;
    TByte iCompressedBytes[kCompressedBytesSize];
    TUint32 iCompressedBytesOffset;
  friend class CZipFile;
};

class CZipFileMember: public CBase
{
  public:
    IMPORT_C TUint32 CRC32(void) const;
    IMPORT_C TUint32 CompressedSize(void) const;
    IMPORT_C const TFileName* Name(void) const;
    IMPORT_C TUint32 UncompressedSize(void) const;
    IMPORT_C ~CZipFileMember();
  private:
    TFileName* iName;
    TUint16 iCompressionMethod;
    TUint32 iCompressedSize;
    TUint32 iUncompressedSize;
    TUint32 iCRC32;
    TUint32 iDataOffset;
  friend class CZipFile;
};

class CZipFileMemberIterator
{
  public:
    IMPORT_C CZipFileMember* NextL(void);
  private:
    CZipFileMemberIterator(CZipFile*);
  private:
    CZipFile* iZipFile;
    TInt iIndex;
  friend class CZipFile;
};

class CZipFile : public CZipArchive
{
  public:
    enum
    {
      KZipFileError=KZipArchiveMinError-1,
      KZipFileNotFound=KZipFileError-1,
      KZipFileIOError=KZipFileError-2
    };
  public:
    IMPORT_C static CZipFile* NewL(RFs& aFs,const TDesC& aFileName);
    IMPORT_C CZipFile(RFs& aFs,const TDesC& aFileName);
    IMPORT_C TInt OpenL(void);
    IMPORT_C void Close(void);
    IMPORT_C virtual ~CZipFile();
  protected:
    IMPORT_C void ConstructL(const TDesC& aFileName);
  public:
    IMPORT_C TInt Size(TInt& aSize) const;
    IMPORT_C CZipFileMember* MemberL(const TDesC&);
    IMPORT_C CZipFileMember* CaseInsensitiveMemberL(const TDesC&);
    IMPORT_C CZipFileMember* CaseSensitiveOrCaseInsensitiveMemberL(const TDesC&);
    IMPORT_C TInt GetInputStreamL(const CZipFileMember*,RZipFileMemberReaderStream*&);
    IMPORT_C CZipFileMemberIterator* GetMembersL(void);
  protected:
    struct TMemberPointer
    {
      TFileName* iName;
      TUint32 iCentralHeaderOffset;
      TUint32 iLocalHeaderOffset;
      TUint32 iCRC32;
      TUint32 iCompressedSize;
      TUint32 iUncompressedSize;
    };
  protected:
    TInt FindCentralDirectoryTrailer(TUint32& offset);
    TInt ReadCentralDirectoryTrailer(TUint32 offset,TCentralDirectoryTrailer&);
    TInt ReadCentralDirectoryHeaderL(TCentralDirectoryHeader&,TMemberPointer&);
    TInt ReadLocalHeader(TUint32,TLocalHeader&);
    const TMemberPointer* FindMemberPointer(const TDesC&,TBool);
    TInt LoadMemberPointersL(void);
    RZipFileMemberReaderStream*   MakeInputStreamL(TUint32,TUint32,TUint32);
    CZipFileMember* MakeMemberL(TInt);
    CZipFileMember* MakeMemberL(const TMemberPointer&,const TLocalHeader&);
    void OpenFileL(const TDesC& aFileName);
    TInt Read(TByte*,TUint32);
    TInt Read(TUint16&);
    TInt Read(TUint32&);
    TInt Seek(TInt);
    void DeleteMemberPointers(void);
  private:
    const TDesC& iFileName;
    TUint32 iFileLength;
    TCentralDirectoryTrailer iTrailer;
    TMemberPointer* iMemberPointers;
    RFs& iFs;
    RFile iData;
    TBool iDataOpened;
  friend class RZipFileMemberReaderStream;
  friend class CZipFileMemberIterator;
};

#endif
