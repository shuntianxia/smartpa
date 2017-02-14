/*
** $Id: iconview.c 741 2009-03-31 07:16:18Z weiym $
**
** iconview.c: Sample program for MiniGUI Programming Guide
**      Usage of ICONVIEW control.
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
#include "button_ex.h"
#include "dock_bar.h"

#define IDC_BACK 100
#define IDC_DEVMFR    101
#define IDC_DOCKBAR 102

typedef struct {
	dev_mfr_t mfr;
	char *label;
	char *icon_normal;
	char *icon_focus;
}item_info_t;

static HWND hdevmfr;
static HWND hdockbar;
static HWND hMainWnd = HWND_INVALID;

static buttonex_info_t back_button = 
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
};

static const item_info_t dev_mfr_item[] =
{
    {
    	DEV_MFR_CHIGO,
    	"Chigo",
    	"dev_mfr/Chigo.png",
    	"dev_mfr/Chigo_focus.png"
    },
    {
    	DEV_MFR_GREE,
    	"Gree",
    	"dev_mfr/Gree.png",
    	"dev_mfr/Gree_focus.png"
    },
	{
		DEV_MFR_HAIER,
		"Haier",
		"dev_mfr/Haier.png",
		"dev_mfr/Haier_focus.png"
	},
	{
		DEV_MFR_HISENSE,
    	"Hisense",
    	"dev_mfr/Hisense.png",
    	"dev_mfr/Hisense_focus.png"
    },
    {
    	DEV_MFR_MIDEA,
    	"Midea",
    	"dev_mfr/Midea.png",
    	"dev_mfr/Midea_focus.png"
    },
    {
    	DEV_MFR_NULL,
    	"Add Item",
    	"dev_mfr/+.png",
    	"dev_mfr/+_focus.png"
    }
};

static void draw_dev_mfr (HWND hWnd, GHANDLE hsvi, HDC hdc, RECT *rcDraw)
{
    const item_info_t *dev_mfr = (const item_info_t*)iconview_get_item_adddata(hsvi);
    
    SetBkMode (hdc, BM_TRANSPARENT);

    if (iconview_is_item_hilight(hWnd, hsvi)) {
    	SetTextColor (hdc, PIXEL_yellow);
        FillBoxWithBitmap (hdc, rcDraw->left + 25, rcDraw->top + 35, 0, 0, GetBitmapFromRes(Str2Key(dev_mfr->icon_focus)));
    }
    else {
    	SetTextColor (hdc, PIXEL_black);
        FillBoxWithBitmap (hdc, rcDraw->left + 25, rcDraw->top + 35, 0, 0, GetBitmapFromRes(Str2Key(dev_mfr->icon_normal)));
    }
        
    if (dev_mfr->label) {
        RECT rcTxt = *rcDraw;
        rcTxt.top = rcTxt.bottom - GetWindowFont (hWnd)->size * 10;
        rcTxt.left = rcTxt.left - (GetWindowFont (hWnd)->size) + 2;
        DrawText (hdc, dev_mfr->label, -1, &rcTxt, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    }
}

void init_dev_mfr(HWND hWnd)
{
	IVITEMINFO ivii;
	int i = 0;
	
	SetWindowBkColor (hWnd, PIXEL_lightwhite);
	//SetWindowExStyle(hIconView, WS_EX_TRANSPARENT);
	SendMessage (hWnd, IVM_SETITEMDRAW, 0, (LPARAM)draw_dev_mfr);
	SendMessage (hWnd, IVM_SETITEMSIZE, 168, 228);
	
	for (i = 0; i < TABLESIZE(dev_mfr_item); i++) {
		memset (&ivii, 0, sizeof(IVITEMINFO));
		ivii.bmp = GetBitmapFromRes(Str2Key(dev_mfr_item[i].icon_normal));
		ivii.nItem = i;
		ivii.label = dev_mfr_item[i].label;
		ivii.addData = (DWORD)&dev_mfr_item[i];
		SendMessage (hWnd, IVM_ADDITEM, 0, (LPARAM)&ivii);
	}
}

void dev_mfr_cmd_handler(HWND hWnd, int code)
{
	int sel;
	const item_info_t *info;
	dev_info_t *dev_info;

	if (code == IVN_CLICKED) {
		sel = SendMessage (hWnd, IVM_GETCURSEL, 0, 0);
		info = (const item_info_t *)SendMessage (hWnd, SVM_GETITEMADDDATA, sel, 0);
		dev_info = (dev_info_t *)GetWindowAdditionalData(GetParent(hWnd));
		dev_info->mfr = info->mfr;
		//printf("mfr is %d\n", info->mfr);
		match_code(GetParent(GetParent(hWnd)), dev_info);
	}
}

static int dev_mfr_proc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
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
			CreateButtonEx(&back_button, hWnd);
			hdevmfr = CreateWindowEx	(CTRL_ICONVIEW, 
							0, 
							WS_CHILD | WS_VISIBLE | SVS_UPNOTIFY, 
							WS_EX_TRANSPARENT,
							IDC_DEVMFR, 
							60,
							389, 
							360,
							800,
							hWnd, 0L);
        	init_dev_mfr(hdevmfr);
        	hdockbar = create_dock_bar(hWnd, IDC_DOCKBAR);
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
			case IDC_DOCKBAR:
				dock_bar_cmd_handler(hdockbar, code);
				break;
				
			case IDC_DEVMFR:
				dev_mfr_cmd_handler(hdevmfr, code);
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
    pCreateInfo->MainWindowProc = dev_mfr_proc;
    pCreateInfo->lx = 0; 
    pCreateInfo->ty = 0;
    pCreateInfo->rx = 480;
    pCreateInfo->by = 1280;
    pCreateInfo->iBkColor = GetWindowElementColor (WE_MAINC_THREED_BODY);
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

void dev_mfr (HWND hosting, dev_info_t *dev_info)
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

