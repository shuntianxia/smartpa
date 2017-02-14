#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include "taskbar.h"
#include "common.h"

static BOOL quit = FALSE;
static BITMAP bmp;
static HWND hwnd_scnlock = HWND_INVALID;

static struct {
	int svr_wait_time;
	int svr_left; //screen saver waiting time left
}screen_saver;

#define ResetScreenSaver()  (screen_saver.svr_left = screen_saver.svr_wait_time)
#define DecreaseScreenSaver(hWnd)  do{ \
	if(screen_saver.svr_wait_time > 0 && hwnd_scnlock == HWND_INVALID) { \
		screen_saver.svr_left -= 1; \
		if(screen_saver.svr_left <= 0){ \
			StartScreenSaver(hWnd); \
		} \
	} \
}while(0)

static void StartScreenSaver(HWND hWnd)
{
	if(hwnd_scnlock == HWND_INVALID) { 
		hwnd_scnlock = screen_lock(hWnd);
	}
}

static void StopScreenSaver()
{
	if(hwnd_scnlock != HWND_INVALID){
		SendNotifyMessage (hwnd_scnlock, MSG_CLOSE, 0, 0);
		hwnd_scnlock = HWND_INVALID;
		screen_saver.svr_left = screen_saver.svr_wait_time;
	}
}

static void GetScreenSaverInfoFromEtc()
{
	char szvalue[128];
#define GET_STR(key) (GetValueFromEtcFile(CFGFILE, "screensaver", key, szvalue, sizeof(szvalue)-1)==ETC_OK? \
	strdup(szvalue):NULL)
#define GET_INT(key, value) GetIntValueFromEtcFile(CFGFILE, "screensaver", key, &value)
	
	StopScreenSaver();
	memset(&screen_saver, 0 ,sizeof(screen_saver));
	GET_INT("waittime",screen_saver.svr_wait_time);
	screen_saver.svr_left = screen_saver.svr_wait_time;

#undef GET_STR
#undef GET_INT
}

static BOOL drag_time_proc(HWND hwnd, int id, DWORD count)
{
	InvalidateRect(hwnd, NULL, TRUE);
    DecreaseScreenSaver(hwnd);
	return TRUE;
}
//////////////////////////////////////////////

static int my_event_hook (void* context, HWND dst_wnd, 
                int msg, WPARAM wparam, LPARAM lparam)
{
	if((msg >= MSG_FIRSTMOUSEMSG && msg <= MSG_LASTMOUSEMSG) || (msg >= MSG_FIRSTKEYMSG && msg <= MSG_LASTKEYMSG))
	{
        ResetScreenSaver();
		StopScreenSaver();
	}

    return HOOK_GOON;
}

static char* mk_time (char* buff)
{
    time_t t;
    struct tm * tm;

    time (&t);
    tm = localtime (&t);
    sprintf (buff, "%02d:%02d:%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);

    return buff;
}

static void taskbar_draw(HDC hdc)
{
	char buff [20];
	SetBkMode(hdc,BM_TRANSPARENT);
	SetTextColor(hdc,COLOR_lightwhite);
	SelectFont(hdc, font30);
	TextOut(hdc,349,22,mk_time (buff));
}

static int TaskBarWinProc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case MSG_CREATE:
    {
		HDC hdc_dsk = GetClientDC(HWND_DESKTOP);
		if(!GetBitmapFromDC(hdc_dsk, 0, 0, 480, HEIGHT_TASKBAR, &bmp)) {
			printf("failed\n");
		}
		ReleaseDC(hdc_dsk);

#ifdef _MGTIMER_UNIT_10MS
        //SetTimer (hWnd, _ID_TIMER, 100);
        //SetTimerEx (hWnd, _ID_TIMER, 100, drag_time_proc);
#else
        //SetTimer (hWnd, _ID_TIMER, 10);
        //SetTimerEx (hWnd, _ID_TIMER, 10, drag_time_proc);
#endif
        break;
    }
    
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
					RECTW(rcTemp), RECTH(rcTemp), &bmp);
		
		if (fGetDC)
			ReleaseDC (hdc);
		return 0;
	}

    case MSG_COMMAND:
    {
        int code = HIWORD (wParam);
        int id   = LOWORD (wParam);

    }
    break;

    case MSG_PAINT:
    {
    	HDC hdc;
		hdc = BeginPaint (hWnd);
		//taskbar_draw(hdc);
		EndPaint (hWnd, hdc);
		break;
    }
        
    case MSG_CLOSE:
        KillTimer (hWnd, _ID_TIMER);
        UnloadBitmap(&bmp);
        DestroyAllControls (hWnd);
        DestroyMainWindow (hWnd);
        PostQuitMessage (hWnd);
        return 0;
    }

    return DefaultMainWinProc (hWnd, message, wParam, lParam);
}

HWND create_task_bar (void)
{
    MAINWINCREATE CreateInfo;
    HWND hTaskBar;

    CreateInfo.dwStyle = WS_ABSSCRPOS | WS_VISIBLE;
    CreateInfo.dwExStyle = WS_EX_TOOLWINDOW | WS_EX_AUTOSECONDARYDC;
    CreateInfo.spCaption = "TaskBar" ;
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor (0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = TaskBarWinProc;
    CreateInfo.lx = g_rcScr.left; 
    CreateInfo.ty = g_rcScr.top;
    CreateInfo.rx = g_rcScr.right;
    CreateInfo.by = g_rcScr.top + HEIGHT_TASKBAR;
    CreateInfo.iBkColor = PIXEL_darkgray; 
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;
    
	GetScreenSaverInfoFromEtc();
	RegisterMouseMsgHook(0, my_event_hook); 

    hTaskBar = CreateMainWindow (&CreateInfo);

    return hTaskBar;
}

