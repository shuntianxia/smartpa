/*
** $Id: scrollwnd2.c 741 2009-03-31 07:16:18Z weiym $
**
** scrollwnd2.c: Sample program for MiniGUI Programming Guide
**      Usage of SCROLLWND control.
**
** Copyright (C) 2004 ~ 2007 Feynman Software.
**
** License: GPL
*/


#include <stdio.h>
#include <stdlib.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include "register_res.h"
#include "button_ex.h"

#define IDC_SCROLLWND          100
#define ID_ZOOMIN              200
#define ID_ZOOMOUT             300

static buttonex_info_t buttonex_info [] =
{
    {
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		101, 
		20, 2, 
		34, 34,
		{RETURN_ARROW_NORMAL_PNG,
		RETURN_ARROW_FOCUSED_PNG,
		RETURN_ARROW_FOCUSED_PNG,
		0},
		0
	},
    {
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		102, 
		170, 2, 
		34, 34,
		{SWITCH_NORMAL_PNG,
		SWITCH_FOCUSED_PNG,
		SWITCH_FOCUSED_PNG,
		0},
		0
    },
	{
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		103, 
		320, 2, 
		34, 34,
		{SOURCE_NORMAL_PNG,
		SOURCE_FOCUSED_PNG,
		SOURCE_FOCUSED_PNG,
		0},
		0
	},
    {
		0,
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		104, 
		470, 2, 
		34, 34,
		{BUTTON_BG_NORMAL_PNG,
		BUTTON_BG_FOCUSED_PNG,
		BUTTON_BG_FOCUSED_PNG,
		0},
		NUMBER_1_PNG
    }
};

static int container_proc (HWND hWnd, int message, 
                WPARAM wParam, LPARAM lParam)
{
    switch (message) {
		case MSG_ERASEBKGND:
		{
			SetWindowExStyle(hWnd, WS_EX_TRANSPARENT);
			break;
		}
	}
    return DefaultContainerProc (hWnd, message, wParam, lParam);
}

#define IDC_PROMPTINFO	100
#define IDC_PROGRESS  	110

static CTRLDATA controls [] =
{ 
    {
        "button",
        WS_TABSTOP | WS_VISIBLE, 
        20, 20, 60, 60,
        IDOK, 
        "OK",
        0
    },
    {
        "button",
        WS_TABSTOP | WS_VISIBLE, 
        120, 20, 60, 60,
        IDCANCEL, 
        "CANCEL",
        0
    },
    {
        "button",
        WS_TABSTOP | WS_VISIBLE, 
        220, 20, 60, 60,
        IDABORT, 
        "ABORT",
        0
    },
    {
        "button",
        WS_TABSTOP | WS_VISIBLE, 
        320, 20, 60, 60,
        IDRETRY, 
        "RETRY",
        0
    },
    {
        "button",
        WS_TABSTOP | WS_VISIBLE, 
        420, 20, 60, 60,
        IDIGNORE, 
        "IGNORE",
        0
    }
};

static int dock_bar_proc (HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{

    switch (message) {
	case MSG_ERASEBKGND:
	{
	   HDC hdc = (HDC)wParam;
	   const RECT* clip = (const RECT*) lParam;
	   BOOL fGetDC = FALSE;
	   RECT rcTemp;
			   
	   if (hdc == 0) {
		   hdc = GetClientDC (hDlg);
		   fGetDC = TRUE;
	   }	   
			   
	   if (clip) {
		   rcTemp = *clip;
		   ScreenToClient (hDlg, &rcTemp.left, &rcTemp.top);
		   ScreenToClient (hDlg, &rcTemp.right, &rcTemp.bottom);
		   IncludeClipRect (hdc, &rcTemp);
	   }
	   else
		   GetClientRect (hDlg, &rcTemp);

	   FillBoxWithBitmap (hdc, 0, 0, 
			   RECTW(rcTemp), RECTH(rcTemp), GetBitmapFromRes(Str2Key("bkgnd.png")));

	   if (fGetDC)
		   ReleaseDC (hdc);
	   return 0;
	}

    case MSG_INITDIALOG:
    {
		int i;
        HWND hScrollWnd;

        hScrollWnd = GetDlgItem (hDlg, IDC_SCROLLWND);

        SendMessage (hScrollWnd, SVM_SETCONTAINERPROC, 0, 
                        (LPARAM)container_proc);
        SendMessage (hScrollWnd, SVM_SETCONTRANGE, 
                        800, 100);

        SendMessage (hScrollWnd, SVM_ADDCTRLS, 5, (LPARAM)controls);

		//for ( i = 0; i < TABLESIZE(buttonex_info); i++ )
		//{
			//CreateButtonEx(&buttonex_info[i], hScrollWnd);
		//}
        break;
    }

    case MSG_CLOSE:
        EndDialog (hDlg, 0);
        return 0;

    }

    return DefaultDialogProc (hDlg, message, wParam, lParam);
}

static CTRLDATA CtrlViewer[] =
{
    {
        "scrollwnd",
        WS_CHILD | WS_VISIBLE,
         0, 0, 400, 100,
        IDC_SCROLLWND,
        "Scroll Window",
        0,
        WS_EX_TRANSPARENT
    }
};

static DLGTEMPLATE DlgViewer =
{
    WS_NONE,
    WS_EX_TRANSPARENT,
    0, 0, 400, 1280,
    "dock bar",
    0, 0,
    TABLESIZE(CtrlViewer), CtrlViewer,
    0
};

int dock_test ()
{
    DialogBoxIndirectParam (&DlgViewer, HWND_DESKTOP, dock_bar_proc, 0L);

    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif

