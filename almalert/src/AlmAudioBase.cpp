/*
    AlmAudioBase.cpp
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

#include "AlmAudioBase.hpp"
#include "AlmProfile.hpp"
#include <ProfileApi.hpp> //CProfileAPI

CAlmAudioBase::CAlmAudioBase(CEikonEnv* anEnv): CBase(),iEnv(anEnv)
{
}

CAlmAudioBase::~CAlmAudioBase()
{
  if(iPrepared) iPlayer->Stop();
  delete iPlayer;
  iSuspend.Close();
}

void CAlmAudioBase::ConstructL(CSettings* aSettings)
{
  AlmProfile::SoundParamsL(iRingType,iRingVolume);
  if(PlayAlways()||iRingType!=CProfileAPI::ERingTypeSilent)
  {
    iSuspend.Open();
    iPlayer=CMdaAudioPlayerUtility::NewFilePlayerL(FileName(aSettings),*this,Priority(),PriorityPreference());
  }
}

void CAlmAudioBase::UpdateVolume(void)
{
  TInt maxVolume=(iPlayer)?iPlayer->MaxVolume():0;
  TInt volume=maxVolume;
  if(!PlayAlways())
  {
    volume*=iRingVolume/9U;
    if(volume==0) volume=1;
    if(volume>maxVolume) volume=maxVolume;
  }
  if(iPlayer) iPlayer->SetVolume(volume);
}

void CAlmAudioBase::MapcInitComplete(TInt aError,const TTimeIntervalMicroSeconds& aDuration)
{
  if(aError!=KErrNone)
  {
    iEnv->NotifyIdleErrorWhileRedrawing(aError);
  }
  else
  {
    UpdateVolume();
    PlayInit();
    iPlayer->Play();
    iPrepared=ETrue;
  }
}

// aError может быть KErrInUse
void CAlmAudioBase::MapcPlayComplete(TInt aError)
{
  iSuspend.Close();
}
