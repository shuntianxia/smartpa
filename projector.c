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
		PJT_On, 
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
		PJT_Off, 
		208, 124, 
		66, 66,
		{"projector/shutdown.png",
		"projector/shutdown_focus.png",
		"projector/shutdown_focus.png"},
		0
	},
    {
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		PJT_Mute, 
		350, 124, 
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
		PJT_Computer, 
		55, 272, 
		66, 66,
		{"projector/pc.png",
		"projector/pc_focus.png",
		"projector/pc_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		PJT_Video, 
		208, 272, 
		66, 66,
		{"projector/video.png",
		"projector/video_focus.png",
		"projector/video_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		PJT_MCD, 
		350, 272, 
		66, 66,
		{"projector/brightness.png",
		"projector/brightness_focus.png",
		"projector/brightness_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		PJT_Auto, 
		55, 387, 
		66, 66,
		{"projector/auto.png",
		"projector/auto_focus.png",
		"projector/auto_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		PJT_Pause, 
		208, 387, 
		66, 66,
		{"remote_control/pause.png",
		"remote_control/pause_focus.png",
		"remote_control/pause_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		PJT_Quit, 
		350, 387, 
		66, 66,
		{"projector/quit.png",
		"projector/quit_focus.png",
		"projector/quit_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		PJT_Menu, 
		55, 502, 
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
		PJT_SignalSource, 
		350, 502, 
		66, 66,
		{"projector/source.png",
		"projector/source_focus.png",
		"projector/source_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		PJT_VolRed, 
		55, 616, 
		66, 66,
		{"projector/volume_down.png",
		"projector/volume_down_focus.png",
		"projector/volume_down_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		PJT_VolAdd, 
		350, 616, 
		66, 66,
		{"projector/volume_up.png",
		"projector/volume_up_focus.png",
		"projector/volume_up_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		PJT_Up, 
		210, 626, 
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
		PJT_Down, 
		210, 824, 
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
		PJT_Left, 
		121, 714, 
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
		PJT_Right, 
		320, 714, 
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
		PJT_Confirm, 
		205, 709, 
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
		PJT_PictureAdd, 
		65, 839, 
		64, 64,
		{"remote_control/+.png",
		"remote_control/+_focus.png",
		"remote_control/+_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		PJT_PictureRed, 
		65, 935, 
		64, 64,
		{"remote_control/-.png",
		"remote_control/-_focus.png",
		"remote_control/-_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		PJT_FocusAdd, 
		349, 839, 
		64, 64,
		{"remote_control/+.png",
		"remote_control/+_focus.png",
		"remote_control/+_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		PJT_FocusRed, 
		349, 935, 
		64, 64,
		{"remote_control/-.png",
		"remote_control/-_focus.png",
		"remote_control/-_focus.png"},
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

static int projector_proc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
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
			register_pic("res/projector/");
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
            FillBoxWithBitmap (hdc, 55, 827, 86, 186, GetBitmapFromRes(Str2Key("tv/+-bg.png")));
            FillBoxWithBitmap (hdc, 339, 827, 86, 186, GetBitmapFromRes(Str2Key("tv/+-bg.png")));
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
        	un_register_pic("res/projector/");
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
    pCreateInfo->MainWindowProc = projector_proc;
    pCreateInfo->lx = 0; 
    pCreateInfo->ty = 0;
    pCreateInfo->rx = 480;
    pCreateInfo->by = 1280;
    pCreateInfo->iBkColor = GetWindowElementColor (WE_MAINC_THREED_BODY);
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

void projector (HWND hosting, dev_info_t *dev_info)
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
