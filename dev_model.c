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

#include "sqlite3.h"
#include "dev_info.h"
#include "button_ex.h"

#define IDC_BACK 100
#define IDC_FINISH 101
#define IDC_DEVMODEL   102
#define IDC_DOCKBAR 103

static HWND hMainWnd = HWND_INVALID;
static HWND hdevmodel;
static HWND hdockbar;

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
		IDC_FINISH, 
		92, 1094, 
		296, 70,
		{"common/finish.png",
		"common/finish_focus.png",
		"common/finish_focus.png"},
		0
    }
};

static void draw_dev_model (HWND hWnd, HSVITEM hsvi, HDC hdc, RECT *rcDraw)
{
    const char *item_info = (const char*)scrollview_get_item_adddata (hsvi);

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
    TextOut (hdc, rcDraw->left + 130, rcDraw->top + 33, item_info);
}

static void destroy_item (HWND hWnd, HSVITEM hsvi)
{
	free((void *)SendMessage (hWnd, SVM_GETITEMADDDATA, 0, hsvi));
}

void init_dev_model(HWND hWnd)
{
	SVITEMINFO svii;
	sqlite3 *db = NULL;
	sqlite3_stmt *stmt = NULL;
	char sql[128];
	int i, rc, ncols;
	dev_info_t *dev_info;

	dev_info = (dev_info_t *)GetWindowAdditionalData(GetParent(hWnd));
	memset(sql, 0, sizeof(sql));
	snprintf(sql, sizeof(sql), \
			"select model from '%s' where brand_en='%s' and model!='no_model' order by model", \
			get_dev_tbl_name(dev_info->type), dev_info->dev_brand);
	printf("%s\n", sql);
	SendMessage (hWnd, SVM_SETITEMDRAW, 0, (LPARAM)draw_dev_model);
	SendMessage (hWnd, SVM_SETITEMDESTROY, 0, (LPARAM)destroy_item);

	rc = sqlite3_open("ir/IRLibaray.db" , &db);
	if(rc != SQLITE_OK) {
		fprintf(stderr , "can't open database: %s\n",sqlite3_errmsg(db));
		sqlite3_close(db);
		return;
	}
	rc = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
	if(rc != SQLITE_OK) {
		fprintf(stderr, "%s: %s\n", sql, sqlite3_errmsg(db));
		sqlite3_close(db);
		return;
	}
	while(sqlite3_step(stmt) == SQLITE_ROW) {
		//svii.nItem = i++;
		svii.nItemHeight = 90;
		svii.addData = (DWORD)strdup(sqlite3_column_text(stmt, 0));
		SendMessage (hWnd, SVM_ADDITEM, 0, (LPARAM)&svii);
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);
}

void dev_model_cmd_handler(HWND hWnd, int code)
{
	int sel;
	const char *info;
	dev_info_t *dev_info;
	sqlite3 *db = NULL;
	sqlite3_stmt *stmt = NULL;
	char sql[128];
	char *data = NULL;
	int i, rc, ncols;

	dev_info = (dev_info_t *)GetWindowAdditionalData(GetParent(hWnd));
	if (code == SVN_CLICKED) {
		sel = SendMessage (hWnd, SVM_GETCURSEL, 0, 0);
		info = (const char *)SendMessage (hWnd, SVM_GETITEMADDDATA, sel, 0);
		strcpy(dev_info->dev_model, info);
		memset(sql, 0, sizeof(sql));
		snprintf(sql, sizeof(sql), \
			"select code from '%s' where brand_en='%s' and model='%s'", \
			get_dev_tbl_name(dev_info->type), dev_info->dev_brand, dev_info->dev_model);
		printf("%s\n", sql);
		
		rc = sqlite3_open("ir/IRLibaray.db" , &db);
		if(rc != SQLITE_OK) {
			fprintf(stderr , "can't open database: %s\n",sqlite3_errmsg(db));
			sqlite3_close(db);
			return;
		}
		rc = sqlite3_prepare_v2(db, sql, strlen(sql), &stmt, NULL);
		if(rc != SQLITE_OK) {
			fprintf(stderr, "%s: %s\n", sql, sqlite3_errmsg(db));
			sqlite3_close(db);
			return;
		}
		sqlite3_step(stmt);
		data = (char *)sqlite3_column_blob(stmt,0);
		dev_info->code_len = sqlite3_column_bytes(stmt, 0);
		memcpy(dev_info->ir_code, data, dev_info->code_len);
		printf("ir_code_len is %d\n", dev_info->code_len);

		sqlite3_finalize(stmt);
		sqlite3_close(db);	
		switch(dev_info->type)
		{
		case DEV_TYPE_AIR_CLEANER:
			air_cleaner(GetParent(GetParent(hWnd)), dev_info);
			break;

		case DEV_TYPE_AIR_CONDITION:
			air_condition(GetParent(GetParent(hWnd)), dev_info);
			break;

		case DEV_TYPE_TV:
			tv(GetParent(GetParent(hWnd)), dev_info);
			break;

		case DEV_TYPE_IPTV:
			iptv(GetParent(GetParent(hWnd)), dev_info);
			break;

		case DEV_TYPE_TVBOX:
			tvbox(GetParent(GetParent(hWnd)), dev_info);
			break;

		case DEV_TYPE_FAN:
			fan(GetParent(GetParent(hWnd)), dev_info);
			break;

		case DEV_TYPE_DVD:
			dvd(GetParent(GetParent(hWnd)), dev_info);
			break;

		case DEV_TYPE_PROJECTOR:
			projector(GetParent(GetParent(hWnd)), dev_info);
			break;

		case DEV_TYPE_WATER_HEATER:
			water_heater(GetParent(GetParent(hWnd)), dev_info);
			break;
		}
	}
}

static int dev_model_proc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
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

			hdevmodel = CreateWindowEx	(CTRL_SCROLLVIEW, 
							0, 
							WS_CHILD | WS_VISIBLE | SVS_UPNOTIFY, 
							WS_EX_TRANSPARENT | WS_EX_AUTOSECONDARYDC,
							IDC_DEVMODEL, 
							0,
							389, 
							480,
							611,
							hWnd, 0L);
        	init_dev_model(hdevmodel);
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
				
			case IDC_DEVMODEL:
				//dev_model_cmd_handler(hdevmodel, code);
				break;
				
			case IDC_BACK:
				PostMessage(hWnd, MSG_CLOSE, 0, 0);
				break;
				
			case IDC_FINISH:
				dev_model_cmd_handler(hdevmodel, SVN_CLICKED);
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
    pCreateInfo->MainWindowProc = dev_model_proc;
    pCreateInfo->lx = 0; 
    pCreateInfo->ty = 0;
    pCreateInfo->rx = 480;
    pCreateInfo->by = 1280;
    pCreateInfo->iBkColor = GetWindowElementColor (WE_MAINC_THREED_BODY);
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

void dev_model (HWND hosting, dev_info_t *dev_info)
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

