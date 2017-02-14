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

#include "button_ex.h"

#define IDC_BACK 100
#define IDC_OK 101
#define IDC_DEVMODEL   102

static HWND hMainWnd = HWND_INVALID;
static HWND htime_setting;

typedef struct {
	char *dev_name;
	char *dev_icon;
}dev_info_t;

static buttonex_info_t buttonex_info [] =
{
    {
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		IDC_BACK, 
		19, 20, 
		62, 70,
		{"common/back.png",
		"common/back_focus.png",
		"common/back_focus.png"},
		0
    },
    {
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		IDC_OK, 
		92, 1094, 
		296, 70,
		{"common/finish.png",
		"common/finish_focus.png",
		"common/finish_focus.png"},
		0
    }
};

const static dev_info_t dev_info[] =
{
	{
		"English",
		"dev_icon/air_cleaner.png"
	},
	{
		"Chinese",
		"dev_icon/air_condition.png"
	},
	{
		"Japanese",
		"dev_icon/air_fan.png"
	}
};

static void draw_time_setting (HWND hWnd, HSVITEM hsvi, HDC hdc, RECT *rcDraw)
{
    const dev_info_t *dev_info = (const dev_info_t*)scrollview_get_item_adddata (hsvi);

    SetBkMode (hdc, BM_TRANSPARENT);
    
    if (scrollview_is_item_hilight(hWnd, hsvi)) {
        //SetBkColor (hdc, PIXEL_blue);
		SetTextColor (hdc, PIXEL_yellow);
    } else {
		SetTextColor (hdc, PIXEL_black);
	}
    //Rectangle (hdc, rcDraw->left, rcDraw->top, rcDraw->right - 1, rcDraw->bottom);
	SetBrushColor(hdc,PIXEL_lightgray);
	FillBox(hdc,rcDraw->left + 20,rcDraw->top + 90, 440, 1);
    TextOut (hdc, rcDraw->left + 60, rcDraw->top + 33, dev_info->dev_name);
}

void init_time_setting(HWND hWnd)
{
	SVITEMINFO svii;
	static int i = 0;
	//SetWindowBkColor (hScrollView, PIXEL_lightwhite);
	//SetWindowBkColor (hDlg, PIXEL_lightgray);

	SendMessage (hWnd, SVM_SETITEMDRAW, 0, (LPARAM)draw_time_setting);

	for (i = 0; i < TABLESIZE(dev_info); i++) {
		svii.nItemHeight = 90;
		svii.addData = (DWORD)&dev_info[i];
		svii.nItem = i;
		SendMessage (hWnd, SVM_ADDITEM, 0, (LPARAM)&svii);
		SendMessage (hWnd, SVM_SETITEMADDDATA, i, (DWORD)&dev_info[i]);
	}
}

void time_setting_cmd_handler(HWND hWnd, int code)
{
	int sel;
	const char *info;

	if (code == SVN_CLICKED) {
		sel = SendMessage (hWnd, SVM_GETCURSEL, 0, 0);
		info = (const char *)SendMessage (hWnd, SVM_GETITEMADDDATA, sel, 0);
	}
}

static int time_setting_proc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{	
	switch (message)
	{
		case MSG_ERASEBKGND:
			   {
				   HDC hdc = (HDC)wParam;
				   const RECT* clip = (const RECT*) lParam;
				   BOOL fGetDC = FALSE;
				   RECT rcTemp;
						   
				   if (hdc == 0) {
					   hdc = GetClientDC (hWnd);
					   fGetDC = TRUE;
				   }	   
						   
				   if (clip) {
					   rcTemp = *clip;
					   ScreenToClient (hWnd, &rcTemp.left, &rcTemp.top);
					   ScreenToClient (hWnd, &rcTemp.right, &rcTemp.bottom);
					   IncludeClipRect (hdc, &rcTemp);
				   }
				   else
					   GetClientRect (hWnd, &rcTemp);
		
				   FillBoxWithBitmap (hdc, 0, 0, 
						   RECTW(rcTemp), RECTH(rcTemp), GetBitmapFromRes(Str2Key("bg.png")));
		
				   if (fGetDC)
					   ReleaseDC (hdc);
				   return 0;
			   }

		case MSG_CREATE:
		{	
			int i;
			for ( i = 0; i < TABLESIZE(buttonex_info); i++ )
			{
				CreateButtonEx(&buttonex_info[i], hWnd);
			}

			htime_setting = CreateWindowEx	(CTRL_SCROLLVIEW, 
							0, 
							WS_CHILD | WS_VISIBLE, 
							WS_EX_TRANSPARENT,
							IDC_DEVMODEL, 
							0,
							389, 
							480,
							611,
							hWnd, 0L);
        	init_time_setting(htime_setting);
			return 0;
		}

		case MSG_PAINT:
		{
			HDC hdc;
            hdc = BeginPaint (hWnd);
            //FillBoxWithBitmap (hdc, 55, 914, 86, 186, GetBitmapFromRes(Str2Key("tv/+-bg.png")));
            //FillBoxWithBitmap (hdc, 339, 914, 86, 186, GetBitmapFromRes(Str2Key("tv/+-bg.png")));
            EndPaint (hWnd, hdc);
            return 0;
		}
		
		case MSG_COMMAND:
		{
			int id = LOWORD (wParam);
			int code = HIWORD (wParam);
		
			switch (id) {
			case IDC_DEVMODEL:
				time_setting_cmd_handler(htime_setting, code);
				break;
				
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
    pCreateInfo->hCursor = GetSystemCursor(0);
    pCreateInfo->hIcon = 0;
    pCreateInfo->MainWindowProc = time_setting_proc;
    pCreateInfo->lx = 0; 
    pCreateInfo->ty = 0;
    pCreateInfo->rx = 480;
    pCreateInfo->by = 1280;
    pCreateInfo->iBkColor = GetWindowElementColor (WE_MAINC_THREED_BODY);
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

void time_setting (HWND hosting)
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

