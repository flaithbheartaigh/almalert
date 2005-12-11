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

class CProfileAPI: public CBase
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
    IMPORT_C static CProfileAPI* NewLC(TBool aIsServerEngine=EFalse);
    IMPORT_C static CProfileAPI* NewL(TBool aIsServerEngine=EFalse);

    IMPORT_C TProErrorCode GetProfileActiveName(TPtr16 aProfileName,TInt* aProfileUId);
    IMPORT_C TProErrorCode GetProfileAlertFor(CArrayFixFlat<TContactItemId>* anAlertFor,TInt* aSize,TInt aTableId=-1);
    IMPORT_C TProErrorCode GetProfileMsgAlertTone(TFileName* aFileName,TInt* aVolume,TInt aTableId=-1);
    IMPORT_C TProErrorCode GetProfileMultiData(TDes16& aRingingTone,TDes16& aSms,TInt& aRingType,TInt& aRingVolume,TInt& aVibra,TInt& aKeypadTones,CArrayFixFlat<TContactItemId>* anAlertFor,TInt& anAlertForCount,TInt aTableId=-1);
    IMPORT_C TProErrorCode GetProfileNameList(MDesC16Array* aProfileNameArray,CProfileDb::TDbSortType aSortType=CProfileDb::ENoSort);
    IMPORT_C TProErrorCode GetProfileNameList(CArrayFixFlat<CProfileDb::TProfileStruct>* aProfileArray,CProfileDb::TDbSortType aSortType=CProfileDb::ENoSort);
    IMPORT_C TProErrorCode GetProfileNotification(TInt* aOnf,TInt aProfileIndx=-1);
    IMPORT_C TProErrorCode GetProfileSettingList(CArrayFixFlat<CProfileDb::TSettingStruct>* aSettingArray,TInt aTableId=-1);
    IMPORT_C TProErrorCode GetProfileSoundList(CArrayFixFlat<CProfileDb::TSoundStruct>* aSoundArray,TInt aTableId=-1);
    IMPORT_C TProErrorCode SetProfileName(TInt aTableId);
    IMPORT_C TProErrorCode SilentIndicator(TInt& aSilent);
  private:
    CProfileDb *iDb;
    TBool iGlobalNote;
    RSAVarChangeNotify iNotify;
    TBool iNotifyConnect;
};

#endif
