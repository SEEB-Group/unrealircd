/*
 * Disallow notices in channel UnrealIRCd Module (Channel Mode +T)
 * (C) Copyright 2014 Travis McArthur (Heero) and the UnrealIRCd team
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 1, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "config.h"
#include "struct.h"
#include "common.h"
#include "sys.h"
#include "numeric.h"
#include "msg.h"
#include "proto.h"
#include "channel.h"
#include <time.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <io.h>
#endif
#include <fcntl.h>
#include "h.h"
#ifdef _WIN32
#include "version.h"
#endif

ModuleHeader MOD_HEADER(nonotice)
  = {
	"chanmodes/nonotice",
	"$Id$",
	"Channel Mode +T",
	"3.2-b8-1",
	NULL 
    };

Cmode_t EXTCMODE_NONOTICE;

#define IsNoNotice(chptr)    (chptr->mode.extmode & EXTCMODE_NONOTICE)

int nonotice_check_can_send(aClient *cptr, aChannel *chptr, char *msgtext, Membership *lp, int notice);

DLLFUNC int MOD_TEST(nonotice)(ModuleInfo *modinfo)
{
	return MOD_SUCCESS;
}

DLLFUNC int MOD_INIT(nonotice)(ModuleInfo *modinfo)
{
	CmodeInfo req;

	memset(&req, 0, sizeof(req));
	req.paracount = 0;
	req.flag = 'T';
	req.is_ok = extcmode_default_requirechop;
	CmodeAdd(modinfo->handle, req, &EXTCMODE_NONOTICE);
	
	HookAddEx(modinfo->handle, HOOKTYPE_CAN_SEND, nonotice_check_can_send);

	MARK_AS_OFFICIAL_MODULE(modinfo);
	return MOD_SUCCESS;
}

DLLFUNC int MOD_LOAD(nonotice)(int module_load)
{
	return MOD_SUCCESS;
}

DLLFUNC int MOD_UNLOAD(nonotice)(int module_unload)
{
	return MOD_SUCCESS;
}

int nonotice_check_can_send(aClient *cptr, aChannel *chptr, char *msgtext, Membership *lp, int notice)
{
	if (notice && IsNoNotice(chptr) &&
	   (!lp || !(lp->flags & (CHFL_CHANOP | CHFL_CHANOWNER | CHFL_CHANPROT))))
		return CANNOT_SEND_NONOTICE;

	return HOOK_CONTINUE;
}
