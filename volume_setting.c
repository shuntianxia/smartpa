/*
** $Id: scrollview.c 741 2009-03-31 07:16:18Z weiym $
**
** scrollview.c: Sample program for MiniGUI Programming Guide
**      Usage of SCROLLVIEW control.
**
** Copyright (C) 2004 ~ 2007 Feynman Software.
**
** License: GPL
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include "common.h"

#define IDC_VOLUME 102

static HWND hMainWnd = HWND_INVALID;
static HWND hvolume;

typedef struct {
	char *dev_name;
	char *dev_icon;
}item_info_t;

static void volume_notif_proc (HWND hwnd, int id, int nc, DWORD add_data)
{
    if (nc == TBN_CHANGE) {
        //radius = SendMessage (hwnd, TBM_GETPOS, 0, 0);
        //InvalidateRect (GetParent (hwnd), &rcCircle, TRUE);
    }
}

static int volume_setting_proc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{	
	switch (message)
	{
		case MSG_ERASEBKGND:
		{
			erase_bkgnd_with_pic(hWnd, (HDC)wParam, (const RECT*) lParam, Y_OFFSET);
			return 0;
		}

		case MSG_CREATE:
		{	
			PBITMAP pic[3];

			pic[0] = GetBitmapFromRes(Str2Key("common/trackbar_0.png"));
			pic[1] = GetBitmapFromRes(Str2Key("common/trackbar_1.png"));
			pic[2] = GetBitmapFromRes(Str2Key("common/trackbar_2.png"));

	        hvolume = CreateWindowEx (CTRL_TRACKBAR, 
                  "", 
                  WS_VISIBLE | TBS_NOTIFY | TBS_USEPIC,
                  WS_EX_TRANSPARENT,
                  IDC_VOLUME,
                  102, 668 - Y_OFFSET, 270, 26, hWnd, 0);

	        SendMessage (hvolume, TBM_SETRANGE, 0, 80);
	        SendMessage (hvolume, TBM_SETLINESIZE, 1, 0);
	        SendMessage (hvolume, TBM_SETPAGESIZE, 10, 0);
	        SendMessage (hvolume, TBM_SETTICKFREQ, 10, 0);
	        //SendMessage (hworkbri, TBM_SETPOS, radius, 0);
	        SendMessage (hvolume, TBM_SETPIC, (WPARAM)pic, 0);
	        SetNotificationCallback (hvolume, volume_notif_proc);
			return 0;
		}

		case MSG_PAINT:
		{
			HDC hdc;
            hdc = BeginPaint (hWnd);
            FillBoxWithBitmap (hdc, 66, 665- Y_OFFSET, 32, 32, GetBitmapFromRes(Str2Key("common/s_volume.png")));
            FillBoxWithBitmap (hdc, 378, 665- Y_OFFSET, 34, 32, GetBitmapFromRes(Str2Key("common/l_volume.png")));
            EndPaint (hWnd, hdc);
            return 0;
		}
		
		case MSG_COMMAND:
		{
			int id = LOWORD (wParam);
			int code = HIWORD (wParam);
		
			switch (id) {
			case IDC_BACK:
				PostMessage(hWnd, MSG_CLOSE, 0, 0);
				break;
			}
			break;
		}
		
        case MSG_DESTROY:
            DestroyAllControls (hWnd);
            hMainWnd = HWND_INVALID;
        	return 0;

        case MSG_CLOSE:
            DestroyMainWindow (hWnd);
            MainWindowCleanup (hWnd);
            return 0;		
	} 
	return DefaultMainWinProc (hWnd, message, wParam, lParam);
} 

static void InitCreateInfo(PMAINWINCREATE pCreateInfo)
{
    pCreateInfo->dwStyle = WS_VISIBLE | WS_CHILD;
    pCreateInfo->dwExStyle = WS_EX_TOPMOST;
    pCreateInfo->spCaption = "";
    pCreateInfo->hMenu = 0;
    pCreateInfo->hCursor = GetSystemCursor(1);
    pCreateInfo->hIcon = 0;
    pCreateInfo->MainWindowProc = volume_setting_proc;
    pCreateInfo->lx = 0; 
    pCreateInfo->ty = Y_OFFSET;
    pCreateInfo->rx = 480;
    pCreateInfo->by = 1280;
    pCreateInfo->iBkColor = GetWindowElementColor (WE_MAINC_THREED_BODY);
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

void volume_setting (HWND hosting)
{
    MAINWINCREATE CreateInfo;

    if (hMainWnd != HWND_INVALID) {
        ShowWindow (hMainWnd, SW_SHOWNORMAL);
        return;
    }

    InitCreateInfo (&CreateInfo);
    CreateInfo.hHosting = hosting;

    hMainWnd = CreateMainWindow (&CreateInfo);
}

