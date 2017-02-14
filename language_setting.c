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

#define IDC_OK 101
#define IDC_LANGUAGE   102

static HWND hMainWnd = HWND_INVALID;
static HWND hlanguage;

typedef struct {
	char *dev_name;
	char *dev_icon;
}item_info_t;

static buttonex_info_t buttonex_info [] =
{
    {
		"OK",
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		IDC_OK, 
		92, 1094 - Y_OFFSET, 
		296, 70,
		{"common/long_button_bg.png",
		"common/long_button_bg_focus.png",
		"common/long_button_bg_focus.png"},
		0
    }
};

const static item_info_t item_info[] =
{
	{
		"中文",
		"dev_icon/air_condition.png"
	},
	{
		"English",
		"dev_icon/air_cleaner.png"
	}
};

static void draw_language_setting (HWND hWnd, HSVITEM hsvi, HDC hdc, RECT *rcDraw)
{
    const item_info_t *item_info = (const item_info_t*)scrollview_get_item_adddata (hsvi);

    SetBkMode (hdc, BM_TRANSPARENT);
    
    if (scrollview_is_item_hilight(hWnd, hsvi)) {
        //SetBkColor (hdc, PIXEL_blue);
		SetTextColor (hdc, RGB2Pixel(hdc,0xf6,0xc4,0x6b));
    } else {
		SetTextColor (hdc, RGB2Pixel(hdc,0x9f,0xa0,0xa2));
	}
    //Rectangle (hdc, rcDraw->left, rcDraw->top, rcDraw->right - 1, rcDraw->bottom);
	SetBrushColor(hdc,PIXEL_lightgray);
	FillBox(hdc,rcDraw->left + 20,rcDraw->top + 90, 440, 1);
	SelectFont(hdc, font36);
    TextOut (hdc, rcDraw->left + 60, rcDraw->top + 33, item_info->dev_name);
}

void init_language_setting(HWND hWnd)
{
	SVITEMINFO svii;
	static int i = 0;
	//SetWindowBkColor (hScrollView, PIXEL_lightwhite);
	//SetWindowBkColor (hDlg, PIXEL_lightgray);

	SendMessage (hWnd, SVM_SETITEMDRAW, 0, (LPARAM)draw_language_setting);

	for (i = 0; i < TABLESIZE(item_info); i++) {
		svii.nItemHeight = 90;
		svii.addData = (DWORD)&item_info[i];
		svii.nItem = i;
		SendMessage (hWnd, SVM_ADDITEM, 0, (LPARAM)&svii);
		SendMessage (hWnd, SVM_SETITEMADDDATA, i, (DWORD)&item_info[i]);
	}
}

void language_setting_cmd_handler(HWND hWnd, int code)
{
	int sel;
	const item_info_t *info;

	if (code == SVN_CLICKED) {
		sel = SendMessage (hWnd, SVM_GETCURSEL, 0, 0);
		info = (const item_info_t *)SendMessage (hWnd, SVM_GETITEMADDDATA, sel, 0);
	}
}

static int language_setting_proc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
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
			init_button_ex(hWnd, buttonex_info, TABLESIZE(buttonex_info));

			hlanguage = CreateWindowEx	(CTRL_SCROLLVIEW, 
							0, 
							WS_CHILD | WS_VISIBLE, 
							WS_EX_TRANSPARENT | WS_EX_AUTOSECONDARYDC,
							IDC_LANGUAGE, 
							0,
							389 - Y_OFFSET, 
							480,
							611,
							hWnd, 0L);
        	init_language_setting(hlanguage);
			return 0;
		}

		case MSG_SHOWWINDOW:
		{
			//printf("windows caption is %s\n", GetWindowCaption(hWnd));
			
			break;
		}
		
		case MSG_COMMAND:
		{
			int id = LOWORD (wParam);
			int code = HIWORD (wParam);
		
			switch (id) 
			{			
				case IDC_LANGUAGE:
					language_setting_cmd_handler(hlanguage, code);
					break;

				case IDC_OK:
					PostMessage(hWnd, MSG_CLOSE, 0, 0);
					break;
					
				case IDC_BACK:
				{
					HWND hosted;
					hosted = GetFirstHosted(hWnd);
					if(hosted == HWND_INVALID || hosted == HWND_NULL) {
						PostMessage(hWnd, MSG_CLOSE, 0, 0);
					} else {
						//SendNotifyMessage(hosted,MSG_COMMAND, wParam, lParam);
						SendMessage(hosted,MSG_COMMAND,wParam,lParam);
					}
					break;
				}
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
    pCreateInfo->spCaption = "Language Setting";
    pCreateInfo->hMenu = 0;
    pCreateInfo->hCursor = GetSystemCursor(1);
    pCreateInfo->hIcon = 0;
    pCreateInfo->MainWindowProc = language_setting_proc;
    pCreateInfo->lx = 0; 
    pCreateInfo->ty = Y_OFFSET;
    pCreateInfo->rx = 480;
    pCreateInfo->by = 1280;
    pCreateInfo->iBkColor = GetWindowElementColor (WE_MAINC_THREED_BODY);
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

void language_setting (HWND hosting)
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

