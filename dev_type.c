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
#include "button_ex.h"
#include "common.h"

//#define IDC_BACK 100
#define IDC_DEVTYPE    101
//#define IDC_DOCKBAR	102

static HWND hdockbar;
static HWND hdevtype;
static HWND hMainWnd = HWND_INVALID;
static dev_info_t dev_info;

typedef struct {
	dev_type_t type;
	char *dev_name;
	char *dev_icon;
	char *dev_icon_focus;
}item_info_t;

const static item_info_t item_info[] =
{
	{
		DEV_TYPE_TV,
		"TV",
		"dev_type/TV.png",
		"dev_type/TV_focus.png"
	},
	{
		DEV_TYPE_IPTV,
		"IPTV",
		"dev_type/IPTV.png",
		"dev_type/IPTV_focus.png"
	},
	{
		DEV_TYPE_TVBOX,
		"STB",
		"dev_type/STB.png",
		"dev_type/STB_focus.png"
	},
	{
		DEV_TYPE_DVD,
		"DVD",
		"dev_type/DVD.png",
		"dev_type/DVD_focus.png"
	},
	{
		DEV_TYPE_PROJECTOR,
		"projector",
		"dev_type/projector.png",
		"dev_type/projector_focus.png"
	},
	{
		DEV_TYPE_AIR_CONDITION,
		"air_condition",
		"dev_type/air_condition.png",
		"dev_type/air_condition_focus.png"
	},
	{
		DEV_TYPE_FAN,
		"air_fan",
		"dev_type/air_fan.png",
		"dev_type/air_fan_focus.png"
	},
	{
		DEV_TYPE_AIR_CLEANER,
		"air_cleaner",
		"dev_type/air_cleaner.png",
		"dev_type/air_cleaner_focus.png"
	},
	{
		DEV_TYPE_WATER_HEATER,
		"water_heaters",
		"dev_type/water_heaters.png",
		"dev_type/water_heaters_focus.png"
	}
};

static void draw_dev_type (HWND hWnd, HSVITEM hsvi, HDC hdc, RECT *rcDraw)
{
    const item_info_t *item_info = (const item_info_t*)scrollview_get_item_adddata (hsvi);
	PBITMAP pbmp, pbmp_arrow;

    if (scrollview_is_item_hilight(hWnd, hsvi)) {
        //SetBrushColor (hdc, PIXEL_blue);
        //FillBox (hdc, rcDraw->left+1, rcDraw->top+1, RECTWP(rcDraw)-2, RECTHP(rcDraw)-1);
        //SetBkColor (hdc, PIXEL_blue);
        SetTextColor (hdc, RGB2Pixel(hdc,0xf6,0xc4,0x6b));
        pbmp = GetBitmapFromRes(Str2Key(item_info->dev_icon_focus));
        pbmp_arrow = GetBitmapFromRes(Str2Key("dev_type/arrow_focus.png"));
    } else {
    	SetTextColor (hdc, RGB2Pixel(hdc,0x9f,0xa0,0xa2));
    	pbmp = GetBitmapFromRes(Str2Key(item_info->dev_icon));
    	pbmp_arrow = GetBitmapFromRes(Str2Key("dev_type/arrow.png"));
    }

    //Rectangle (hdc, rcDraw->left, rcDraw->top, rcDraw->right - 1, rcDraw->bottom);
	FillBoxWithBitmap (hdc, rcDraw->left + 44, rcDraw->top + 21, 0, 0, pbmp);
	FillBoxWithBitmap (hdc, rcDraw->left + 396, rcDraw->top + 33, 0, 0, pbmp_arrow);
	SetBrushColor(hdc,PIXEL_lightgray);
	FillBox(hdc,rcDraw->left + 20,rcDraw->top + 90, 440, 1);
	SelectFont(hdc, font24);
	SetBkMode (hdc, BM_TRANSPARENT);
    TextOut (hdc, rcDraw->left + 130, rcDraw->top + 33, item_info->dev_name);
}

void init_dev_type(HWND hdevtype)
{
	SVITEMINFO svii;
	static int i = 0;
	//SetWindowBkColor (hScrollView, PIXEL_lightwhite);
	//SetWindowBkColor (hDlg, PIXEL_lightgray);

	SendMessage (hdevtype, SVM_SETITEMDRAW, 0, (LPARAM)draw_dev_type);

	for (i = 0; i < TABLESIZE(item_info); i++) {
		svii.nItemHeight = 90;
		svii.addData = (DWORD)&item_info[i];
		svii.nItem = i;
		SendMessage (hdevtype, SVM_ADDITEM, 0, (LPARAM)&svii);
		SendMessage (hdevtype, SVM_SETITEMADDDATA, i, (DWORD)&item_info[i]);
	}
}

