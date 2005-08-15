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
  if(iState==EPrepared1||iState==EPrepared2) iPlayer->Stop();
  delete iPlayer;
  SetDspState(ETrue);
  delete iAudio;
}

void CAlmAudioBase::ConstructL(CSettings* aSettings)
{
  AlmProfile::SoundParamsL(iRingType,iRingVolume);
  iAudio=CAudioClient::NewL();
  SetDspState(EFalse);
  iPlayer=CMdaAudioPlayerUtility::NewFilePlayerL(FileName(aSettings),*this,Priority(),PriorityPreference());
}

/*
 aState:
   EFalse - выключить DSP
   ETrue - включить DSP
 */
void CAlmAudioBase::SetDspState(TBool aState)
{
  if(iAudio->Connect()==KErrNone) // при невозможности коннекта - продолжаем работать дальше.
  {
    iAudio->Session().AudioSendReceive(aState?0x40:0x3f,NULL);
    iAudio->Close();
  }
}

TInt CAlmAudioBase::UpdateVolume(void)
{
  TInt maxVolume=(iPlayer)?iPlayer->MaxVolume():0;
  TInt volume=maxVolume;
  if(!PlayAlways())
  {
    volume*=iRingVolume/9U;
    if(volume==0) volume=1;
    if(volume>maxVolume) volume=maxVolume;
    if(iRingType==CProfileAPI::ERingTypeSilent) volume=0;
  }
  if(iPlayer) iPlayer->SetVolume(volume);
  return volume;
}

void CAlmAudioBase::MapcInitComplete(TInt aError,const TTimeIntervalMicroSeconds& aDuration)
{
  if(aError!=KErrNone)
  {
    iEnv->NotifyIdleErrorWhileRedrawing(aError);
  }
  else
  {
    if(iState==ENonPrepared1)
    {
      iState=EPrepared1;
      if(UpdateVolume())
      {
        PlayInit();
        iPlayer->Play();
      }
    }
    else
    {
      iState=EPrepared2;
      iPlayer->SetVolume(iPlayer->MaxVolume());
      iPlayer->Play();
    }
  }
}

// aError может быть KErrInUse
void CAlmAudioBase::MapcPlayComplete(TInt aError)
{
  _LIT(KNoSound,"z:\\System\\Sounds\\Digital\\No_Sound.wav");
  if(iState==EPrepared1&&aError==KErrInUse)
  {
    delete iPlayer;
    iState=ENonPrepared2;
    TRAPD(err,iPlayer=CMdaAudioPlayerUtility::NewFilePlayerL(KNoSound,*this,Priority(),PriorityPreference()));
    if(err!=KErrNone) iPlayer=NULL;
  }
}
