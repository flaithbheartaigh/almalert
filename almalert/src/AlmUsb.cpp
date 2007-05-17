/*
    AlmUsb.cpp
    Copyright (C) 2007 zg

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

#include "AlmUsb.hpp"
#include <sacls_ngage.hpp>

CUsbNotifier* CUsbNotifier::NewLC(CLockNotifier& aNotifier)
{
  CUsbNotifier* self=new(ELeave)CUsbNotifier(aNotifier);
  CleanupStack::PushL(self);
  self->ConstructL();
  return self;
}

CUsbNotifier::~CUsbNotifier()
{
  Cancel();
  iSysAgnt.Close();
}

void CUsbNotifier::RunL(void)
{
  TRAPD(err,iNotifier.LockNotifyL(iEvent.State()));
  NotifyL();
}

void CUsbNotifier::DoCancel(void)
{
  iSysAgnt.NotifyEventCancel();
}

CUsbNotifier::CUsbNotifier(CLockNotifier& aNotifier): CActive(EPriorityStandard),iNotifier(aNotifier),iEvent(iStatus)
{
}

void CUsbNotifier::ConstructL(void)
{
  User::LeaveIfError(iSysAgnt.Connect());
  iSysAgnt.SetEventBufferEnabled(ETrue);
  iEvent.SetUid(KUidUsbStatus);
  CActiveScheduler::Add(this);
  NotifyL();
}

void CUsbNotifier::NotifyL(void)
{
  iSysAgnt.NotifyOnEvent(iEvent);
  SetActive();
}
