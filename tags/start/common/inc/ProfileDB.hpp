/*
    ProfileDB.hpp
    Copyright (C) 2005 zg

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

#ifndef __PROFILEDB_HPP__
#define __PROFILEDB_HPP__

#include <cdblen.h>
#include <e32base.h>

const TInt KMaxProfileNameLength    = 30;  // Maximum length of a profile name

class CProfileDb: public CBase
{
  public:
    friend class CProfileAPI;
    struct TProfileStruct
    {
        TInt iTableId;
        TInt iUID;
        TInt iRemove;
        TInt iModify;
        TInt iActive;
        TInt iVisible;
        TInt iRename;
        TInt iDivertType;
        TBuf<KCommsDbSvrMaxFieldLength> iDivertData;
        TBuf<KCommsDbSvrMaxFieldLength> iDivertTelephoneNumber;
        TBufC<KMaxProfileNameLength> iProfileName;
    };
    struct TSoundStruct
    {
      TInt iIndx;
      TInt iModify;
      TInt iEnable;
      TInt iVolume;
      TInt iMaxVolume;
      TFileName iFileName;
    };
    struct TSettingStruct
    {
      TInt iIndx;
      TInt iModify;
      TInt iValue;
    };
    enum TDbSortType
    {
      ENoSort=0,
      ECMTSort,
      EPDASort
    };
    enum TDbErrorCode
    {
      EDbSuccess=0,
      EErrorDatabaseFull,
      EErrorDoesNotExist,
      EErrorNoActiveProfile,
      EErrorOldAndNewProfileSame,
      EErrorActiveProfile,
      EErrorDatabase
    };
    IMPORT_C ~CProfileDb(void);
  private:
    IMPORT_C static CProfileDb* NewLC(void);
    IMPORT_C static CProfileDb* NewL(void);

    IMPORT_C TDbErrorCode LookupQueryL(CArrayFixFlat<CProfileDb::TProfileStruct>* aProfileArray,CProfileDb::TDbSortType aSortType=ENoSort);
    IMPORT_C TDbErrorCode ProfileAlertForQueryL(CArrayFixFlat<long>*,TInt*,TDes16&,TInt); //FIXME: ?
    IMPORT_C TDbErrorCode ProfileSettingQueryL(CArrayFixFlat<CProfileDb::TSettingStruct>* aSettingArray,TInt aTableId);
    IMPORT_C TDbErrorCode ProfileSoundQueryL(CArrayFixFlat<CProfileDb::TSoundStruct>* aSoundArray,TInt aTableId);

    IMPORT_C TDbErrorCode ReadProfileL(CArrayFixFlat<CProfileDb::TSettingStruct>* aSettingArray,CArrayFixFlat<CProfileDb::TSoundStruct>* aSoundArray,CArrayFixFlat<long>* aUnknown,TInt*,TDes16&,TInt); //FIXME: ?
    IMPORT_C TDbErrorCode WriteProfileL(CProfileDb::TProfileStruct* aProfileStruct,TDes16&,CArrayFixFlat<CProfileDb::TSettingStruct>* aSettingArray=NULL,CArrayFixFlat<CProfileDb::TSoundStruct>* aSoundArray=NULL,CArrayFixFlat<long>* aUnknown=NULL,unsigned short=0); //FIXME: ?
};

#endif
