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
		AIR_Power, 
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
		AIR_Mode, 
		208, 124, 
		66, 66,
		{"remote_control/mode.png",
		"remote_control/mode_focus.png",
		"remote_control/mode_focus.png"},
		0
	},
    {
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		AIR_Mute, 
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
		AIR_Sleep, 
		55, 272, 
		66, 66,
		{"air_cleaner/sleep.png",
		"air_cleaner/sleep_focus.png",
		"air_cleaner/sleep_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		AIR_Anion, 
		208, 272, 
		66, 66,
		{"air_cleaner/anion.png",
		"air_cleaner/anion_focus.png",
		"air_cleaner/anion_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		AIR_UV, 
		350, 272, 
		66, 66,
		{"air_cleaner/uv.png",
		"air_cleaner/uv_focus.png",
		"air_cleaner/uv_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		AIR_AirVol, 
		55, 387, 
		66, 66,
		{"fan/low_speed.png",
		"fan/low_speed.png",
		"fan/low_speed.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		AIR_Natural, 
		208, 387, 
		66, 66,
		{"air_cleaner/natural.png",
		"air_cleaner/natural_focus.png",
		"air_cleaner/natural_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		AIR_Comfort, 
		350, 387, 
		66, 66,
		{"air_cleaner/comfortable.png",
		"air_cleaner/comfortable_focus.png",
		"air_cleaner/comfortable_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		AIR_Timer, 
		55, 502, 
		66, 66,
		{"air_cleaner/timer.png",
		"air_cleaner/timer_focus.png",
		"air_cleaner/timer_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		AIR_Strong, 
		208, 502, 
		66, 66,
		{"air_cleaner/strong.png",
		"air_cleaner/strong_focus.png",
		"air_cleaner/strong_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		AIR_Smart, 
		350, 502, 
		66, 66,
		{"air_cleaner/smart.png",
		"air_cleaner/smart_focus.png",
		"air_cleaner/smart_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		AIR_Light1, 
		55, 617, 
		66, 66,
		{"air_cleaner/light.png",
		"air_cleaner/light_focus.png",
		"air_cleaner/light_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		AIR_Light2, 
		208, 617, 
		66, 66,
		{"air_cleaner/light.png",
		"air_cleaner/light_focus.png",
		"air_cleaner/light_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		AIR_Light3, 
		350, 617, 
		66, 66,
		{"air_cleaner/light.png",
		"air_cleaner/light_focus.png",
		"air_cleaner/light_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		AIR_Auto, 
		55, 732, 
		66, 66,
		{"air_cleaner/auto.png",
		"air_cleaner/auto_focus.png",
		"air_cleaner/auto_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		AIR_LightOff, 
		208, 732, 
		66, 66,
		{"air_cleaner/light_off.png",
		"air_cleaner/light_off_focus.png",
		"air_cleaner/light_off_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		AIR_Close, 
		350, 732, 
		66, 66,
		{"air_cleaner/off.png",
		"air_cleaner/off_focus.png",
		"air_cleaner/off_focus.png"},
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

static int air_cleaner_proc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
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
			register_pic("res/air_cleaner/");
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
        	un_register_pic("res/air_cleaner/");
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
    pCreateInfo->MainWindowProc = air_cleaner_proc;
    pCreateInfo->lx = 0; 
    pCreateInfo->ty = 0;
    pCreateInfo->rx = 480;
    pCreateInfo->by = 1280;
    pCreateInfo->iBkColor = GetWindowElementColor (WE_MAINC_THREED_BODY);
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

void air_cleaner (HWND hosting, dev_info_t *dev_info)
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
