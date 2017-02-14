#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>
#include <minigui/ctrl/wheelview.h>
#include <mgeff/mgeff.h>

#include "button_ex.h"
#include "register_res.h"
#include "font.h"

#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 1280
#define IDC_NEXT 0
long g_distance;
#define ITEM_HEIGHT       48
#define VISIBLE_NUM       5
#define IDC_YEARWHEEL    100
static HWND hYearWheel;
#define IDC_MONTHWHEEL    101
static HWND hMonthWheel;
#define IDC_DAYWHEEL    102
static HWND hDayWheel;

struct COLOR_LEVEL {
    int isCurColor;
    DWORD color;
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

static void myDrawItem (HWND hWnd, HSVITEM hsvi, HDC hdc, RECT *rcDraw)
{
    const char *name = (const char*)scrollview_get_item_adddata (hsvi);
    int x,y;
    int i;
    long cur_center;
    SIZE font_pixel;
    int uFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE ;
   
    g_distance = SendMessage(hWnd, SVM_GETMOVEDIS, 0, 0);
    long hilight_center = ITEM_HEIGHT*(VISIBLE_NUM/2) + ITEM_HEIGHT/2 - g_distance;
    struct COLOR_LEVEL color_level[VISIBLE_NUM/2+1];
 
    SetBkMode (hdc, BM_TRANSPARENT);

    //SetTextColor (hdc, RGBA2Pixel(hdc, 0x40, 0x40, 0x40, 0xff));
    SelectFont (hdc, font46);
   
    for(i=0; i<sizeof(color_level)/sizeof(color_level[0]); i++){
        color_level[i].isCurColor = 0;
        if(i == 0){
            color_level[i].color = RGBA2Pixel(hdc, 0xbf, 0xbf, 0x00, 0xff);
        }else{
            unsigned char offset=0xff;
            offset = ((0xff/(VISIBLE_NUM/2+1))<<i);
            color_level[i].color = RGBA2Pixel(hdc, offset, offset, offset, 0xff);
        }

    }

    for(i=0; i<sizeof(color_level)/sizeof(color_level[0]); i++){
        color_level[i].isCurColor =  (hilight_center>rcDraw->top-i*ITEM_HEIGHT && hilight_center<rcDraw->bottom-i*ITEM_HEIGHT) || \
                  (hilight_center>rcDraw->top+i*ITEM_HEIGHT && hilight_center<rcDraw->bottom+i*ITEM_HEIGHT);
        if(color_level[0].isCurColor){
            SetTextColor(hdc, color_level[0].color);
        }else if(color_level[i].isCurColor){
            SetTextColor(hdc, color_level[i].color);
        }
    }
    DrawText(hdc, name, -1, rcDraw, uFormat);
}


static int LangProc (HWND hDlg, int message, WPARAM wParam, LPARAM lParam)
{
	char *year[] = {"2000", "2001","2002","2003","2004","2005",\
        "2006","2007","2008", "2009", "2010","2011","2012","2013",\
        "2014","2015","2016","2017","2018","2019","2020","2021"};
    char *month[] = {"1", "2", "3", "4","5","6","7","8","9","10","11","12"};
	char *day[] = {"1", "2", "3", "4","5","6","7","8","9","10",\
		           "11", "12", "13", "14", "15", "16", "17", "18", "19", "20",\
		           "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31"};
	SVITEMINFO svii;
    static int i = 0;
	HDC hdc;
    RECT rcMargin = {0, 0, 0, 0};
    RECT rect_wheelview={50, 50, 80, VISIBLE_NUM*ITEM_HEIGHT};
	switch (message) {	
		case MSG_INITDIALOG:
			SetWindowBkColor (hDlg, COLOR_darkgray);
			
			hYearWheel = GetDlgItem (hDlg, IDC_YEARWHEEL);
			SetWindowBkColor(hYearWheel, COLOR_darkgray);
			SendMessage (hYearWheel, SVM_SETMARGINS, 0, (LPARAM)&rcMargin);
			SendMessage (hYearWheel, SVM_SETITEMDRAW, 0, (LPARAM)myDrawItem);
			
			for (i = 0; i < TABLESIZE(year); i++) {
				svii.nItemHeight = ITEM_HEIGHT;
				svii.addData = (DWORD)year[i];
				svii.nItem = i;
				SendMessage (hYearWheel, SVM_ADDITEM, 0, (LPARAM)&svii);
				SendMessage (hYearWheel, SVM_SETITEMADDDATA, i, (DWORD)year[i]);
			}
			SendMessage(hYearWheel, SVM_INIT, ITEM_HEIGHT, TABLESIZE(year));
			SendMessage(hYearWheel, SVM_SETCURSEL, VISIBLE_NUM/2, 0);
			SendMessage(hYearWheel, SVM_SETCURSEL, VISIBLE_NUM/2, 0);

        
        hMonthWheel = GetDlgItem (hDlg, IDC_MONTHWHEEL);
        SetWindowBkColor(hMonthWheel, COLOR_darkgray);

        SendMessage (hMonthWheel, SVM_SETMARGINS, 0, (LPARAM)&rcMargin);
        SendMessage (hMonthWheel, SVM_SETITEMDRAW, 0, (LPARAM)myDrawItem);

        for (i = 0; i < TABLESIZE(month); i++) {
            svii.nItemHeight = ITEM_HEIGHT;
            svii.addData = (DWORD)month[i];
            svii.nItem = i;
            SendMessage (hMonthWheel, SVM_ADDITEM, 0, (LPARAM)&svii);
            SendMessage (hMonthWheel, SVM_SETITEMADDDATA, i, (DWORD)month[i]);
        }
        SendMessage(hMonthWheel, SVM_INIT, ITEM_HEIGHT, TABLESIZE(month));
        SendMessage(hMonthWheel, SVM_SETCURSEL, VISIBLE_NUM/2, 0);
        SendMessage(hMonthWheel, SVM_SETCURSEL, VISIBLE_NUM/2, 0);

        hDayWheel = GetDlgItem (hDlg, IDC_DAYWHEEL);
        SetWindowBkColor(hDayWheel, COLOR_darkgray);

        SendMessage (hDayWheel, SVM_SETMARGINS, 0, (LPARAM)&rcMargin);
        SendMessage (hDayWheel, SVM_SETITEMDRAW, 0, (LPARAM)myDrawItem);

        for (i = 0; i < TABLESIZE(day); i++) {
            svii.nItemHeight = ITEM_HEIGHT;
            svii.addData = (DWORD)day[i];
            svii.nItem = i;
            SendMessage (hDayWheel, SVM_ADDITEM, 0, (LPARAM)&svii);
            SendMessage (hDayWheel, SVM_SETITEMADDDATA, i, (DWORD)day[i]);
        }
        SendMessage(hDayWheel, SVM_INIT, ITEM_HEIGHT, TABLESIZE(day));
        SendMessage(hDayWheel, SVM_SETCURSEL, VISIBLE_NUM/2, 0);
        SendMessage(hDayWheel, SVM_SETCURSEL, VISIBLE_NUM/2, 0);

		CreateButtonEx(&button_next, hDlg);
		return 0;

		case MSG_PAINT:
            hdc = BeginPaint (hDlg);
			SetTextColor(hdc,COLOR_lightwhite);
			SetBkMode(hdc, BM_TRANSPARENT);
			SelectFont (hdc, font46);
			TextOut (hdc, 30, 140, "Please setting date");
            EndPaint (hDlg, hdc);
            break;
			
        case MSG_MOUSEMOVE:
           // printf("---lang button mouse_move----y:%d\n", HISWORD(lParam));
            SendMessage(hYearWheel, MSG_MOUSEMOVE, 0, lParam);
            SendMessage(hMonthWheel, MSG_MOUSEMOVE, 0, lParam);
			SendMessage(hDayWheel, MSG_MOUSEMOVE, 0, lParam);
            return 0;

        case MSG_LBUTTONUP:
            //printf("---lang button up----y:%d\n", HISWORD(lParam));
            SendMessage(hYearWheel, MSG_LBUTTONUP, 0, lParam);
            SendMessage(hMonthWheel, MSG_LBUTTONUP, 0, lParam);
			SendMessage(hDayWheel, MSG_LBUTTONUP, 0, lParam);
            return 0;

		case MSG_COMMAND:
		{
			int id = LOWORD (wParam);
			int code = HIWORD (wParam);
		
			switch (id) {
				//case IDC_SCROLLVIEW:
					//break;
					
				case IDC_NEXT:
					PostMessage(hDlg, MSG_CLOSE, 0, 0);
					break;
			}
			break;
		}

		case MSG_CLOSE:
            EndDialog(hDlg, 0);
			return 0;
	}
	return DefaultDialogProc (hDlg, message, wParam, lParam);
}

static CTRLDATA CtrlLang[] =
{
    {
        "wheelview",
        WS_CHILD | WS_VISIBLE,
         60, 445, 80, VISIBLE_NUM*ITEM_HEIGHT,
        IDC_YEARWHEEL,
        "",
        0
    }, 
  
    {
        "wheelview",
        WS_CHILD | WS_VISIBLE,
         140, 445, 80, VISIBLE_NUM*ITEM_HEIGHT,
        IDC_MONTHWHEEL,
        "",
        0
    },

	{
		"wheelview",
		WS_CHILD | WS_VISIBLE,
		 220, 445, 80, VISIBLE_NUM*ITEM_HEIGHT,
		IDC_DAYWHEEL,
		"",
		0
	}
};

static DLGTEMPLATE DlgLang =
{
    WS_NONE,
    WS_EX_NONE,
    0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
    "",
    0, 0,
    TABLESIZE(CtrlLang), NULL,
    0
};

int date_select()
{
	HWND hLangWnd;
    DlgLang.controls = CtrlLang;
    DialogBoxIndirectParam(&DlgLang, HWND_DESKTOP, LangProc, 0L);
	return 0;
}
