/*
    ekern_kern_global.cpp
    Copyright (C) 2006 zg

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

#include <k32std61.hpp>

EXPORT_C void Kern61::KeyClick(void) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void Kern61::PointerClick(void) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void Kern::LocaleMessageText(TLocaleMessage aMsgNo,TDes& aMsg) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void Kern::SetIdleCallBack(TIdleCallBack aCallback) //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C void Kern::ClearIdleCallBack() //FIXME: NOT IMPLEMENTED
{
}

EXPORT_C DPeriphBusController* Kern::PBusController(TPBusNum aMachineBusNum) //FIXME: NOT IMPLEMENTED
{
  return NULL;
}

EXPORT_C CObjectCon* Kern::Processes() //FIXME: NOT IMPLEMENTED
{
  return NULL;
}

EXPORT_C CObjectCon* Kern::Threads() //FIXME: NOT IMPLEMENTED
{
  return NULL;
}

EXPORT_C CObjectCon* Kern::Chunks() //FIXME: NOT IMPLEMENTED
{
  return NULL;
}

EXPORT_C CObjectCon* Kern::LogicalDevices() //FIXME: NOT IMPLEMENTED
{
  return NULL;
}

EXPORT_C CObjectCon* Kern::PhysicalDevices() //FIXME: NOT IMPLEMENTED
{
  return NULL;
}

EXPORT_C CObjectCon* Kern::Channels() //FIXME: NOT IMPLEMENTED
{
  return NULL;
}

EXPORT_C CObjectCon* Kern::Libraries() //FIXME: NOT IMPLEMENTED
{
  return NULL;
}
