#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include "setup_wizard.h"
#include "button_ex.h"
#include "register_res.h"
//#include "mg_common_header.h"
#include "font.h"

static BITMAP backbitmap;
static BITMAP digibitmap;
static HWND hScrollView;

const char *language[] =
{
    "English",
    "Chinese",
    "Japanese",
};

static buttonex_info_t button_next =
{
	"NEXT",
	WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
	WS_EX_TRANSPARENT,
	IDC_NEXT, 
	52, 944, 
	296, 70,
	{"test/slide-to-unlock-_bg.png",
	"test/slide-to-unlock-_bg.png",
	"test/slide-to-unlock-_bg.png",
	0},
	0
};

static CTRLDATA CtrlBook[] =
{
    {
        "scrollview",
        WS_BORDER | WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL |
                SVS_AUTOSORT,
         10, 10, 320, 150,
        IDC_SCROLLVIEW,
        "",
        0
    },
};

static int my_usleep(useconds_t usec){
    unsigned int now = times(NULL);
    unsigned int end = now + usec / (1000 * 10);
    while (((int)(end - now)) > 0) {
        usleep(((int)(end - now)) * 1000 * 10);
        now = times(NULL);
    }
    return 0;
}

static void myDrawItem (HWND hWnd, HSVITEM hsvi, HDC hdc, RECT *rcDraw)
{
    const char *name = (const char*)scrollview_get_item_adddata (hsvi);

    SetBkMode (hdc, BM_TRANSPARENT);
    SetTextColor (hdc, PIXEL_black);
	SelectFont (hdc, font20);

    if (scrollview_is_item_hilight(hWnd, hsvi)) {
        SetTextColor (hdc, PIXEL_lightwhite);
    }
    TextOut (hdc, rcDraw->left + 3, rcDraw->top + 2, name);
}

static int myCmpItem (HSVITEM hsvi1, HSVITEM hsvi2)
{
    const char *name1 = (const char*)scrollview_get_item_adddata (hsvi1);
    const char *name2 = (const char*)scrollview_get_item_adddata (hsvi2);

    return strcmp (name1, name2);
}

static int setup_wizard_proc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
	WORD keycode = -1;

    switch (message) {
        case MSG_CREATE:
		{
			SVITEMINFO svii;
			int i = 0;
            hScrollView = CreateWindowEx ("scrollview",
                          "", 
                          WS_CHILD | WS_VISIBLE,
                          WS_EX_TRANSPARENT,
                          IDC_SCROLLVIEW, 
                          110, 512, 320, 150, hWnd, 0);

			CreateButtonEx(&button_next, hWnd);

	        SetWindowBkColor (hScrollView, PIXEL_lightwhite);

	        SendMessage (hScrollView, SVM_SETITEMCMP, 0, (LPARAM)myCmpItem);
	        SendMessage (hScrollView, SVM_SETITEMDRAW, 0, (LPARAM)myDrawItem);

	        for (i = 0; i < TABLESIZE(language); i++) {
	            svii.nItemHeight = 32;
	            svii.addData = (DWORD)language[i];
	            svii.nItem = i;
	            SendMessage (hScrollView, SVM_ADDITEM, 0, (LPARAM)&svii);
	            SendMessage (hScrollView, SVM_SETITEMADDDATA, i, (DWORD)language[i]);
	        }
            return 0;
        }

        case MSG_PAINT:
            hdc = BeginPaint (hWnd);
			SetTextColor(hdc,COLOR_lightwhite);
			SetBkMode(hdc, BM_TRANSPARENT);
			SelectFont (hdc, font20);
			TextOut (hdc, 30, 140, "Please select language");
            EndPaint (hWnd, hdc);
            break;

		case MSG_COMMAND:
		{
			int id = LOWORD (wParam);
			int code = HIWORD (wParam);
		
			switch (id) {
	
				case IDC_SCROLLVIEW:
					if (code == SVN_CLICKED) {
						int sel;
						const char *info;
						sel = SendMessage (hScrollView, SVM_GETCURSEL, 0, 0);
						info = (const char *)SendMessage (hScrollView, SVM_GETITEMADDDATA, sel, 0);
					}
					break;
					
				case IDC_NEXT:
					PostMessage(hWnd, MSG_CLOSE, 0, 0);
					//date_select();
					break;
			}
			break;
		}

		case MSG_KEYDOWN:
			keycode = LOWORD(wParam);
			if (wParam == SCANCODE_REMOVE) {
				int idx = SendMessage (hScrollView, SVM_GETCURSEL, 0, 0);
				SendMessage (hScrollView, SVM_DELITEM, idx, 0);
				SendMessage (hScrollView, SVM_SETCURSEL, idx, 0);
				return 0;
			}
			//if (SYME_KEY_ESC == keycode) {
				//PostMessage(hWnd, MSG_CLOSE, 0, 0);
			//}
			break;

        case MSG_CLOSE:
            DestroyMainWindow (hWnd);
            return 0;
    }

    return DefaultMainWinProc (hWnd, message, wParam, lParam);
}

int setup_wizard (HWND hosting)
{
    MSG Msg;
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;

    CreateInfo.dwStyle = WS_NONE;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = "";
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = setup_wizard_proc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = 400;
    CreateInfo.by = 1280;
    CreateInfo.iBkColor = COLOR_darkgray;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = hosting;
    
    hMainWnd = CreateMainWindow (&CreateInfo);
    
    if (hMainWnd == HWND_INVALID) {
        return -1;
    }
	ShowWindow(hMainWnd, SW_SHOWNORMAL);
	while (GetMessage (&Msg, hMainWnd)) {
		TranslateMessage (&Msg);
		DispatchMessage (&Msg);
	}
	MainWindowThreadCleanup (hMainWnd);

    return 0;
}

