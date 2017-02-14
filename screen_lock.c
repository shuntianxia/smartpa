#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

typedef struct _WATCH_INFO{
    int k_h;
    int k_m;
    int k_s;
    RECT clock;
    RECT year;
    RECT month;
    RECT day;
    RECT wday;
    RECT first_bar;
    RECT second_bar;
}WATCH_INFO;

static WATCH_INFO g_watch;

static void init_all_rect(void)
{
    SetRect(&g_watch.clock,90,314,90+300,314+300);
    SetRect(&g_watch.year,111+24*3,732,24,36);
    SetRect(&g_watch.month,111+24*6,732,24,36);
    SetRect(&g_watch.day,111+24*9,732,24,36);
    SetRect(&g_watch.first_bar,111+24*4,732,24,36);
    SetRect(&g_watch.second_bar,111+24*7,732,24,36);
    SetRect(&g_watch.wday,209,785,66,31);
}

static void draw_time(HDC hdc)
{
    time_t timep;
    struct tm *p;
    int year, month;
	RECT year_rect;
	char pic_file[64];

    time(&timep);
    p = localtime(&timep);

	g_watch.k_h = (p->tm_hour%12)*5*6 + (p->tm_min/12)*6;
    g_watch.k_m = p->tm_min*6 + p->tm_sec/10;
	g_watch.k_s = p->tm_sec * 6;

	sprintf(pic_file, "screen_lock/clock/hour_pointer.png");
	PivotBitmap(hdc,GetBitmapFromRes(Str2Key(pic_file)),90+150,314+150,20,150,64*g_watch.k_h);
	sprintf(pic_file, "screen_lock/clock/minute_pointer.png");
	PivotBitmap(hdc,GetBitmapFromRes(Str2Key(pic_file)),90+150,314+150,20,150,64*g_watch.k_m);
	sprintf(pic_file, "screen_lock/clock/second_pointer.png");
	PivotBitmap(hdc,GetBitmapFromRes(Str2Key(pic_file)),90+150,314+150,20,150,64*g_watch.k_s);
	FillBoxWithBitmap(hdc,90,314,300,300,GetBitmapFromRes(Str2Key("screen_lock/clock/dial_plate.png")));

	year = p->tm_year + 1900;
	month = p->tm_mon + 1;
	year_rect = g_watch.year;
	while(year != 0)
	{
		sprintf(pic_file, "screen_lock/date/date_%d.png", year%10);
		FillBoxWithBitmap(hdc,year_rect.left,year_rect.top,24,36,GetBitmapFromRes(Str2Key(pic_file)));
		year_rect.left -= 24;
		year /= 10;
	}

	sprintf(pic_file, "screen_lock/date/date_%d.png", month%10);
	FillBoxWithBitmap(hdc,g_watch.month.left,g_watch.month.top,24,36,GetBitmapFromRes(Str2Key(pic_file)));
	sprintf(pic_file, "screen_lock/date/date_%d.png", month/10);
	FillBoxWithBitmap(hdc,g_watch.month.left - 24,g_watch.month.top,24,36,GetBitmapFromRes(Str2Key(pic_file)));

	sprintf(pic_file, "screen_lock/date/date_%d.png", p->tm_mday%10);
	FillBoxWithBitmap(hdc,g_watch.day.left,g_watch.day.top,24,36,GetBitmapFromRes(Str2Key(pic_file)));
	sprintf(pic_file, "screen_lock/date/date_%d.png", p->tm_mday/10);
	FillBoxWithBitmap(hdc,g_watch.day.left - 24,g_watch.day.top,24,36,GetBitmapFromRes(Str2Key(pic_file)));

	sprintf(pic_file, "screen_lock/date/-.png");
	FillBoxWithBitmap(hdc,g_watch.first_bar.left,g_watch.first_bar.top,24,36,GetBitmapFromRes(Str2Key(pic_file)));
	FillBoxWithBitmap(hdc,g_watch.second_bar.left,g_watch.second_bar.top,24,36,GetBitmapFromRes(Str2Key(pic_file)));

	sprintf(pic_file, "screen_lock/week/%d.png", p->tm_wday);
	FillBoxWithBitmap(hdc,g_watch.wday.left,g_watch.wday.top,0,0,GetBitmapFromRes(Str2Key(pic_file)));
}

static int screen_lock_proc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    switch (message) {
        case MSG_CREATE:
        	register_pic("res/screen_lock/");
			register_pic("res/screen_lock/clock/");
			register_pic("res/screen_lock/date/");
			register_pic("res/screen_lock/week/");
            memset(&g_watch, 0, sizeof(WATCH_INFO));
            init_all_rect();
            SetTimer(hWnd, 100, 100);
            return 0;

        case MSG_PAINT:
            hdc = BeginPaint (hWnd);
            draw_time(hdc);
            EndPaint (hWnd, hdc);
            return 0;

		case MSG_ERASEBKGND:
		{
			erase_bkgnd_with_pic(hWnd, (HDC)wParam, (const RECT*) lParam, 0);
			return 0;
		}

        case MSG_TIMER:
            InvalidateRect(hWnd, &g_watch.clock, TRUE);
            break;

        case MSG_KEYDOWN:
            PostMessage (hWnd, MSG_CLOSE, 0, 0);
            break;

        case MSG_CLOSE:
        	un_register_pic("res/screen_lock/");
			un_register_pic("res/screen_lock/clock/");
			un_register_pic("res/screen_lock/date/");
			un_register_pic("res/screen_lock/week/");
            KillTimer (hWnd, 100);
            DestroyMainWindow (hWnd);
            return 0;
    }
    return DefaultMainWinProc (hWnd, message, wParam, lParam);
}

HWND screen_lock (HWND hosting)
{
    MSG Msg;
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;

    CreateInfo.dwStyle = WS_NONE;
    CreateInfo.dwExStyle = WS_EX_TOPMOST | WS_EX_AUTOSECONDARYDC;
    CreateInfo.spCaption = "screen_lock";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = screen_lock_proc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = 480;
    CreateInfo.by = 1280;
    CreateInfo.iBkColor = PIXEL_lightwhite;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = hosting;
    
    hMainWnd = CreateMainWindow (&CreateInfo);
    
    if (hMainWnd != HWND_INVALID) {
		ShowWindow (hMainWnd, SW_SHOWNORMAL);
    }
	return hMainWnd;
#if 0
    while (GetMessage (&Msg, hMainWnd)) {
        TranslateMessage (&Msg);
        DispatchMessage (&Msg);
    }

    MainWindowThreadCleanup (hMainWnd);
#endif
    return 0;
}

