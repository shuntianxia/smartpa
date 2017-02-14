/*
** $Id$
**
** skindemo.c: Sample program for MiniGUI Programming Guide
**      Usage of SKIN.
**
** Copyright (C) 2004 ~ 2008 Feynman Software.
**
** License: GPL
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include "register_res.h"
#include "font.h"
#include "taskbar.h"
#include "desktop.h"
#include "common.h"

#define IDC_DOCKBAR    100
#define IDC_DEVTYPE    101
#define IDC_DEVMODEL   102
#define IDC_DEVMFR     103

static HWND hdockbar;
static HWND hTaskBar;
static HWND hwnd_screensaver;

static int init_resource (void);
static void cleaup_resource (void);

static int init_resource (void)
{
	int nRet;
	
	nRet = RegisterButtonExControl ();
	create_font();
	return nRet;
}

static void cleaup_resource (void)
{
	UnregisterButtonExControl ();	
	destroy_font();
}

int MiniGUIMain (int argc, const char *argv[])
{
    MSG Msg;
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;

    if ( FALSE == init_resource() )
	{
		printf ("Init Resource fail\n");
		return -1;
	}
	SetCustomDesktopOperationSet(&mg_dsk_ops);
    if ((hTaskBar = create_task_bar ()) == HWND_INVALID) {
        fprintf (stderr, "Can not create task bar.\n");
        return 2;
    }
    while (GetMessage (&Msg, hTaskBar)) {
    	TranslateMessage (&Msg);
        DispatchMessage (&Msg);
    }
    return 0;
}
