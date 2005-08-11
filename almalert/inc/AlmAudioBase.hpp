/*
    AlmAudioBase.hpp
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

#ifndef __ALMALERTAUDIOBASE_HPP__
#define __ALMALERTAUDIOBASE_HPP__

#include <mdaaudiosampleplayer.h> //MMdaAudioToneObserver
#include <eikenv.h> //CEikonEnv
#include <AudioSvrClient.hpp> //CAudioClient
#include "AlarmAlertSettings.hpp" //CSettings

class CAlmAudioBase: public CBase,public MMdaAudioPlayerCallback
{
  public:
    ~CAlmAudioBase();
  protected: //MMdaAudioPlayerCallback
    void MapcInitComplete(TInt aError,const TTimeIntervalMicroSeconds& aDuration);
    void MapcPlayComplete(TInt aError);
  protected: //interface
    virtual TInt Priority(void)=0;
    virtual TMdaPriorityPreference PriorityPreference(void)=0;
    virtual TBool PlayAlways(void)=0;
    virtual void PlayInit(void)=0;
    virtual const TDesC& FileName(CSettings* aSettings)=0;
  protected:
    CAlmAudioBase(CEikonEnv* anEnv);
    void ConstructL(CSettings* aSettings);
  protected:
    TInt iRingVolume;
    TInt iRingType;
    CMdaAudioPlayerUtility* iPlayer;
  private:
    enum TState
    {
      ENonPrepared1=0,
      EPrepared1,
      ENonPrepared2,
      EPrepared2
    };
  private:
    TState iState;
    CEikonEnv* iEnv;
    CAudioClient* iAudio;
  private:
    TInt UpdateVolume(void);
    void SetDspState(TBool aState);
};

#endif
