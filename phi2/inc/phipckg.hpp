/*
    phipckg.hpp
    Copyright (C) 2007-2008 zg

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

#ifndef __PHIPCKG_HPP__
#define __PHIPCKG_HPP__

#include <e32std.h>

#define sizeof16(T) ((sizeof(T)+1)/2)

template <class T> class TPckgC16: public TPtrC16
{
  public:
    inline TPckgC16(const T& aRef);
    inline TPckgC16(const TPtrC16& aRef);
    inline const T& operator()() const;
  private:
    TPckgC16<T>& operator=(const TPckgC16<T>& aRef);
};

template <class T> inline TPckgC16<T>::TPckgC16(const T& aRef): TPtrC16((const TUint16*)&aRef,sizeof16(T)) {}
template <class T> inline TPckgC16<T>::TPckgC16(const TPtrC16& aRef): TPtrC16(aRef) {}
template <class T> inline const T& TPckgC16<T>::operator()() const
{
  return *((const T*)iPtr);
}

template <class T> class TPckgBuf16: public TBuf16<sizeof16(T)>
{
  public:
    inline TPckgBuf16();
    inline TPckgBuf16(const T& aRef);
    inline TPckgBuf16& operator=(const TPckgBuf16<T>& aRef);
    inline T& operator=(const T& aRef);
    inline T& operator()();
};

template <class T> inline TPckgBuf16<T>::TPckgBuf16(): TBuf16<sizeof16(T)>(sizeof16(T))
{
  new(&iBuf[0]) T;
}

template <class T> inline TPckgBuf16<T>::TPckgBuf16(const T& aRef): TBuf16<sizeof16(T)>(sizeof16(T))
{
  new(&iBuf[0]) T(aRef);
}

template <class T> inline TPckgBuf16<T>& TPckgBuf16<T>::operator=(const TPckgBuf16<T>& aRef)
{
  Copy(aRef);
  return *this;
}

template <class T> inline T& TPckgBuf16<T>::operator=(const T& aRef)
{
  Copy((TUint16*)&aRef,sizeof16(T));
  return *((T*)&iBuf[0]);
}

template <class T> inline T& TPckgBuf16<T>::operator()()
{
  return *((T*)&iBuf[0]);
}

#endif
