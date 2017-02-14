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

#include "dev_info.h"
#include "common.h"

#define IDC_DEVMODEL   102

static HWND hMainWnd = HWND_INVALID;
static HWND hdevmodel;

typedef struct {
	match_code_mode_t mode;
	char *dev_name;
	char *dev_icon;
}item_info_t;

const static item_info_t item_info[] =
{
	{
		MATCH_CODE_MODEL,
		"Type",
		"dev_icon/air_cleaner.png"
	},
	{
		MATCH_CODE_AUTO,
		"Auto match code",
		"dev_icon/air_condition.png"
	},
	{
		MATCH_CODE_MANUAL,
		"Manual match code",
		"dev_icon/air_fan.png"
	}
};

static void draw_match_code (HWND hWnd, HSVITEM hsvi, HDC hdc, RECT *rcDraw)
{
    const item_info_t *item_info = (const item_info_t*)scrollview_get_item_adddata (hsvi);
	PBITMAP pbmp, pbmp_arrow;

    SetBkMode (hdc, BM_TRANSPARENT);
    
    if (scrollview_is_item_hilight(hWnd, hsvi)) {
        //SetBkColor (hdc, PIXEL_blue);
		SetTextColor (hdc, PIXEL_yellow);
		pbmp_arrow = GetBitmapFromRes(Str2Key("dev_type/arrow_focus.png"));
    } else {
		SetTextColor (hdc, PIXEL_black);
		pbmp_arrow = GetBitmapFromRes(Str2Key("dev_type/arrow.png"));
	}
    //Rectangle (hdc, rcDraw->left, rcDraw->top, rcDraw->right - 1, rcDraw->bottom);
	SetBrushColor(hdc,PIXEL_lightgray);
	FillBox(hdc,rcDraw->left + 20,rcDraw->top + 90, 440, 1);
    TextOut (hdc, rcDraw->left + 60, rcDraw->top + 33, item_info->dev_name);
    FillBoxWithBitmap (hdc, rcDraw->left + 396, rcDraw->top + 33, 0, 0, pbmp_arrow);
}

void init_match_code(HWND hWnd)
{
	SVITEMINFO svii;
	static int i = 0;
	//SetWindowBkColor (hScrollView, PIXEL_lightwhite);
	//SetWindowBkColor (hDlg, PIXEL_lightgray);

	SendMessage (hWnd, SVM_SETITEMDRAW, 0, (LPARAM)draw_match_code);

	for (i = 0; i < TABLESIZE(item_info); i++) {
		svii.nItemHeight = 90;
		svii.addData = (DWORD)&item_info[i];
		svii.nItem = i;
		SendMessage (hWnd, SVM_ADDITEM, 0, (LPARAM)&svii);
		SendMessage (hWnd, SVM_SETITEMADDDATA, i, (DWORD)&item_info[i]);
	}
}

void match_code_cmd_handler(HWND hWnd, int code)
{
	int sel;
	const item_info_t *info;
	dev_info_t *dev_info;

	if (code == SVN_CLICKED) {
		sel = SendMessage (hWnd, SVM_GETCURSEL, 0, 0);
		info = (const item_info_t *)SendMessage (hWnd, SVM_GETITEMADDDATA, sel, 0);
		dev_info = (dev_info_t *)GetWindowAdditionalData(GetParent(hWnd));
		dev_info->match_code = info->mode;
		if(dev_info->match_code == MATCH_CODE_AUTO) {
			match_code_auto(GetParent(GetParent(hWnd)));
		} else if (dev_info->match_code == MATCH_CODE_MANUAL) {
			switch(dev_info->type)
			{
			case DEV_TYPE_AIR_CLEANER:
				//match_code_air_cleaner(GetParent(GetParent(hWnd)));
				break;

			case DEV_TYPE_AIR_CONDITION:
				match_code_air_condition(GetParent(GetParent(hWnd)));
				break;

			case DEV_TYPE_TV:
				match_code_tv(GetParent(GetParent(hWnd)));
				break;

			case DEV_TYPE_IPTV:
				match_code_tv(GetParent(GetParent(hWnd)));
				break;

			case DEV_TYPE_TVBOX:
				match_code_tvbox(GetParent(GetParent(hWnd)));
				break;

			case DEV_TYPE_FAN:
				match_code_fan(GetParent(GetParent(hWnd)));
				break;

			case DEV_TYPE_DVD:
				match_code_dvd(GetParent(GetParent(hWnd)));
				break;

			case DEV_TYPE_PROJECTOR:
				match_code_projector(GetParent(GetParent(hWnd)));
				break;

			case DEV_TYPE_WATER_HEATER:
				//match_code_water_heater(GetParent(GetParent(hWnd)));
				break;
			}
		} else if (dev_info->match_code == MATCH_CODE_MODEL) {
			dev_model(GetParent(GetParent(hWnd)), dev_info);
		}
	}
}

static int match_code_proc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
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
			hdevmodel = CreateWindowEx	(CTRL_SCROLLVIEW, 
							0, 
							WS_CHILD | WS_VISIBLE | SVS_UPNOTIFY, 
							WS_EX_TRANSPARENT,
							IDC_DEVMODEL, 
							0,
							389 - Y_OFFSET, 
							480,
							611,
							hWnd, 0L);
        	init_match_code(hdevmodel);
			return 0;
		}
		
		case MSG_COMMAND:
		{
			int id = LOWORD (wParam);
			int code = HIWORD (wParam);
		
			switch (id) {				
			case IDC_DEVMODEL:
				match_code_cmd_handler(hdevmodel, code);
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
    pCreateInfo->hCursor = GetSystemCursor(1);
    pCreateInfo->hIcon = 0;
    pCreateInfo->MainWindowProc = match_code_proc;
    pCreateInfo->lx = 0; 
    pCreateInfo->ty = Y_OFFSET;
    pCreateInfo->rx = 480;
    pCreateInfo->by = 1280;
    pCreateInfo->iBkColor = GetWindowElementColor (WE_MAINC_THREED_BODY);
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

void match_code (HWND hosting, dev_info_t *dev_info)
{
    MAINWINCREATE CreateInfo;

    if (hMainWnd != HWND_INVALID) {
        ShowWindow (hMainWnd, SW_SHOWNORMAL);
        return;
    }

    InitCreateInfo (&CreateInfo);
    CreateInfo.hHosting = hosting;
    CreateInfo.dwAddData = (DWORD)dev_info;

    hMainWnd = CreateMainWindow (&CreateInfo);
}

