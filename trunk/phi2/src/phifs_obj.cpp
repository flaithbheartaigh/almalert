/*
    phifs_obj.cpp
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

#include "phifs.hpp"
#include "philistbox.hpp"

void CPhiFs::KillProcessesL(void)
{
  CPhiSelection* selection=iListBox->SelectionLC(CurrentFolder(),EFalse,CPhiSelection::ENone);
  TInt count=selection->Array()->MdcaCount();
  for(TInt ii=0;ii<count;ii++)
  {
    TPckgC16<TPhiEntry> item(selection->Array()->MdcaPoint(ii));
    RProcess process;
    if(process.Open(item().iName)==KErrNone)
    {
      if(!process.Protected()) process.Kill(KErrNone);
      process.Close();
    }
  }
  CleanupStack::PopAndDestroy(); //selection
  RefreshL();
}

void CPhiFs::KillThreadsL(void)
{
  CPhiSelection* selection=iListBox->SelectionLC(CurrentFolder(),EFalse,CPhiSelection::ENone);
  TInt count=selection->Array()->MdcaCount();
  for(TInt ii=0;ii<count;ii++)
  {
    TPckgC16<TPhiEntry> item(selection->Array()->MdcaPoint(ii));
    RThread thread;
    if(thread.Open(item().iName)==KErrNone)
    {
      if(!thread.Protected()) thread.Kill(KErrNone);
      thread.Close();
    }
  }
  CleanupStack::PopAndDestroy(); //selection
  RefreshL();
}
