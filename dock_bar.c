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
#include "common.h"
#include "dock_bar.h"

typedef struct {
	char *label;
	char *icon_normal;
	char *icon_focus;
}dock_item_t;

static HWND hdockbar;
static int nContX, nContY;
static int cur_sel;

static const dock_item_t dockbar_item[] =
{
    {
    	"language",
    	"dock_bar/ic_language.png",
    	"dock_bar/ic_language_focus.png"
    },
    {
    	"brightness",
    	"dock_bar/ic_brightness.png",
    	"dock_bar/ic_brightness_focus.png"
    },
    {
    	"wifi",
    	"dock_bar/ic_wifi.png",
    	"dock_bar/ic_wifi_focus.png"
    },
    {
    	"remote_control",
    	"dock_bar/ic_remote_control.png",
    	"dock_bar/ic_remote_control_focus.png"
    },
    {
    	"time",
    	"dock_bar/ic_time.png",
    	"dock_bar/ic_time_focus.png",
    },
    {
    	"volume",
    	"dock_bar/ic_volume.png",
    	"dock_bar/ic_volume_focus.png"
    },
    {
    	"power_mgt",
    	"dock_bar/ic_power_mgt.png",
    	"dock_bar/ic_power_mgt_focus.png"
    },
#if 0
    {
    	"timer_switch",
    	"dock_bar/ic_timer_switch.png",
    	"dock_bar/ic_timer_switch_focus.png"
    },
    {
    	"help",
    	"dock_bar/ic_help.png",
    	"dock_bar/ic_help_focus.png"
    }
#endif
};

static void draw_dock_bar (HWND hWnd, GHANDLE hsvi, HDC hdc, RECT *rcDraw)
{
    const dock_item_t *dock_item = (const dock_item_t*)iconview_get_item_adddata(hsvi);
    
    SetBkMode (hdc, BM_TRANSPARENT);

    if (iconview_is_item_hilight(hWnd, hsvi)) {
    	SetTextColor (hdc, RGB2Pixel(hdc,0xf6,0xc4,0x6b));
        FillBoxWithBitmap (hdc, rcDraw->left + 46, rcDraw->top, 0, 0, GetBitmapFromRes(Str2Key(dock_item->icon_focus)));
    }
    else {
    	SetTextColor (hdc, RGB2Pixel(hdc,0x9f,0xa0,0xa2));
        FillBoxWithBitmap (hdc, rcDraw->left + 46, rcDraw->top, 0, 0, GetBitmapFromRes(Str2Key(dock_item->icon_normal)));
    }
        
    if (dock_item->label) {
        RECT rcTxt = *rcDraw;
        rcTxt.top = rcTxt.bottom - 30;
        SelectFont(hdc, font22);
        DrawText (hdc, dock_item->label, -1, &rcTxt, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    }
}

void init_dock_bar(HWND hdockbar)
{
	IVITEMINFO ivii;
	int i = 0;
	
	SetWindowBkColor (hdockbar, PIXEL_lightwhite);
	//SetWindowExStyle(hIconView, WS_EX_TRANSPARENT);
	SendMessage (hdockbar, IVM_SETITEMDRAW, 0, (LPARAM)draw_dock_bar);
	SendMessage (hdockbar, IVM_SETITEMSIZE, 152, 98);
	
	for (i = 0; i < TABLESIZE(dockbar_item); i++) {
		memset (&ivii, 0, sizeof(IVITEMINFO));
		ivii.bmp = GetBitmapFromRes(Str2Key(dockbar_item[i].icon_normal));
		ivii.nItem = i;
		ivii.label = dockbar_item[i].label;
		ivii.addData = (DWORD)&dockbar_item[i];
		SendMessage (hdockbar, IVM_ADDITEM_ALINE, 0, (LPARAM)&ivii);
	}
	SendMessage(hdockbar,SVM_SETCURSEL,cur_sel,0);
	SendMessage(hdockbar,SVM_SETCONTPOS,nContX, nContY);
}

HWND create_dock_bar(HWND hWnd, int id)
{
	HWND hdockbar;
	hdockbar = CreateWindowEx (CTRL_ICONVIEW, NULL, 
			WS_VISIBLE | SVS_UPNOTIFY,
			WS_EX_TRANSPARENT | WS_EX_AUTOSECONDARYDC, 
			id, 
			0, 150, 480, 100,
			hWnd, 0L);
	init_dock_bar(hdockbar);
	return hdockbar;
}

void dock_bar_cmd_handler(HWND hdockbar, int code)
{
	HWND hosted;
	if (code == IVN_CLICKED) {
		hosted = GetFirstHosted(GetParent(hdockbar));
		if(hosted != HWND_INVALID && hosted != HWND_NULL) {
			DestroyMainWindow(hosted);
		}
		cur_sel = SendMessage (hdockbar, IVM_GETCURSEL, 0, 0);
		nContX = SendMessage(hdockbar,SVM_GETCONTENTX, 0, 0);
		nContY = SendMessage(hdockbar,SVM_GETCONTENTY, 0, 0);
		if(strcmp(dockbar_item[cur_sel].label, "language") == 0) {
			language_setting(GetParent(hdockbar));
		} else if(strcmp(dockbar_item[cur_sel].label, "remote_control") == 0) {
			dev_type(GetParent(hdockbar));
		} else if(strcmp(dockbar_item[cur_sel].label, "brightness") == 0) {
			brightness_setting(GetParent(hdockbar));
		} else if(strcmp(dockbar_item[cur_sel].label, "volume") == 0) {
			volume_setting(GetParent(hdockbar));
		} else if(strcmp(dockbar_item[cur_sel].label, "power_mgt") == 0) {
			power_setting(GetParent(hdockbar));
		}else if(strcmp(dockbar_item[cur_sel].label, "time") == 0) {
			//power_setting(GetParent(hdockbar));
			//date_select();
		}
	}
}
#if 0
static int dock_bar_proc (HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case MSG_ERASEBKGND:
		//erase_bkgnd_with_pic(hDlg, (HDC)wParam, (const RECT*) lParam);
		erase_bkgnd_with_color(hDlg, (HDC)wParam, (const RECT*) lParam);
		return 0;
			   
    case MSG_INITDIALOG:
    {
    	hdockbar = GetDlgItem (hDlg, IDC_DOCKBAR);
		init_dock_bar(hdockbar);
        break;
    }

    case MSG_COMMAND:
	{
		int id = LOWORD (wParam);
		int code = HIWORD (wParam);
	
		switch (id) {
		case IDC_DOCKBAR:
			dock_bar_cmd_handler(hdockbar, code);
			break;
		}
		break;
	}

    case MSG_CLOSE:
    {
        EndDialog (hDlg, 0);
        return 0;
    }

    }

    return DefaultDialogProc (hDlg, message, wParam, lParam);
}

static CTRLDATA ctrls[] =
{
    {
        CTRL_ICONVIEW,
        WS_CHILD | WS_VISIBLE,
         0, 0, 400, 100,
        IDC_DOCKBAR,
        "",
        0,
        WS_EX_TRANSPARENT
    }
};

static DLGTEMPLATE Dlgdockbar =
{
    WS_NONE,
    WS_EX_TOPMOST,
    0, 150, 400, 100,
    "my_friends",
    0, 0,
    TABLESIZE(ctrls), ctrls,
    0
};

void dock_bar (HWND hosting)
{
    CreateMainWindowIndirectParam (&Dlgdockbar, hosting, dock_bar_proc, 0L);
}
#endif
