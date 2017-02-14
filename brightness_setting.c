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
#include "font.h"

#define IDC_WORK_BRIGHT 102
#define IDC_STANDBY_BRIGHT 103

static HWND hMainWnd = HWND_INVALID;
static HWND hworkbri;
static HWND hstandbybri;

typedef struct {
	char *dev_name;
	char *dev_icon;
}item_info_t;

static void workbri_notif_proc (HWND hwnd, int id, int nc, DWORD add_data)
{
    if (nc == TBN_CHANGE) {
        //radius = SendMessage (hwnd, TBM_GETPOS, 0, 0);
        //InvalidateRect (GetParent (hwnd), &rcCircle, TRUE);
    }
}

static void standbybri_notif_proc (HWND hwnd, int id, int nc, DWORD add_data)
{
    if (nc == TBN_CHANGE) {
        //radius = SendMessage (hwnd, TBM_GETPOS, 0, 0);
        //InvalidateRect (GetParent (hwnd), &rcCircle, TRUE);
    }
}

static int brightness_setting_proc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
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

	        hworkbri = CreateWindowEx (CTRL_TRACKBAR, 
                  "", 
                  WS_VISIBLE | TBS_NOTIFY | TBS_USEPIC,
                  WS_EX_TRANSPARENT | WS_EX_AUTOSECONDARYDC,
                  IDC_WORK_BRIGHT, 
                  98, 478 - Y_OFFSET, 270, 26, hWnd, 0);

	        SendMessage (hworkbri, TBM_SETRANGE, 0, 80);
	        SendMessage (hworkbri, TBM_SETLINESIZE, 1, 0);
	        SendMessage (hworkbri, TBM_SETPAGESIZE, 10, 0);
	        SendMessage (hworkbri, TBM_SETTICKFREQ, 10, 0);
	        //SendMessage (hworkbri, TBM_SETPOS, radius, 0);
	        SendMessage (hworkbri, TBM_SETPIC, (WPARAM)pic, 0);
	        SetNotificationCallback (hworkbri, workbri_notif_proc);

	        hstandbybri = CreateWindowEx (CTRL_TRACKBAR, 
                  "", 
                  WS_VISIBLE | TBS_NOTIFY | TBS_USEPIC,
                  WS_EX_TRANSPARENT | WS_EX_AUTOSECONDARYDC,
                  IDC_STANDBY_BRIGHT,
                  98, 688 - Y_OFFSET, 270, 26, hWnd, 0);

	        SendMessage (hstandbybri, TBM_SETRANGE, 0, 80);
	        SendMessage (hstandbybri, TBM_SETLINESIZE, 1, 0);
	        SendMessage (hstandbybri, TBM_SETPAGESIZE, 10, 0);
	        SendMessage (hstandbybri, TBM_SETTICKFREQ, 10, 0);
	        //SendMessage (hworkbri, TBM_SETPOS, radius, 0);
	        SendMessage (hstandbybri, TBM_SETPIC, (WPARAM)pic, 0);
	        SetNotificationCallback (hstandbybri, standbybri_notif_proc);
			return 0;
		}

		case MSG_PAINT:
		{
			HDC hdc;
            hdc = BeginPaint (hWnd);
            FillBoxWithBitmap (hdc, 62, 475 - Y_OFFSET, 34, 34, GetBitmapFromRes(Str2Key("common/darker.png")));
            FillBoxWithBitmap (hdc, 376, 475 - Y_OFFSET, 34, 34, GetBitmapFromRes(Str2Key("common/brighter.png")));
            FillBoxWithBitmap (hdc, 62, 685 - Y_OFFSET, 34, 34, GetBitmapFromRes(Str2Key("common/darker.png")));
            FillBoxWithBitmap (hdc, 376, 685 - Y_OFFSET, 34, 34, GetBitmapFromRes(Str2Key("common/brighter.png")));
            SelectFont(hdc,font30);
            SetTextColor (hdc, RGB2Pixel(hdc,0x9f,0xa0,0xa2));
            SetBkMode(hdc,BM_TRANSPARENT);
       		TextOut(hdc, 80, 410 - Y_OFFSET,"work brightness");
       		TextOut(hdc, 80, 611 - Y_OFFSET,"standby brightness");
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
    pCreateInfo->dwExStyle = WS_EX_TOPMOST | WS_EX_AUTOSECONDARYDC;
    pCreateInfo->spCaption = "";
    pCreateInfo->hMenu = 0;
    pCreateInfo->hCursor = GetSystemCursor(1);
    pCreateInfo->hIcon = 0;
    pCreateInfo->MainWindowProc = brightness_setting_proc;
    pCreateInfo->lx = 0; 
    pCreateInfo->ty = Y_OFFSET;
    pCreateInfo->rx = 480;
    pCreateInfo->by = 1280;
    pCreateInfo->iBkColor = GetWindowElementColor (WE_MAINC_THREED_BODY);
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

void brightness_setting (HWND hosting)
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

