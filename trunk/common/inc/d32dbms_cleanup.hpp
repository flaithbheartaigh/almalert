/*
    d32dbms_cleanup.hpp
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

#ifndef __D32DBMS_CLEANUP_HPP__
#define __D32DBMS_CLEANUP_HPP__

#include <e32base.h>

template<class T> class CleanupRollback
{
  public:
    inline static void PushL(T& aRef);
  private:
    static void Rollback(TAny *aPtr);
};
template <class T> inline void CleanupRollbackPushL(T& aRef);

template<class T> class CleanupCommit
{
  public:
    inline static void PushL(T& aRef);
  private:
    static void Commit(TAny *aPtr);
};
template <class T> inline void CleanupCommitPushL(T& aRef);

template<class T> class CleanupCancel
{
  public:
    inline static void PushL(T& aRef);
  private:
    static void Cancel(TAny *aPtr);
};
template <class T> inline void CleanupCancelPushL(T& aRef);

#include "d32dbms_cleanup.inl"
#endif
