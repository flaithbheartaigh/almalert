/*
    mrouterclient.h
    Copyright (C) 2008 zg

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

#ifndef __MROUTERCLIENT_H__
#define __MROUTERCLIENT_H__

#include <mrouterclientdefs.h>

class RmRouterClient: public RSessionBase
{
  public:
    IMPORT_C RmRouterClient();
    IMPORT_C ~RmRouterClient();
    IMPORT_C void ConnectL(TBool aStartIfNeeded);
    IMPORT_C TInt MRConnectL(TInt aAttempts);
    IMPORT_C void MRDisconnectL(void);
    IMPORT_C void MRConfigL(TmRConfig* aConfig);
    IMPORT_C void MRSetConfigL(const TmRConfig& aConfig);
    IMPORT_C void MRStatusL(TmRStatus* aStatus);
    IMPORT_C void BTListenStartL(void);
    IMPORT_C void BTListenStopL(void);
    IMPORT_C void BTListenStatusL(TBTListenStatus* aStatus);
  };

const TInt KmRouterClientMajorVersion=1;
const TInt KmRouterClientMinorVersion=0;

#endif
