#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>
#include "button_ex.h"

#define IDC_RETURN          100

static HWND htop_bar = HWND_INVALID;

static buttonex_info_t button_arrow =
{
	"",
	WS_VISIBLE | WS_CHILD | BEXS_BKIMAGE, 
	WS_EX_TRANSPARENT,
	IDC_RETURN, 
	10, 0, 
	24, 32,
	{"top_bar/return.png",
	"top_bar/return_focus.png",
	"top_bar/return_focus.png",
	0},
	0
};

static int top_bar_proc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    HWND hwnd;
    switch (message) {
    	case MSG_ERASEBKGND:
    		//erase_bkgnd_with_color(hWnd, (HDC)wParam, (const RECT*) lParam);
    		SetWindowBkColor(hWnd, RGB2Pixel((HDC)wParam,85,85,85));
    		break;
    		
        case MSG_CREATE:
        	//SetWindowExStyle(hWnd, WS_EX_TRANSPARENT);
 			CreateButtonEx(&button_arrow, hWnd);
            CreateWindowEx(CTRL_STATIC, 
                          "Language select", 
                          WS_CHILD | WS_VISIBLE,
                          WS_EX_TRANSPARENT,
                          IDC_STATIC, 
                          60, 10, 180, 140, hWnd, 0);

        break;

        case MSG_COMMAND:
        {
            //if (wParam == IDCANCEL) {
                //PostMessage (hWnd, MSG_CLOSE, 0, 0);
            //}
        }
        break;
       
        case MSG_DESTROY:
            DestroyAllControls (hWnd);
            htop_bar = HWND_INVALID;
        return 0;

        case MSG_CLOSE:
            DestroyMainWindow (hWnd);
            MainWindowCleanup (hWnd);
            return 0;
    }

    return DefaultMainWinProc (hWnd, message, wParam, lParam);
}

int create_top_bar (HWND hosting)
{
	MSG Msg;
    MAINWINCREATE CreateInfo;

    CreateInfo.dwStyle = WS_VISIBLE | WS_CHILD;
    CreateInfo.dwExStyle = WS_EX_TOPMOST;
    CreateInfo.spCaption = "top bar";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = top_bar_proc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 10;
    CreateInfo.rx = 400;
    CreateInfo.by = 42;
    CreateInfo.iBkColor = COLOR_darkgray;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = hosting;

	htop_bar = CreateMainWindow (&CreateInfo);

	if (htop_bar == HWND_INVALID)
		return -1;
	ShowWindow(htop_bar, SW_SHOWNORMAL);
	return 0;
}

