/*
    EikSrvUi.hpp
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

#ifndef __EIKSRVUI_HPP__
#define __EIKSRVUI_HPP__

#include <eiksrv.h>
#include <AknNotifyPlugin.hpp>

class CEikServAppUi: public CEikServAppUiBase //FIXME: incompleted
{
  public:
    void SuppressAppSwitching(TInt);
  public:
    TInt iValueData39; //90
    TInt iValueData40; //94
    TInt iValueData41; //98
    TInt iValueData42; //9c
    TInt iValueData43; //a0
    TInt iValueData44; //a4
    TInt iValueData45; //a8
    TInt iValueData46; //ac
    TInt iValueData47; //b0
    TInt iValueData48; //b4
    TInt iValueData49; //b8
    TInt iValueData50; //bc
    TInt iValueData51; //c0
    TInt iValueData52; //c4
    TInt iValueData53; //c8
    TInt iValueData54; //cc
    TInt iValueData55; //d0
    TInt iValueData56; //d4
    TInt iValueData57; //d8
    TInt iValueData58; //dc
    TInt iValueData59; //e0
    TInt iValueData60; //e4
    TInt iValueData61; //e8
    TInt iValueData62; //ec
    CNotifierDialogController* iNoteController; //f0

};

#endif
