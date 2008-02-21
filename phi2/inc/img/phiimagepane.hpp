/*
    phiimagepane.hpp
    Copyright (C) 2008 zg

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __PHIIMAGEPANE_HPP__
#define __PHIIMAGEPANE_HPP__

#include <coecntrl.h>
#include <fbs.h>
#include <mdaimageconverter.h>

class CPhiImagePane: public CCoeControl,public MMdaImageUtilObserver
{
  public:
    static CPhiImagePane* NewL(const TRect& aRect,MObjectProvider* aObjectProvider,const TDesC& aFileName);
    ~CPhiImagePane();
  public:
    TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
  private:
    void Draw(const TRect& aRect) const;
  private: //MMdaImageUtilObserver
    void MiuoCreateComplete(TInt aError);
    void MiuoOpenComplete(TInt aError);
    void MiuoConvertComplete(TInt aError);
  private:
    void ConstructL(const TRect& aRect,MObjectProvider* aObjectProvider,const TDesC& aFileName);
    CPhiImagePane();
    void ScrollL(TUint aCode,TBool aFast);
  private:
    enum TConsts
    {
      EStep=8,
      EStepFast=48
    };
  private:
    TBool iLoaded;
    CMdaImageFileToBitmapUtility* iConvert;
    CFbsBitmap* iBitmap;
    TPoint iOffset;
    TPoint iDrawOffset;
};

#endif
