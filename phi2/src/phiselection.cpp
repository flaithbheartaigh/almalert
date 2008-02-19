/*
    phiselection.cpp
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

#include "phiselection.hpp"

CPhiSelection* CPhiSelection::NewLC(void)
{
  CPhiSelection* self=new(ELeave)CPhiSelection;
  CleanupStack::PushL(self);
  self->ConstructL();
  return self;
}

CPhiSelection::~CPhiSelection()
{
}

void CPhiSelection::ConstructL(void)
{
  iFiles=new(ELeave)CDesCArrayFlat(6);
}

const MDesCArray* CPhiSelection::Array(void) const
{
  return iFiles;
}

const TDesC& CPhiSelection::Folder(void) const
{
  return iFolder;
}

CPhiSelection::TType CPhiSelection::Type(void) const
{
  return iType;
}
