/*
    FixSS.cpp
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

#include <adsp.hpp>
#include "FixSS.hpp"

_LIT8(KCopyright,"(c) by zg. version 1.06");

const TUid KUidRecog={KUidRecogValue};

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
{
  return KErrNone;
}

EXPORT_C CApaDataRecognizerType* CreateRecognizer()
{
  KCopyright();
  ADsp::StartServer();
  CRecog::StartThread();
  CApaDataRecognizerType* thing=new CRecog();
  return thing;
}

CRecog::CRecog():CApaDataRecognizerType(KUidRecog,CApaDataRecognizerType::ENormal)
{
  iCountDataTypes=0;
}

TUint CRecog::PreferredBufSize()
{
  return 0;
}

TDataType CRecog::SupportedDataTypeL(TInt /*aIndex*/) const
{
  return TDataType();
}

void CRecog::DoRecognizeL(TDesC& /*aName*/,const TDesC8& /*aBuffer*/)
{
  iConfidence=ENotRecognized;
}
