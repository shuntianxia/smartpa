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
		TVBOX_Wait, 
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
		TVBOX_Lead, 
		350, 124, 
		66, 66,
		{"tvbox/guide.png",
		"tvbox/guide_focus.png",
		"tvbox/guide_focus.png"},
		0
    },
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		TVBOX_1, 
		55, 272, 
		66, 66,
		{"remote_control/1.png",
		"remote_control/1_focus.png",
		"remote_control/1_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		TVBOX_2, 
		208, 272, 
		66, 66,
		{"remote_control/2.png",
		"remote_control/2_focus.png",
		"remote_control/2_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		TVBOX_3, 
		350, 272, 
		66, 66,
		{"remote_control/3.png",
		"remote_control/3_focus.png",
		"remote_control/3_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		TVBOX_4, 
		55, 362, 
		66, 66,
		{"remote_control/4.png",
		"remote_control/4_focus.png",
		"remote_control/4_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		TVBOX_5, 
		208, 362, 
		66, 66,
		{"remote_control/5.png",
		"remote_control/5_focus.png",
		"remote_control/5_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		TVBOX_6, 
		350, 362, 
		66, 66,
		{"remote_control/6.png",
		"remote_control/6_focus.png",
		"remote_control/6_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		TVBOX_7, 
		55, 452, 
		66, 66,
		{"remote_control/7.png",
		"remote_control/7_focus.png",
		"remote_control/7_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		TVBOX_8, 
		208, 452, 
		66, 66,
		{"remote_control/8.png",
		"remote_control/8_focus.png",
		"remote_control/8_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		TVBOX_9, 
		350, 452, 
		66, 66,
		{"remote_control/9.png",
		"remote_control/9_focus.png",
		"remote_control/9_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		TVBOX_Menu, 
		55, 542, 
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
		TVBOX_0, 
		208, 542, 
		66, 66,
		{"remote_control/0.png",
		"remote_control/0_focus.png",
		"remote_control/0_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		TVBOX_Back, 
		350, 542, 
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
		TVBOX_Up, 
		210, 660, 
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
		TVBOX_Down, 
		210, 858, 
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
		TVBOX_Left, 
		121, 748, 
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
		TVBOX_Right, 
		320, 748, 
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
		TVBOX_Comfirm, 
		205, 743, 
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
		TVBOX_ChAdd, 
		65, 926, 
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
		TVBOX_ChRed, 
		65, 1022, 
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
		TVBOX_VolAdd, 
		349, 926, 
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
		TVBOX_VolRed, 
		349, 1022, 
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

static int tvbox_proc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
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
        	register_pic("res/tvbox/");
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
            FillBoxWithBitmap (hdc, 55, 914, 86, 186, GetBitmapFromRes(Str2Key("tv/+-bg.png")));
            FillBoxWithBitmap (hdc, 339, 914, 86, 186, GetBitmapFromRes(Str2Key("tv/+-bg.png")));
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
        	un_register_pic("res/tvbox/");
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
    pCreateInfo->MainWindowProc = tvbox_proc;
    pCreateInfo->lx = 0; 
    pCreateInfo->ty = 0;
    pCreateInfo->rx = 480;
    pCreateInfo->by = 1280;
    pCreateInfo->iBkColor = GetWindowElementColor (WE_MAINC_THREED_BODY);
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

void tvbox (HWND hosting, dev_info_t *dev_info)
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
