/*
    SysUtilEx.hpp
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

#ifndef __SYSUTILEX_HPP__
#define __SYSUTILEX_HPP__

#include <sysutil.h>

enum TSWState
{
  ESWState200=200,
  ESWState201, //device startup phaze 1
  ESWState202, //device startup phaze 2
  ESWState203, //device on with gui
  ESWState204, //device off
  ESWState205, //device on with alarm
  ESWState206, //device on without gui
  ESWState207, //gui loading
  ESWState208,
  ESWState209,
  ESWState210,
  ESWState211,
  ESWState212,
  ESWState213
};

enum TSWStartupReason
{
  ESWStartupReason000,
  ESWStartupReason001,
  ESWStartupReason002,
  ESWStartupReason003,
  ESWStartupReason004
};

enum TWD2StartupReason
{
  EWD2StartupNormal,
  EWD2StartupAlarm,
  EWD2StartupReason002,
  EWD2StartupReason003,
  EWD2StartupCharger
};

class SysStartup
{
  public:
    static void SetState(TUid const&,TSWState);
    static void SetSWStartupReason(TSWStartupReason);
    static void GetStartupReasons(TWD2StartupReason&,TSWStartupReason&);
    static void ShutdownAndRestart(TUid const&,TSWStartupReason);
    static void Shutdown(TUid const&);
    static TSWState State(void);
    static void HiddenReset(void);
    static void RTCStatus(void);
};

class SysEvent
{
  public:
    static void PostEvent(TDesC16 const&,TUid const&,TUid const&,TDesC16 const&,int,TDesC16 const&,int,TDesC16 const&,TDesC16 const&);
};

class SysAlarm
{
  public:
    static void SetNOSAlarm(TTime const&,int);
};

class SysLocale
{
  public:
    static void LoadLocaleL(TDesC16 const&);
    static void SaveLocaleL(TDesC16 const&);
    static void MakeFileNameL(TDesC16 const&,TLanguage);
};

#endif
