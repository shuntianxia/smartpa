#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include "setup_wizard.h"
#include "button_ex.h"
#include "register_res.h"
//#include "mg_common_header.h"
#include "font.h"

static int welcome_proc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
	WORD keycode = -1;

    switch (message) {
        case MSG_CREATE:
            SetTimer (hWnd, 1000, 200);
			break;

        case MSG_PAINT:
            hdc = BeginPaint (hWnd);
			SetTextColor(hdc,COLOR_lightwhite);
			SetBkMode(hdc, BM_TRANSPARENT);
			SelectFont (hdc, 20);
			TextOut (hdc, 104, 597, "Welcome");
            EndPaint (hWnd, hdc);
            break;

        case MSG_TIMER:
			KillTimer(hWnd, 1000);
			PostMessage (hWnd, MSG_CLOSE, 0, 0);
            break;

        case MSG_CLOSE:
            DestroyMainWindow (hWnd);
            return 0;
    }

    return DefaultMainWinProc (hWnd, message, wParam, lParam);
}

int welcome (HWND hosting)
{
    MSG Msg;
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;

    CreateInfo.dwStyle = WS_NONE;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = welcome_proc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = 400;
    CreateInfo.by = 1280;
    CreateInfo.iBkColor = COLOR_darkgray;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = hosting;
    
    hMainWnd = CreateMainWindow (&CreateInfo);
    
    if (hMainWnd == HWND_INVALID) {
        return -1;
    }
	ShowWindow(hMainWnd, SW_SHOWNORMAL);
	while (GetMessage (&Msg, hMainWnd)) {
		TranslateMessage (&Msg);
		DispatchMessage (&Msg);
	}
	MainWindowThreadCleanup (hMainWnd);

    return 0;
}

