#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include "common.h"
#include "dev_info.h"
#include "sqlite3.h"

#define IDC_PREV 101
#define IDC_NEXT 102
#define IDC_OK 103

typedef struct code_info {
	int serial;
	int len;
	char *data;
}code_info_t;

typedef struct match_code{
	code_info_t *code;
	int total;
	int cur;
}match_code_t;

static match_code_t code_array;

static HWND hMainWnd = HWND_INVALID;

static buttonex_info_t buttonex_info [] =
{
    {
		"prev",
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		IDC_PREV, 
		90, 519 - Y_OFFSET, 
		296, 70,
		{"common/long_button_bg.png",
		"common/long_button_bg_focus.png",
		"common/long_button_bg_focus.png"},
		0
    },
    {
		"next",
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		IDC_NEXT, 
		90, 629 - Y_OFFSET, 
		296, 70,
		{"common/long_button_bg.png",
		"common/long_button_bg_focus.png",
		"common/long_button_bg_focus.png"},
		0
    },
    {
		"ok",
		WS_VISIBLE | WS_CHILD | WS_TABSTOP | BEXS_BKIMAGE, 
		WS_EX_TRANSPARENT,
		IDC_OK, 
		90, 1094 - Y_OFFSET, 
		296, 70,
		{"common/long_button_bg.png",
		"common/long_button_bg_focus.png",
		"common/long_button_bg_focus.png"},
		0
    }
};

int get_ir_code(HWND hWnd)   
{    
	sqlite3 *db=NULL;    
    sqlite3_stmt* stmt = 0;  
    int ncolumn = 0;
    char *db_name = "ir/IRLibaray.db";
    char sql[128];
    const char *column_name;  
    int vtype , i;  
    int rc;

	dev_info_t *dev_info = (dev_info_t *)GetWindowAdditionalData(hWnd);
	snprintf(sql, sizeof(sql), \
			"select count(*) from '%s' where brand_en='%s'", \
			get_dev_tbl_name(dev_info->type), dev_info->dev_brand);

    if(access(db_name, 0) == -1)  
    {  
        fprintf(stderr, "%s not found\n", db_name);    
        return -1;    
    }  

    rc = sqlite3_open(db_name, &db);  
  
    if( rc != SQLITE_OK)  
    {    
        fprintf(stderr, "%s open failed: %s\n", db_name,sqlite3_errmsg(db));    
        sqlite3_close(db);    
        return -1;    
    }    
  
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);  
    if (rc != SQLITE_OK)  
    {  
        fprintf(stderr, "%s %s: %s\n", db_name,sql, sqlite3_errmsg(db));    
        sqlite3_close(db);    
        return -1;    
    }  
	sqlite3_step(stmt);
	code_array.total = sqlite3_column_int(stmt,0);
	sqlite3_finalize(stmt);

	code_array.code = (code_info_t *)malloc(code_array.total*sizeof(code_info_t));
    if (!code_array.code) {
		sqlite3_finalize(stmt);
		sqlite3_close(db);
		return -1;
    }
	
	snprintf(sql, sizeof(sql), \
			"select serial,code from '%s' where brand_en='%s'", \
			get_dev_tbl_name(dev_info->type), dev_info->dev_brand);

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);  
    if (rc != SQLITE_OK)  
    {  
        fprintf(stderr, "%s %s: %s\n", db_name,sql, sqlite3_errmsg(db));    
        sqlite3_close(db);    
        return -1;    
    }

    ncolumn = sqlite3_column_count(stmt);  

  	i = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW)  
    {     
		code_array.code[i].serial = sqlite3_column_int(stmt, 0);
		code_array.code[i].len = sqlite3_column_bytes(stmt, 1);
		code_array.code[i].data = (char *)malloc(code_array.code[i].len);
		if(code_array.code[i].data == NULL) {
			sqlite3_finalize(stmt);
			sqlite3_close(db);
			return -1;
		}
		memcpy(code_array.code[i].data, sqlite3_column_blob(stmt,1), code_array.code[i].len);
		i++;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);   
	return 0;
}  

void free_ir_code()
{
	int i;
	
	for(i = 0; i < code_array.total; i++) {
		free(code_array.code[i].data);
	}
	free(code_array.code);
}