static void init_dev_info(dev_info_t *dev_info, const item_info_t *item)
{
	dev_info->type = item->type;
	switch(dev_info->type)
	{
		case  DEV_TYPE_TV:
			dev_info->match_code_key = TV_VolAdd;
			break;

		case  DEV_TYPE_IPTV:
			dev_info->match_code_key = IPTV_VolAdd;
			break;
			
		case  DEV_TYPE_TVBOX:
			dev_info->match_code_key = TVBOX_VolAdd;
			break;
		
		case  DEV_TYPE_DVD:
			dev_info->match_code_key = DVD_SK;
			break;
			
		case  DEV_TYPE_FAN:
			dev_info->match_code_key = FAN_On_Off;
			break;
		
		case  DEV_TYPE_AIR_CONDITION:
			dev_info->match_code_key = ARC_Power;
			break;
			
		case  DEV_TYPE_AIR_CLEANER:
			dev_info->match_code_key = TV_VolAdd;
			break;
			
		case  DEV_TYPE_PROJECTOR:
			dev_info->match_code_key = PJT_Off;
			break;
			
		case  DEV_TYPE_WATER_HEATER:
			dev_info->match_code_key = WH_Power;
			break;
	}
}

void dev_type_cmd_handler(HWND hdevtype, int code)
{
	int sel;
	const item_info_t *item;

	if (code == SVN_CLICKED) {
		sel = SendMessage (hdevtype, SVM_GETCURSEL, 0, 0);
		item = (const item_info_t *)SendMessage (hdevtype, SVM_GETITEMADDDATA, sel, 0);
		//SendMessage(GetParent(hdevtype),MSG_CLOSE,0,0);
		//dev_info.type = info->type;
		init_dev_info(&dev_info, item);
		dev_brand(GetParent(hdevtype), &dev_info);
	}
}

static int dev_type_proc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
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
			register_pic("res/dev_type/");
			//CreateButtonEx(&back_button, hWnd);
			//hdockbar = create_dock_bar(hWnd, IDC_DOCKBAR);
			hdevtype = CreateWindowEx	(CTRL_SCROLLVIEW, 
							0, 
							WS_CHILD | WS_VISIBLE | SVS_UPNOTIFY, 
							WS_EX_TRANSPARENT | WS_EX_AUTOSECONDARYDC,
							IDC_DEVTYPE, 
							0,
							389 - Y_OFFSET, 
							480,
							800,
							hWnd, 0L);
        	init_dev_type(hdevtype);
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
			HWND hosted;
		
			switch (id) {
			//case IDC_DOCKBAR:
				//dock_bar_cmd_handler(hdockbar, code);
				//break;
					
			case IDC_DEVTYPE:
				dev_type_cmd_handler(hdevtype, code);
				break;
				
			case IDC_BACK:
				hosted = GetFirstHosted(hWnd);
				if(hosted == HWND_INVALID || hosted == HWND_NULL) {
					PostMessage(hWnd, MSG_CLOSE, 0, 0);
				} else {
					//PostMessage(hosted, MSG_CLOSE, 0, 0);
					//SendNotifyMessage(hosted,MSG_COMMAND, wParam, lParam);
					SendMessage(hosted,MSG_COMMAND,wParam,lParam);
				}
				break;
			}
			break;
		}
		
        case MSG_DESTROY:
            DestroyAllControls (hWnd);
            hMainWnd = HWND_INVALID;
        	return 0;

        case MSG_CLOSE:
        	un_register_pic("res/dev_type/");
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
    pCreateInfo->hCursor = GetSystemCursor(0);
    pCreateInfo->hIcon = 0;
    pCreateInfo->MainWindowProc = dev_type_proc;
    pCreateInfo->lx = 0; 
    pCreateInfo->ty = Y_OFFSET;
    pCreateInfo->rx = 480;
    pCreateInfo->by = 1280;
    pCreateInfo->iBkColor = GetWindowElementColor (WE_MAINC_THREED_BODY);
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

void dev_type (HWND hosting)
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

