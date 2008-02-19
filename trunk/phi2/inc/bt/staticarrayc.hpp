/*
    staticarrayc.hpp
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

#ifndef __STATICARRAYC_HPP__
#define __STATICARRAYC_HPP__

template <class T> class TStaticArrayC
{
  public:
    inline const T& operator[](TInt aIndex) const;
  public:
    const T* iArray;
    TInt iCount;
};

#define CONSTRUCT_STATIC_ARRAY_C(aValue) {aValue,sizeof(aValue)/sizeof(*aValue)}

#include "bt/staticarrayc.inl"

#endif
