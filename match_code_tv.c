#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include "button_ex.h"

enum idc_ma_en
{
	IDC_BASE = 0,
	IDC_BACK,
	IDC_VOLUME_UP,
	IDC_OK,
	IDC_NEXT,
	IDC_MAX
};

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
		IDC_VOLUME_UP, 
		187, 519, 
		108, 108,
		{"match_code/volume_up.png",
		"match_code/volume_up_focus.png",
		"match_code/volume_up_focus.png"},
		0
    },
    {
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		IDC_OK, 
		90, 1094, 
		120, 70,
		{"match_code/ok.png",
		"match_code/ok_focus.png",
		"match_code/ok_focus.png"},
		0
    },
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		IDC_NEXT, 
		270, 1094, 
		120, 70,
		{"match_code/next.png",
		"match_code/next_focus.png",
		"match_code/next_focus.png"},
		0
	}
};

static int match_code_tv_proc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{	
	switch (message)
	{
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
						   RECTW(rcTemp), RECTH(rcTemp), GetBitmapFromRes(Str2Key("bg.png")));
		
				   if (fGetDC)
					   ReleaseDC (hdc);
				   return 0;
			   }

		case MSG_CREATE:
		{	
			int i;
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
			int nCode = HIWORD(wParam);
			
			switch (LOWORD(wParam))
			{
				case IDC_BACK:
					PostMessage(hWnd, MSG_CLOSE, 0, 0);
					break;
					
				default: 
					break;
					
			}
			
			break;
		}
		
        case MSG_DESTROY:
            DestroyAllControls (hWnd);
            hMainWnd = HWND_INVALID;
        	return 0;

        case MSG_CLOSE:
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
    pCreateInfo->hCursor = GetSystemCursor(0);
    pCreateInfo->hIcon = 0;
    pCreateInfo->MainWindowProc = match_code_tv_proc;
    pCreateInfo->lx = 0; 
    pCreateInfo->ty = 0;
    pCreateInfo->rx = 480;
    pCreateInfo->by = 1280;
    pCreateInfo->iBkColor = GetWindowElementColor (WE_MAINC_THREED_BODY);
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

void match_code_tv (HWND hosting)
{
    MAINWINCREATE CreateInfo;

    if (hMainWnd != HWND_INVALID) {
        ShowWindow (hMainWnd, SW_SHOWNORMAL);
        return;
    }

    InitCreateInfo (&CreateInfo);
    CreateInfo.hHosting = hosting;

    hMainWnd = CreateMainWindow (&CreateInfo);
}
