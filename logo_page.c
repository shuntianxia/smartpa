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

static void make_logo_text (void)
{
    const char* sys_charset = GetSysCharset (TRUE);
    const char* format;

    if (sys_charset == NULL)
        sys_charset = GetSysCharset (FALSE);

    //SetRect (&msg_rc, 10, welcome_rc.bottom + 10, g_rcScr.right - 10, g_rcScr.bottom - 20);

    if (strcmp (sys_charset, FONT_CHARSET_GB2312_0) == 0 
            || strcmp (sys_charset, FONT_CHARSET_GBK) == 0
            || strcmp (sys_charset, FONT_CHARSET_GB18030_0) == 0) {
        format = "天马科技";
    }
    else if (strcmp (sys_charset, FONT_CHARSET_BIG5) == 0) {
        format = "天馬科技";
    }
    else {
        format = "TIAN MA TECH";
    }

    strcpy (msg_text, format);
}

static int logo_page_proc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
	static PLOGFONT logfont;
    HDC hdc;
	static int s_nProgress;
	static HWND s_hProgress;

    switch (message) {
        case MSG_CREATE:
			s_nProgress = 0;
            make_logo_text ();
			logfont = CreateLogFont ("ttf", "times", "ISO8859-1", 
                FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN, FONT_SETWIDTH_NORMAL,
                FONT_SPACING_CHARCELL, FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE, 
                36, 0);
            SetTimer (hWnd, 1000, 200);
            break;

        case MSG_TIMER:
			KillTimer(hWnd, 1000);
			PostMessage (hWnd, MSG_CLOSE, 0, 0);
            break;

        case MSG_PAINT:
            hdc = BeginPaint (hWnd);
			SetTextColor(hdc,COLOR_lightwhite);
			SetBkMode(hdc, BM_TRANSPARENT);
			SelectFont (hdc, logfont);
			TextOut (hdc, 102, 638, "TIAN MA TECH");
            EndPaint (hWnd, hdc);
            return 0;

        case MSG_CLOSE:
			DestroyAllControls(hWnd);
            DestroyMainWindow (hWnd);
            return 0;
    }
    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

int logo_page ()
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
    CreateInfo.MainWindowProc = logo_page_proc;
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

