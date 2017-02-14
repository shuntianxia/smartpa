#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include "dev_info.h"
#include "button_ex.h"

#define IDC_BACK 100
#define IDC_SETTING 101

static HWND hMainWnd = HWND_INVALID;

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
		DVD_Power, 
		55, 124, 
		66, 66,
		{"remote_control/power.png",
		"remote_control/power_focus.png",
		"remote_control/power_focus.png"},
		0
    },
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		DVD_Format, 
		208, 124, 
		66, 66,
		{"dvd/standard.png",
		"dvd/standard_focus.png",
		"dvd/standard_focus.png"},
		0
	},
    {
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		DVD_SK, 
		350, 124, 
		66, 66,
		{"dvd/open_door.png",
		"dvd/open_door_focus.png",
		"dvd/open_door_focus.png"},
		0
    },
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		DVD_Title, 
		55, 272, 
		66, 66,
		{"dvd/title.png",
		"dvd/title_focus.png",
		"dvd/title_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		DVD_Menu, 
		208, 272, 
		66, 66,
		{"remote_control/menu.png",
		"remote_control/menu_focus.png",
		"remote_control/menu_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		DVD_Mute, 
		350, 272, 
		66, 66,
		{"remote_control/mute.png",
		"remote_control/mute_focus.png",
		"remote_control/mute_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		DVD_Pause, 
		55, 398, 
		66, 66,
		{"dvd/pause.png",
		"dvd/pause_focus.png",
		"dvd/pause_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		DVD_Play, 
		208, 398, 
		66, 66,
		{"dvd/play.png",
		"dvd/play_focus.png",
		"dvd/play_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		DVD_Stop, 
		350, 398, 
		66, 66,
		{"dvd/stop.png",
		"dvd/stop_focus.png",
		"dvd/stop_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		DVD_Prev, 
		57, 542, 
		66, 66,
		{"dvd/prev.png",
		"dvd/prev_focus.png",
		"dvd/prev_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		DVD_Next, 
		153, 542, 
		66, 66,
		{"dvd/fast_backward.png",
		"dvd/fast_backward_focus.png",
		"dvd/fast_backward_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		DVD_FForwad, 
		250, 542, 
		66, 66,
		{"dvd/fast_forward.png",
		"dvd/fast_forward_focus.png",
		"dvd/fast_forward_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		DVD_Next, 
		350, 542, 
		66, 66,
		{"dvd/next.png",
		"dvd/next_focus.png",
		"dvd/next_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		DVD_Back, 
		208, 686, 
		66, 66,
		{"remote_control/return.png",
		"remote_control/return_focus.png",
		"remote_control/return_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		DVD_Up, 
		210, 842, 
		80, 60,
		{"remote_control/up.png",
		"remote_control/up_focus.png",
		"remote_control/up_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		DVD_Down, 
		210, 1040, 
		80, 60,
		{"remote_control/down.png",
		"remote_control/down_focus.png",
		"remote_control/down_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		DVD_Left, 
		121, 930, 
		60, 80,
		{"remote_control/left.png",
		"remote_control/left_focus.png",
		"remote_control/left_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		DVD_Right, 
		320, 930, 
		60, 80,
		{"remote_control/right.png",
		"remote_control/right_focus.png",
		"remote_control/right_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		DVD_Ok, 
		205, 925, 
		92, 92,
		{"remote_control/ok.png",
		"remote_control/ok_focus.png",
		"remote_control/ok_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		IDC_SETTING, 
		382, 1186, 
		48, 48,
		{"common/setting.png",
		"common/setting_focus.png",
		"common/setting_focus.png"},
		0
	}
};

static int dvd_proc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{	
	switch (message)
	{
		case MSG_ERASEBKGND:
		{
			erase_bkgnd_with_pic(hWnd, (HDC)wParam, (const RECT*) lParam, 0);
			return 0;
		}

		case MSG_CREATE:
		{	
			int i;
			register_pic("res/remote_control/");
			register_pic("res/dvd/");
			for ( i = 0; i < TABLESIZE(buttonex_info); i++ )
			{
				CreateButtonEx(&buttonex_info[i], hWnd);
			}		
			return 0;
		}

		case MSG_PAINT:
		{
			HDC hdc;
            hdc = BeginPaint (hWnd);
            EndPaint (hWnd, hdc);
            return 0;
		}
		
		case MSG_COMMAND:
		{
			dev_info_t *dev_info = (dev_info_t *)GetWindowAdditionalData(hWnd);
			int id = LOWORD(wParam);
			int nCode = HIWORD(wParam);
			
			if(id == IDC_BACK) {
				PostMessage(hWnd, MSG_CLOSE, 0, 0);
			} else if(id == IDC_SETTING) {
				//
			} else {
				send_ir_code_general(dev_info->ir_code,dev_info->code_len, id);
			}
			break;
		}
		
        case MSG_DESTROY:
            DestroyAllControls (hWnd);
            hMainWnd = HWND_INVALID;
        	return 0;

        case MSG_CLOSE:
        	un_register_pic("res/remote_control/");
			un_register_pic("res/dvd/");
            DestroyMainWindow (hWnd);
            MainWindowCleanup (hWnd);
            return 0;		
	} 
	return DefaultMainWinProc (hWnd, message, wParam, lParam);
} 

static void InitCreateInfo(PMAINWINCREATE pCreateInfo)
{
    pCreateInfo->dwStyle = WS_VISIBLE | WS_CHILD;
    pCreateInfo->dwExStyle = WS_EX_TOPMOST;
    pCreateInfo->spCaption = "";
    pCreateInfo->hMenu = 0;
    pCreateInfo->hCursor = GetSystemCursor(1);
    pCreateInfo->hIcon = 0;
    pCreateInfo->MainWindowProc = dvd_proc;
    pCreateInfo->lx = 0; 
    pCreateInfo->ty = 0;
    pCreateInfo->rx = 480;
    pCreateInfo->by = 1280;
    pCreateInfo->iBkColor = GetWindowElementColor (WE_MAINC_THREED_BODY);
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

void dvd (HWND hosting, dev_info_t *dev_info)
{
    MAINWINCREATE CreateInfo;
    MSG Msg;

    if (hMainWnd != HWND_INVALID) {
        ShowWindow (hMainWnd, SW_SHOWNORMAL);
        return;
    }

    InitCreateInfo (&CreateInfo);
    CreateInfo.hHosting = hosting;
    CreateInfo.dwAddData = (DWORD)dev_info;

    hMainWnd = CreateMainWindow (&CreateInfo);
    if (hMainWnd == HWND_INVALID)
        return;

    ShowWindow (hMainWnd, SW_SHOWNORMAL);

    while (GetMessage (&Msg, hMainWnd)) {
    	TranslateMessage(&Msg);
        DispatchMessage (&Msg);
    }
    MainWindowThreadCleanup (hMainWnd);
}
