#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include "ButtonEx.h"
#include "button_ex.h"

void CreateButtonEx	(buttonex_info_t *btn_info, HWND hParentWnd)
{
	static BEXDATA bexData;
	static BEXCDATA bexCData;
	CreateWindowEx	(CTRL_BUTTONEX, btn_info->spCaption, btn_info->dwStyle, 
		btn_info->dwExStyle,
		btn_info->id, 
		btn_info->x,
		btn_info->y, 
		btn_info->w,
		btn_info->h,
		hParentWnd, 0L);
	
	bexData.bClickEffect = TRUE;
	bexData.nClickEffect = 4;
	bexData.pbmpIcon = GetBitmapFromRes(Str2Key(btn_info->pbmpIcon));
	//bexData.bIconTrans = TRUE;
	bexData.nIconLeft = 0;
	bexData.pixelTextNormal = PIXEL_lightwhite;
	
	bexData.pbmpButton[BEXUI_NORMAL] = GetBitmapFromRes(Str2Key(btn_info->pbmpButton[BEXUI_NORMAL]));
	bexData.pbmpButton[BEXUI_FOCUS] = GetBitmapFromRes(Str2Key(btn_info->pbmpButton[BEXUI_FOCUS]));
	bexData.pbmpButton[BEXUI_CLICK] = GetBitmapFromRes(Str2Key(btn_info->pbmpButton[BEXUI_CLICK]));
	
	SendDlgItemMessage (hParentWnd, btn_info->id, BEXM_SETCOMDATA, 
		(WPARAM)&bexData, (LPARAM)BEX_COM_ALL);
	if(bexData.pbmpIcon != NULL) {
		SendDlgItemMessage (hParentWnd, btn_info->id, BEXM_SETICON, 
			(WPARAM)&bexData, (LPARAM)BEX_ICON_ALL);
	}
}
