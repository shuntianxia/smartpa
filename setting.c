/*
** $Id$
**
** skindemo.c: Sample program for MiniGUI Programming Guide
**      Usage of SKIN.
**
** Copyright (C) 2004 ~ 2008 Feynman Software.
**
** License: GPL
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include "common.h"

#define IDC_DOCKBAR    101

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
    }
};

static int setting_proc (HWND hWnd, int nMessage, WPARAM wParam, LPARAM lParam)
{
	//if(nMessage >= MSG_FIRSTMOUSEMSG && nMessage <= MSG_LASTKEYMSG) {
		//ResetTimerEx(hWnd, 99, 15*100, standby_timer_proc);
	//}
    switch (nMessage)
    {
		case MSG_ERASEBKGND:
		{
			erase_bkgnd_with_pic(hWnd, (HDC)wParam, (const RECT*) lParam, 0);
			//SetWindowBkColor(hWnd, RGB2Pixel(HDC_SCREEN,85,85,85));
			return 0;
		}

        case MSG_CREATE:
        {
        	register_pic("res/common/");
        	register_pic("res/setting/");
        	register_pic("res/dock_bar/");
			init_button_ex(hWnd, buttonex_info, TABLESIZE(buttonex_info));

            hdockbar = CreateWindowEx (CTRL_ICONVIEW, NULL, 
					WS_VISIBLE | SVS_UPNOTIFY,
					WS_EX_TRANSPARENT | WS_EX_AUTOSECONDARYDC, 
					IDC_DOCKBAR, 
					0, 150, 480, 130,
					hWnd, 0L);
			init_dock_bar(hdockbar);
			break;
        }
            
		case MSG_COMMAND:
		{
			int id = LOWORD (wParam);
			int code = HIWORD (wParam);
			HWND hosted;
		
			switch (id) {
			case IDC_DOCKBAR:
				dock_bar_cmd_handler(hdockbar, code);
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
            
        case MSG_CLOSE:
        {
        	un_register_pic("res/common/");
        	un_register_pic("res/setting/");
        	un_register_pic("res/dock_bar/");
            DestroyAllControls (hWnd);
            DestroyMainWindow (hWnd);
            break;
        }
    }
    return DefaultMainWinProc(hWnd, nMessage, wParam, lParam);
}

int setting (HWND hosting)
{
    MSG Msg;
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;

    CreateInfo.dwStyle      = WS_NONE;
    CreateInfo.dwExStyle    = WS_EX_AUTOSECONDARYDC;
    CreateInfo.spCaption    = "setting";
    CreateInfo.hMenu        = 0;
    CreateInfo.hCursor      = GetSystemCursor(0);
    CreateInfo.hIcon        = 0;
    CreateInfo.MainWindowProc = setting_proc;
    CreateInfo.lx           = 0;
    CreateInfo.ty           = 0;
    CreateInfo.rx           = 480;
    CreateInfo.by           = 1280; 
    CreateInfo.iBkColor     = COLOR_lightgray;
    CreateInfo.dwAddData    = 0;
    CreateInfo.hHosting     = hosting;
    
    hMainWnd = CreateMainWindow (&CreateInfo);

    if (hMainWnd == HWND_INVALID)
        return -1;

    ShowWindow (hMainWnd, SW_SHOWNORMAL);

    while (GetMessage (&Msg, hMainWnd)) {
    	TranslateMessage(&Msg);
        DispatchMessage (&Msg);
    }

    MainWindowThreadCleanup (hMainWnd);
    return 0;
}
