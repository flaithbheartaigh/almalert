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
  SetDspState(ETrue);
  delete iAudio;
}

void CAlmAudioBase::ConstructL(const TDesC& aFileName)
{
  AlmProfile::SoundParamsL(iRingType,iRingVolume);
  iAudio=CAudioClient::NewL();
  SetDspState(EFalse);
  iPlayer=CMdaAudioPlayerUtility::NewFilePlayerL(aFileName,*this,Priority(),PriorityPreference());
}

/*
 aState:
   EFalse - ��������� DSP
   ETrue - �������� DSP
 */
void CAlmAudioBase::SetDspState(TBool aState)
{
  if(iAudio->Connect()==KErrNone) // ��� ������������� �������� - ���������� �������� ������.
  {
    iAudio->Session().AudioSendReceive(aState?0x40:0x3f,NULL);
    iAudio->Close();
  }
}

TInt CAlmAudioBase::UpdateVolume(void)
{
  TInt maxVolume=(iPlayer)?iPlayer->MaxVolume():0;
  TInt volume=maxVolume*iRingVolume/9U;
  if(volume>maxVolume) volume=maxVolume;
  if(!PlayAlways())
  {
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
    if(UpdateVolume())
    {
      PlayInit();
      iPlayer->Play();
      iPrepared=ETrue;
    }
  }
}

// aError ����� ���� KErrInUse
void CAlmAudioBase::MapcPlayComplete(TInt aError)
{
}