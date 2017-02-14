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
#include "sqlite3.h"
#include "common.h"

#define IDC_DEVBRAND   102

static HWND hMainWnd = HWND_INVALID;
static HWND hdevbrand;

static void draw_dev_brand (HWND hWnd, HSVITEM hsvi, HDC hdc, RECT *rcDraw)
{
    const char *item_info = (const char*)scrollview_get_item_adddata (hsvi);

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
	SelectFont(hdc, font24);
    TextOut (hdc, rcDraw->left + 130, rcDraw->top + 33, item_info);
}

static void destroy_item (HWND hWnd, HSVITEM hsvi)
{
	free((void *)SendMessage (hWnd, SVM_GETITEMADDDATA, 0, hsvi));
}

void init_dev_brand(HWND hWnd)
{
	SVITEMINFO svii;
	sqlite3 *db = NULL;
	sqlite3_stmt *stmt = NULL;
	char sql[128];
	int i, rc, ncols;
	dev_info_t *dev_info;

	dev_info = (dev_info_t *)GetWindowAdditionalData(GetParent(hWnd));
	memset(sql, 0, sizeof(sql));
	snprintf(sql, sizeof(sql), "select distinct brand_en from %s order by brand_en", get_dev_tbl_name(dev_info->type));
	SendMessage (hWnd, SVM_SETITEMDRAW, 0, (LPARAM)draw_dev_brand);
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

void dev_brand_cmd_handler(HWND hWnd, int code)
{
	int sel;
	const char *info;
	dev_info_t *dev_info;

	if (code == IVN_CLICKED) {
		sel = SendMessage (hWnd, IVM_GETCURSEL, 0, 0);
		info = (const char *)SendMessage (hWnd, SVM_GETITEMADDDATA, sel, 0);
		dev_info = (dev_info_t *)GetWindowAdditionalData(GetParent(hWnd));
		strcpy(dev_info->dev_brand, info);
		printf("dev_info->dev_brand is %s\n", dev_info->dev_brand);
		match_code(GetParent(hWnd), dev_info);
	}
}

static int dev_brand_proc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
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
			hdevbrand = CreateWindowEx	(CTRL_SCROLLVIEW, 
							0, 
							WS_CHILD | WS_VISIBLE | SVS_UPNOTIFY, 
							WS_EX_TRANSPARENT | WS_EX_AUTOSECONDARYDC,
							IDC_DEVBRAND, 
							0,
							389 - Y_OFFSET, 
							480,
							611,
							hWnd, 0L);
        	init_dev_brand(hdevbrand);
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
				
			case IDC_DEVBRAND:
				dev_brand_cmd_handler(hdevbrand, code);
				break;
				
			case IDC_BACK:
				hosted = GetFirstHosted(hWnd);
				if(hosted == HWND_INVALID || hosted == HWND_NULL) {
					PostMessage(hWnd, MSG_CLOSE, 0, 0);
				} else {
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
    pCreateInfo->MainWindowProc = dev_brand_proc;
    pCreateInfo->lx = 0; 
    pCreateInfo->ty = Y_OFFSET;
    pCreateInfo->rx = 480;
    pCreateInfo->by = 1280;
    pCreateInfo->iBkColor = GetWindowElementColor (WE_MAINC_THREED_BODY);
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

void dev_brand (HWND hosting, dev_info_t *dev_info)
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

