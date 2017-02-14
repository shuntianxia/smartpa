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

typedef struct air_cdt_s {
	int temp;
	int wind_volume;
	int wind_dir;
}air_cdt_t;

static air_cdt_t air_cdt;
static RECT temp_rect;
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
		ARC_Power, 
		39, 702, 
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
		ARC_Mode, 
		183, 702, 
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
		ARC_Vol, 
		39, 883, 
		66, 66,
		{"air_condition/wind_speed.png",
		"air_condition/wind_speed_focus.png",
		"air_condition/wind_speed_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		ARC_M, 
		183, 883, 
		66, 66,
		{"air_condition/wind_direction.png",
		"air_condition/wind_direction_focus.png",
		"air_condition/wind_direction_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		ARC_tmpAdd, 
		316, 698, 
		110, 128,
		{"remote_control/temp_up.png",
		"remote_control/temp_up_focus.png",
		"remote_control/temp_up_focus.png"},
		0
	},
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		ARC_tmpRed, 
		316, 826, 
		110, 128,
		{"remote_control/temp_down.png",
		"remote_control/temp_down_focus.png",
		"remote_control/temp_down_focus.png"},
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

static void init_all_rect(void)
{
    SetRect(&temp_rect,126,378,126+228,378+108);
}

static void update_status(HDC hdc)
{
	char bmp[64];

	snprintf(bmp, sizeof(bmp), "air_condition/mode_auto.png");
	FillBoxWithBitmap(hdc, 32, 129, 66, 66, GetBitmapFromRes(Str2Key(bmp)));
	
	snprintf(bmp, sizeof(bmp), "air_condition/wind_volume_1.png");
	FillBoxWithBitmap(hdc, 207, 129, 66, 66, GetBitmapFromRes(Str2Key(bmp)));
	
	snprintf(bmp, sizeof(bmp), "air_condition/wind_dir_medium.png");
	FillBoxWithBitmap(hdc, 362, 129, 66, 66, GetBitmapFromRes(Str2Key(bmp)));

	sprintf(bmp, "air_condition/temperature_%d.png", air_cdt.temp/10);
	FillBoxWithBitmap(hdc,temp_rect.left,temp_rect.top,0,0,GetBitmapFromRes(Str2Key(bmp)));
	sprintf(bmp, "air_condition/temperature_%d.png", air_cdt.temp%10);
	FillBoxWithBitmap(hdc,temp_rect.left+78,temp_rect.top,0,0,GetBitmapFromRes(Str2Key(bmp)));
	sprintf(bmp, "air_condition/temperature_degree.png");
	FillBoxWithBitmap(hdc,temp_rect.left+78*2,416,0,0,GetBitmapFromRes(Str2Key(bmp)));
	sprintf(bmp, "air_condition/temperature_c.png");
	FillBoxWithBitmap(hdc,temp_rect.left+78*2+20,436,0,0,GetBitmapFromRes(Str2Key(bmp)));

}

static int air_condition_proc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
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
			register_pic("res/air_condition/");
			for ( i = 0; i < TABLESIZE(buttonex_info); i++ )
			{
				CreateButtonEx(&buttonex_info[i], hWnd);
			}
			init_all_rect();
			air_cdt.temp = 23;
			return 0;
		}

		case MSG_PAINT:
		{
			HDC hdc;
            hdc = BeginPaint (hWnd);
            update_status(hdc);
            EndPaint (hWnd, hdc);
            return 0;
		}
		
		case MSG_COMMAND:
		{
			dev_info_t *dev_info = (dev_info_t *)GetWindowAdditionalData(hWnd);
			arc_info_t arc_info;
			int id = LOWORD(wParam);
			int nCode = HIWORD(wParam);
			
			if(id == IDC_BACK) {
				PostMessage(hWnd, MSG_CLOSE, 0, 0);
			} else if(id == IDC_SETTING) {
				//
			} else {
				switch (LOWORD(wParam))
				{
					case IDC_BACK:
						PostMessage(hWnd, MSG_CLOSE, 0, 0);
						break;

					case ARC_tmpAdd:
						if(air_cdt.temp < 30) {
							air_cdt.temp += 1;
							InvalidateRect(hWnd, &temp_rect, TRUE);
						}
						break;
						
					case ARC_tmpRed:
						if(air_cdt.temp > 17) {
							air_cdt.temp -= 1;
							InvalidateRect(hWnd, &temp_rect, TRUE);
						}
						break;
				}
				send_ir_code_arc(dev_info->ir_code,dev_info->code_len, &arc_info);
			}
			break;					
		}
		
        case MSG_DESTROY:
            DestroyAllControls (hWnd);
            hMainWnd = HWND_INVALID;
        	return 0;

        case MSG_CLOSE:
        	un_register_pic("res/remote_control/");
        	un_register_pic("res/air_condition/");
            DestroyMainWindow (hWnd);
            MainWindowCleanup (hWnd);
            return 0;		
	} 
	return DefaultMainWinProc (hWnd, message, wParam, lParam);
} 

static void InitCreateInfo(PMAINWINCREATE pCreateInfo)
{
    pCreateInfo->dwStyle = WS_VISIBLE | WS_CHILD;
    pCreateInfo->dwExStyle = WS_EX_TOPMOST | WS_EX_AUTOSECONDARYDC;
    pCreateInfo->spCaption = "";
    pCreateInfo->hMenu = 0;
    pCreateInfo->hCursor = GetSystemCursor(1);
    pCreateInfo->hIcon = 0;
    pCreateInfo->MainWindowProc = air_condition_proc;
    pCreateInfo->lx = 0; 
    pCreateInfo->ty = 0;
    pCreateInfo->rx = 480;
    pCreateInfo->by = 1280;
    pCreateInfo->iBkColor = GetWindowElementColor (WE_MAINC_THREED_BODY);
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

void air_condition (HWND hosting, dev_info_t *dev_info)
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
