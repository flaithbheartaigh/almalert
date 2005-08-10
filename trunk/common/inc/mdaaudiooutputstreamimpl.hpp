/*
    mdaaudiooutputstreamimpl.hpp
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

#ifndef __mdaaudiooutputstreamimpl_hpp__
#define __mdaaudiooutputstreamimpl_hpp__

#include <mdaaudiooutputstream.h>
#include <mdaaudiosampleeditor.h>

enum TMdaStreamPlayerState
{
  EMdaStreamPlayerNotOpen=0,
  EMdaStreamPlayerOpen=1,
  EMdaStreamPlayerPlaying=2
};

class CMdaAudioOutputStreamImpl;

class CSendBufferActive: public CActive
{
  public:
    CMdaAudioOutputStreamImpl* iStream;
  public:
    CSendBufferActive();
    void DoCancel();
    void RunL();
  friend class CMdaAudioOutputStreamImpl;
};

class COpenActive: public CActive
{
  public:
    CMdaAudioOutputStreamImpl* iStream;
    TMdaAudioDataSettings* iSettings;
  public:
    COpenActive();
    void DoCancel();
    void RunL();
  friend class CMdaAudioOutputStreamImpl;
};

class CPlayActive: public CActive
{
  public:
    CMdaAudioOutputStreamImpl* iStream;
  public:
    CPlayActive();
    void DoCancel();
    void RunL();
  friend class CMdaAudioOutputStreamImpl;
};

class CSoundBuffer: public CBase
{
  private:
    TSglQueLink iSlink;
    const TDesC8* iBuffer;
    CSoundBuffer(const TDesC8* aBuffer);
  public:
    static CSoundBuffer* NewL(const TDesC8* aBuffer);
    ~CSoundBuffer();
    const TDesC8* Buffer(void) const;
  public:
    static const TInt iOffset;
};

class CSoundBufferQueue: public CBase
{
  public:
    TSglQue<CSoundBuffer> iBuffers;
    TSglQueIter<CSoundBuffer> iBuffersIter;
    CSoundBufferQueue();
  public:
    ~CSoundBufferQueue();
    static CSoundBufferQueue* NewL(void);
    CSoundBuffer* SafeFirst(void);
    CSoundBuffer* RemoveFirst(void);
};

enum TMdaAudioOutputStream
{
  EMdaAudioOutputStreamDeviceInit,
  EMdaAudioOutputStreamDeviceConfig,
  EMdaAudioOutputStreamDeviceSendBuffer,
  EMdaAudioOutputStreamDeviceStop,
  EMdaAudioOutputStreamDevicePlay
};

class RMdaAudioOutputStreamDevice: public RMdaObject
{
  public:
    MDA_OBJECT_OPENREG(KUidMdaAudioOutputStreamDeviceValue)
    MDA_OBJECT_FUNCTIONPACK(Init,EMdaAudioOutputStreamDeviceInit,TMdaAudioOutputConfig)
    MDA_OBJECT_FUNCTIONPACK(Config,EMdaAudioOutputStreamDeviceConfig,TMdaAudioOutputConfig)
    MDA_OBJECT_FUNCTION2(SendBuffer,EMdaAudioOutputStreamDeviceSendBuffer,const TDesC8*,TUint)
    MDA_OBJECT_FUNCTION(Stop,EMdaAudioOutputStreamDeviceStop)
    MDA_OBJECT_FUNCTION(Play,EMdaAudioOutputStreamDevicePlay)
};


class CMdaAudioOutputStreamImpl: public CMdaAudioOutputStream
{
  public:
    IMPORT_C static CMdaAudioOutputStream* NewL(MMdaAudioOutputStreamCallback& aCallBack,CMdaServer* aServer=NULL);
    IMPORT_C static void ToggleVibra(TBool aState);
  public:
    void SetAudioPropertiesL(TInt aSampleRate,TInt aChannels);
    void Open(TMdaPackage* aSettings);
    TInt MaxVolume();
    TInt Volume();
    void SetVolume(const TInt aNewVolume);
    void SetPriority(TInt aPriority,TMdaPriorityPreference aPref);
    void WriteL(const TDesC8& aData);
    void Stop();
    const TTimeIntervalMicroSeconds& Position();
  public:
    ~CMdaAudioOutputStreamImpl();
  public:
    void Start();
  protected:
    CMdaAudioOutputStreamImpl(MMdaAudioOutputStreamCallback& aCallBack,CMdaServer* aServer);
    void ConstructL();
  private: //data
    MMdaAudioOutputStreamCallback* iCallBack; //0x04
    CMdaServer* iServer; //0x08
    TInt iOwnServer; //0x0c
    TMdaStreamPlayerState iState; //0x10
    RMdaAudioOutputStreamDevice iDevice; //0x14
    CSendBufferActive iSendBufferActive; //0x24
    CPlayActive iPlayActive; //0x40
    COpenActive iOpenActive; //0x5c
    TMdaAudioOutputConfig iConfig; //0x7c
    CSoundBufferQueue* iQueue; //0xb8
  friend class CSendBufferActive;
  friend class COpenActive;
  friend class CPlayActive;
};

#endif
