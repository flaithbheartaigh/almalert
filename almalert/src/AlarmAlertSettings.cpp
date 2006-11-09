/*
    AlarmAlertSettings.cpp
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

#include "AlarmAlertSettings.hpp"
#include <f32file.h>

_LIT(KSettings,"AlmAlert.txt");
_LIT(KLibs,"\\System\\Libs\\");
_LIT(KDefault,"z:\\Nokia\\Sounds\\Digital\\Nokia tune.mid");
_LIT(KNameAlarm,"Alarm");
_LIT(KNameCalendar,"Calendar");
_LIT(KNameSnoozeTime,"SnoozeTime");
_LIT(KNameSnoozeCount,"SnoozeCount");
_LIT(KNameBeep,"Beep");
_LIT(KNameBeepStart,"BeepStart");
_LIT(KNameBeepFinish,"BeepFinish");
_LIT(KNameBirthday,"Birthday");
_LIT(KNameBirthdayStart,"BirthdayStart");
_LIT(KNameBirthdayHour,"BirthdayHour");

CSettings::~CSettings()
{
  delete iAlarm;
  delete iCalendar;
  delete iBeep;
  if(iConnected) iSettings.Close();
}

CSettings::CSettings(): iBeepStart(1),iSnoozeCount(1),iBirthdayHour(12)
{
  if(iSettings.Connect()==KErrNone) iConnected=ETrue;
  RFs fs;
  if(fs.Connect()==KErrNone)
  {
    TFindFile find(fs);
    if(find.FindByDir(KSettings,KLibs)==KErrNone)
    {
      RFile file;
      if(file.Open(fs,find.File(),EFileShareAny)==KErrNone)
      {
        TFileText tfile;
        tfile.Set(file);
        TBuf<256> line;
        while(tfile.Read(line)==KErrNone)
        {
          if(line[0]==';') continue;
          if(!line.Length()) continue;
          TInt pos=line.Locate('=');
          if(pos==KErrNotFound) break;
          TPtrC name=line.Left(pos);
          TPtrC value=line.Mid(pos+1);
          if(!name.CompareF(KNameAlarm)&&!iAlarm)
          {
            iAlarm=value.Alloc();
          }
          else if(!name.CompareF(KNameCalendar)&&!iCalendar)
          {
            iCalendar=value.Alloc();
          }
          else if(!name.CompareF(KNameBeep)&&!iBeep&&value.Length())
          {
            iBeep=value.Alloc();
          }
          else if(!name.CompareF(KNameBirthday)&&!iBirthday&&value.Length())
          {
            iBirthday=value.Alloc();
          }
          else
          {
            TLex lex(value);
            TUint8 ivalue;
            if(lex.Val(ivalue,EDecimal)!=KErrNone) continue;
            if(!name.CompareF(KNameSnoozeTime))
            {
              if(ivalue>5) iSnoozeTime=ivalue-5;
            }
            else if(!name.CompareF(KNameSnoozeCount))
            {
              if(ivalue>0) iSnoozeCount=ivalue;
            }
            else if(!name.CompareF(KNameBeepStart))
            {
              iBeepStart=ivalue;
            }
            else if(!name.CompareF(KNameBeepFinish))
            {
              iBeepFinish=ivalue;
            }
            else if(!name.CompareF(KNameBirthdayStart))
            {
              iBirthdayStart=ivalue;
            }
            else if(!name.CompareF(KNameBirthdayHour))
            {
              iBirthdayHour=ivalue;
            }
          }
        }
        file.Close();
      }
    }
    fs.Close();
  }
}

const TDesC& CSettings::Alarm(void)
{
  if(iAlarm&&FileExist(*iAlarm)) return *iAlarm;
/*  if(iConnected)
  {

  }*/
  return KDefault;
}

const TDesC& CSettings::Calendar(void)
{
  if(iCalendar&&FileExist(*iCalendar)) return *iCalendar;
  return KDefault;
}

const TDesC& CSettings::Beep(void)
{
  if(iBeep&&FileExist(*iBeep)) return *iBeep;
  return KDefault;
}

const TDesC& CSettings::Birthday(void)
{
  if(iBirthday&&FileExist(*iBirthday)) return *iBirthday;
  return KDefault;
}

TBool CSettings::FileExist(const TDesC& aFileName)
{
  TBool res=EFalse;
  RFs fs;
  if(fs.Connect()==KErrNone)
  {
    TUint att;
    if(fs.Att(aFileName,att)==KErrNone) res=ETrue;
    fs.Close();
  }
  return res;
}
