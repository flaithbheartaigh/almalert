/*
    ProfileApi.hpp
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

#ifndef __PROFILEAPI_HPP__
#define __PROFILEAPI_HPP__

#include <ProfileDB.hpp>
#include <bamdesca.h>

class CProfileAPI: public CBase //FIXME: incomplete
{
  public:
    enum TProErrorCode
    {
      EProSuccess=0,
      EProFailure,
      EProDivertPhoneOff,
      EProDivertPhoneOutOfCoverage
    };

    enum TRingTypes
    {
      ERingTypeNormal=0,
      ERingTypeAscending,
      ERingTypeRingOnce,
      ERingTypeBeepOnce,
      ERingTypeSilent
    };

    enum TKeypadTones
    {
      EKeypadToneOff=0,
      EKeypadToneLevel1,
      EKeypadToneLevel2,
      EKeypadToneLevel3
    };

    IMPORT_C ~CProfileAPI(void);
    IMPORT_C static CProfileAPI* NewLC(TBool aIsServerEngine=FALSE);
    IMPORT_C static CProfileAPI* NewL(TBool aIsServerEngine=FALSE);

    IMPORT_C TProErrorCode GetProfileActiveName(TPtr16 aProfileName,TInt* aProfileUId);
    IMPORT_C TProErrorCode GetProfileAlertFor(CArrayFixFlat<long>*,TInt*,TInt); //FIXME:?
    IMPORT_C TProErrorCode GetProfileMsgAlertTone(TBuf<256>*,TInt*,TInt); //FIXME:?
    IMPORT_C TProErrorCode GetProfileMultiData(TDes16& aRingingTone,TDes16& aSms,TInt& aRingType,TInt& aRingVolume,TInt& aVibra,TInt& aKeypadTones,CArrayFixFlat<long>* anAlertFor,TInt& anAlertForCount,TInt aTableId=-1); //FIXME:?
    IMPORT_C TProErrorCode GetProfileNameList(MDesC16Array* aProfileNameArray,CProfileDb::TDbSortType aSortType=CProfileDb::ENoSort);
    IMPORT_C TProErrorCode GetProfileNameList(CArrayFixFlat<CProfileDb::TProfileStruct>* aProfileArray,CProfileDb::TDbSortType aSortType=CProfileDb::ENoSort);
    IMPORT_C TProErrorCode GetProfileNotification(TInt* aOnf,TInt aProfileIndx=-1);
    IMPORT_C TProErrorCode GetProfileSettingList(CArrayFixFlat<CProfileDb::TSettingStruct>* aSettingArray,TInt aTableId=-1);
    IMPORT_C TProErrorCode GetProfileSoundList(CArrayFixFlat<CProfileDb::TSoundStruct>* aSoundArray,TInt aTableId=-1);
    IMPORT_C TProErrorCode SetProfileName(TInt); //FIXME:?
    IMPORT_C TProErrorCode SilentIndicator(TInt&); //FIXME:?

};

#endif