void remote_control(dev_info_t *dev_info)
{
	switch(dev_info->type)
	{
		case  DEV_TYPE_TV:
			tv(HWND_DESKTOP, dev_info);
			break;

		case  DEV_TYPE_IPTV:
			iptv(HWND_DESKTOP, dev_info);
			break;
			
		case  DEV_TYPE_TVBOX:
			tvbox(HWND_DESKTOP, dev_info);
			break;
		
		case  DEV_TYPE_DVD:
			dvd(HWND_DESKTOP, dev_info);
			break;
			
		case  DEV_TYPE_FAN:
			fan(HWND_DESKTOP, dev_info);
			break;
		
		case  DEV_TYPE_AIR_CONDITION:
			air_condition(HWND_DESKTOP, dev_info);
			break;
			
		case  DEV_TYPE_AIR_CLEANER:
			air_cleaner(HWND_DESKTOP, dev_info);
			break;
			
		case  DEV_TYPE_PROJECTOR:
			projector(HWND_DESKTOP, dev_info);
			break;
			
		case  DEV_TYPE_WATER_HEATER:
			water_heater(HWND_DESKTOP, dev_info);
			break;
	}
}

void match_code_op(HWND hWnd, int btn_id)
{
	dev_info_t *dev_info = (dev_info_t *)GetWindowAdditionalData(hWnd);
	HWND hwnd_tmp = HWND_INVALID;
	HWND hosting = hWnd;

	if(btn_id == IDC_OK) {
		while((hwnd_tmp = GetHosting(hosting)) != HWND_DESKTOP) {
			hosting = hwnd_tmp;
		}
		//SendNotifyMessage(hosting, MSG_CLOSE, 0, 0);
		dev_info->code_len = code_array.code[code_array.cur].len;
		memcpy(dev_info->ir_code, code_array.code[code_array.cur].data, code_array.code[code_array.cur].len);
		remote_control(dev_info);
		SendNotifyMessage(hosting, MSG_CLOSE, 0, 0);
		return;
	} else if(btn_id == IDC_PREV || btn_id == IDC_NEXT) {

		if(btn_id == IDC_PREV) {
			code_array.cur = (--code_array.cur + code_array.total)%code_array.total;
		} else if(btn_id == IDC_NEXT) {
			code_array.cur = (++code_array.cur + code_array.total)%code_array.total;
		}
		if(dev_info->type == DEV_TYPE_AIR_CONDITION) {
			send_ir_code_arc(code_array.code[code_array.cur].data,code_array.code[code_array.cur].len,NULL);
		} else {
			send_ir_code_general(code_array.code[code_array.cur].data,code_array.code[code_array.cur].len, dev_info->match_code_key);
		}
		printf("%d:%d\n", code_array.cur + 1, code_array.total);
	}
}

static int match_code_proc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{	
	switch (message)
	{
		case MSG_ERASEBKGND:
		{
			erase_bkgnd_with_pic(hWnd, (HDC)wParam, (const RECT*) lParam, Y_OFFSET);
			return 0;
		}

		case MSG_CREATE:
		{	
			register_pic("res/match_code/");
			init_button_ex(hWnd, buttonex_info, TABLESIZE(buttonex_info));
			get_ir_code(hWnd);
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
			int id = LOWORD(wParam);
			int nCode = HIWORD(wParam);
			
			switch (id)
			{
				case IDC_BACK:
					PostMessage(hWnd, MSG_CLOSE, 0, 0);
					break;
					
				case IDC_PREV:					
				case IDC_NEXT:
				case IDC_OK:
					match_code_op(hWnd, id);
					break;
			}
			break;
		}
		
        case MSG_DESTROY:
        	free_ir_code();
            DestroyAllControls (hWnd);
            hMainWnd = HWND_INVALID;
        	return 0;

        case MSG_CLOSE:
        	un_register_pic("res/match_code/");
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
    pCreateInfo->MainWindowProc = match_code_proc;
    pCreateInfo->lx = 0; 
    pCreateInfo->ty = Y_OFFSET;
    pCreateInfo->rx = 480;
    pCreateInfo->by = 1280;
    pCreateInfo->iBkColor = GetWindowElementColor (WE_MAINC_THREED_BODY);
    pCreateInfo->dwAddData = 0;
    pCreateInfo->hHosting = HWND_DESKTOP;
}

void match_code(HWND hosting, dev_info_t *dev_info)
{
    MAINWINCREATE CreateInfo;

    if (hMainWnd != HWND_INVALID) {
        ShowWindow (hMainWnd, SW_SHOWNORMAL);
        return;
    }

    InitCreateInfo (&CreateInfo);
    CreateInfo.hHosting = hosting;
    CreateInfo.dwAddData = (DWORD)dev_info;

    hMainWnd = CreateMainWindow (&CreateInfo);
}
