/*
    CntPhone.cpp
    Copyright (C) 2005-2006 zg

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

#include "CntPhone.hpp"

void CContactPhoneNumberParserImpl::ExtractRawNumber(const TDesC& aTextualNumber,TDes& aRawNumber)
{
  aRawNumber.Zero();
  TInt length=aTextualNumber.Length();
  if(aTextualNumber.Length())
  {
    TPtrC16 ptr(aTextualNumber);
    TUint16 phChar=ptr[0];
    while(TChar(phChar).IsSpace())
    {
      if(!--length) return;
      ptr.Set(ptr.Right(length));
      phChar=ptr[0];
    }
    if(phChar=='*'||phChar=='#')
    {
      TInt plusPosition=ptr.Locate('+');
      if(plusPosition>4||plusPosition==KErrNotFound)
      {
        TInt astericPosition=ptr.LocateReverse('*');
        TInt barsPosition=ptr.LocateReverse('#');
        if(astericPosition>=barsPosition)
        {
          barsPosition=astericPosition;
        }
        plusPosition=barsPosition;
      }
      length-=plusPosition;
      length--;
      if(length<=0) return;
      ptr.Set(ptr.Right(length));
      phChar=ptr[0];
    }
    if(phChar=='(')
    {
      length--;
      ptr.Set(ptr.Right(length));
      phChar=ptr[0];
    }
    if(phChar=='+')
    {
      length--;
      ptr.Set(ptr.Right(length));
    }
    if(!length) return;
    TLex16 lex(ptr);
    for(;;)
    {
      TChar currChar=lex.Peek();
      if(!currChar) break;
      if(currChar.IsDigit())
      {
        aRawNumber.Append(currChar);
      }
      else
      {
        if(currChar=='*'||currChar=='#')
        {
          aRawNumber.Zero();
          break;
        }
        currChar=User::LowerCase(currChar);
        if(currChar=='p'||currChar=='w'||currChar=='+') break;
      }
      lex.Inc();
    }
  }
}

EXPORT_C CContactPhoneNumberParser* NewPhoneNumberParser(void)
{
  return new CContactPhoneNumberParserImpl;
}

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
{
  return KErrNone;
}
