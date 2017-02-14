/*
** $Id$
**
** skindemo.c: Sample program for MiniGUI Programming Guide
**      Usage of SKIN.
**
** Copyright (C) 2004 ~ 2008 Feynman Software.
**
** License: GPL
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include "common.h"

int init_button_ex(HWND hWnd, buttonex_info_t *buttonex_info, int size)
{
	int i;
	for ( i = 0; i < size; i++ )
	{
		CreateButtonEx(&buttonex_info[i], hWnd);
	}
}

PBITMAP get_bg_pic()
{
	char *pic_str ="desktop/bg.png";

	return GetBitmapFromRes(Str2Key(pic_str));
}

void erase_bkgnd_with_pic(HWND hWnd, HDC hdc, const RECT* clip, int yoffset)
{
	BOOL fGetDC = FALSE;
	RECT rcTemp;
	PBITMAP pic;

	pic = get_bg_pic();
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
	
	FillBoxWithBitmapPart(hdc,0,0,RECTW(rcTemp), RECTH(rcTemp),0,0,pic,0,yoffset);
	
	if (fGetDC)
		ReleaseDC (hdc);
	return;
}

void erase_bkgnd_with_color(HWND hWnd, HDC hdc, const RECT* clip)
{
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
		
	SetBrushColor(hdc,RGB2Pixel(hdc,64,65,69));
	FillBox(hdc, 0, 0,RECTW(rcTemp), RECTH(rcTemp));
	
	if (fGetDC)
		ReleaseDC (hdc);
	return;
}

