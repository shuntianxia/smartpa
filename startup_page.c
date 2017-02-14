#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#define IDC_PROGRESS        206

static char msg_text [32];
static RECT msg_rc = {102, 638, 40, 160};

static int startup_page_proc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
	static PLOGFONT logfont;
    HDC hdc;
	static int s_nProgress;
	static HWND s_hProgress;

    switch (message) {
        case MSG_CREATE:
			s_nProgress = 0;
			logfont = CreateLogFont ("ttf", "times", "ISO8859-1", 
                FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, FONT_SETWIDTH_NORMAL,
                FONT_SPACING_CHARCELL, FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
                18, 0);
		    s_hProgress = CreateWindowEx ("progressbar", NULL, 
                    WS_VISIBLE, WS_EX_NONE,
                    IDC_PROGRESS,
                    42, 636, 316, 8,
                    hWnd, 0L);
            SendMessage (s_hProgress, PBM_SETRANGE, 0, 400);
            SetTimer (hWnd, 1000, 1);
			InvalidateRect (s_hProgress, NULL, FALSE);
            break;

        case MSG_TIMER:
            s_nProgress++;
			if(s_nProgress < 401) {
				SendMessage (s_hProgress, PBM_SETPOS, s_nProgress, 0L);
			} else {
				KillTimer(hWnd, 1000);
				PostMessage (hWnd, MSG_CLOSE, 0, 0);
			}
            break;

        case MSG_PAINT:
            hdc = BeginPaint (hWnd);
			SetTextColor(hdc,COLOR_lightwhite);
			SetBkMode(hdc, BM_TRANSPARENT);
			SelectFont (hdc, logfont);
			TextOut (hdc, 155, 532, "Starting up ...");
            EndPaint (hWnd, hdc);
            return 0;

        case MSG_CLOSE:
			DestroyAllControls(hWnd);
            DestroyMainWindow (hWnd);
            return 0;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

int startup_page ()
{
    MSG Msg;
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "smartpa" , 0 , 0);
#endif
    CreateInfo.dwStyle = WS_VISIBLE;
    CreateInfo.dwExStyle = WS_EX_NONE;
	CreateInfo.spCaption = "test";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = startup_page_proc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = 400;
    CreateInfo.by = 1280;
    CreateInfo.iBkColor = COLOR_black;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;
    hMainWnd = CreateMainWindow (&CreateInfo);
    if (hMainWnd == HWND_INVALID)
        return -1;

    ShowWindow(hMainWnd, SW_SHOWNORMAL);

    while (GetMessage(&Msg, hMainWnd)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    MainWindowThreadCleanup (hMainWnd);

    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

