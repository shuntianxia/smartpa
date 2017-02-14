/* 
** $Id: desktop.c 600 2008-08-14 07:22:04Z weiym $
**
** The desktop of MDE
**
** Copyright (c) 2001, Wei Yongming (ymwei@minigui.org)
** Copyright (C) 2003 ~ 2007 Feynman Software.
**
** Create date: 2008/01/28
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include "common.h"
#include "desktop.h"

#define MGDT_ST_ICONMENU        ""
#define MGDT_ST_OPEN            "Open"
#define MGDT_ST_ABOUT           "About"
#define MGDT_ST_ABOUTM          "About MiniGUI..."
#define MGDT_ST_REFRESH         "Refresh"

static void free_dsp_app (void)
{
    int i;
    DSPITEM* item;

    item = icon_info.app_items;
    for (i = 0; i < icon_info.nr_apps; i++, item++) {
        if (item->bmp.bmBits) {
            UnloadBitmap (&item->bmp);
            item->bmp.bmBits = NULL;
        }
    }

    free (icon_info.app_items);
    icon_info.app_items = NULL;
}

static BOOL get_dsp_app (void)
{
    int i;
    DSPITEM* item;
    char section [10];
	int distance_x = START_X;
	int distance_y = START_Y;
    SIZE size;

    if (GetIntValueFromEtcFile (APP_INFO_FILE, "desktop", "app_nr", &icon_info.nr_apps) != ETC_OK)
        return FALSE;
    
    if (icon_info.nr_apps <= 0)
        return FALSE;
    
    if (GetValueFromEtcFile (APP_INFO_FILE, "desktop", "bg_pic", icon_info.bg_path, PATH_MAX) != ETC_OK)
        return FALSE;
    
    if ((icon_info.app_items = (DSPITEM*)calloc (icon_info.nr_apps, sizeof (DSPITEM))) == NULL) {
        return FALSE;
    }
    item = icon_info.app_items;
    for (i = 0; i < icon_info.nr_apps; i++, item++) {
        sprintf (section, "dsp-app%d", i);
        if (GetValueFromEtcFile (APP_INFO_FILE, section, "path", item->path, PATH_MAX) != ETC_OK)
            goto error;

        if (GetValueFromEtcFile (APP_INFO_FILE, section, "name", item->name, NAME_MAX) != ETC_OK)
            goto error;

        if (GetValueFromEtcFile (APP_INFO_FILE, section, "layer", item->layer, LEN_LAYER_NAME) != ETC_OK)
            goto error;

        if (GetValueFromEtcFile (APP_INFO_FILE, section, "pictrue", item->bmp_path, PATH_MAX + NAME_MAX) != ETC_OK)
            goto error;

        if (LoadBitmap (HDC_SCREEN, &item->bmp, item->bmp_path) != ERR_BMP_OK) {
            fprintf (stderr, "desktop load resource:%s error. \n", item->bmp_path);
            goto error;
        }
        
        item->cdpath = TRUE;

		item->hot_spot_rc.left   = distance_x;
		item->hot_spot_rc.right  = item->hot_spot_rc.left + DEF_WIDTH;
		item->hot_spot_rc.top    = distance_y;
		item->hot_spot_rc.bottom = item->hot_spot_rc.top + DEF_HEIGHT;

		SelectFont(HDC_SCREEN, font22);
        GetTextExtent(HDC_SCREEN, item->name, -1, &size);

        item->text_rc.top     = item->hot_spot_rc.bottom;
        item->text_rc.left    = (item->hot_spot_rc.right + item->hot_spot_rc.left - size.cx)/2;
        item->text_rc.bottom  = item->text_rc.top + size.cy;
        item->text_rc.right   = item->text_rc.left + size.cx;
#if 0		
        distance_y += SPACE + RECTH(item->hot_spot_rc);
        if(distance_y + DEF_HEIGHT >= g_rcDesktop.bottom)
        {
            distance_y = START_Y;
            distance_x += SPACE + DEF_WIDTH;
        }
#else
		distance_x += SPACE + RECTW(item->hot_spot_rc);
		if(distance_x + DEF_WIDTH >= g_rcDesktop.right)
		{
			distance_x = START_X;
			distance_y += SPACE + DEF_HEIGHT;
		}
#endif
    }
    return TRUE;
error:
    free_dsp_app ();
    return FALSE;
}

static HMENU create_icon_menu (void)
{
    HMENU hmnu;
    MENUITEMINFO mii;

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING;
    mii.id          = 0;
    mii.typedata    = (DWORD)MGDT_ST_ICONMENU;
    hmnu = CreatePopupMenu (&mii);
    
    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = ID_OP;
    mii.typedata    = (DWORD)MGDT_ST_OPEN;
    InsertMenuItem(hmnu, 2, TRUE, &mii);

    memset (&mii, 0, sizeof(MENUITEMINFO));
    mii.type        = MFT_STRING ;
    mii.state       = 0;
    mii.id          = ID_AB;
    mii.typedata    = (DWORD)MGDT_ST_ABOUT;
    InsertMenuItem(hmnu, 3, TRUE, &mii);

    return hmnu;
}

static void* this_init(void)
{
    Context *con = malloc(sizeof(Context));

	register_pic("res/desktop/");
    get_dsp_app ();
    icon_info.focus = 0xFFFF;

    if(0 != strcmp(icon_info.bg_path, ""))
    {
        con->bg = (PBITMAP)malloc(sizeof(BITMAP));
        LoadBitmapFromFile(HDC_SCREEN, con->bg, icon_info.bg_path);
    }
    else
        con->bg = NULL; 

    con->icon_menu = create_icon_menu();   

    return (void *)con;
}

static void this_deinit(void* context)
{
	un_register_pic("res/desktop/");
    if(((Context *)context)->bg) 
        UnloadBitmap (((Context *)context)->bg);

    free_dsp_app ();

	if (context != NULL) free(context);

    return;
}

static void this_paint_desktop(void* context, HDC dc_desktop, const RECT* inv_rc)
{
    PBITMAP bg_bmp = NULL;
	int i = 0;
    DSPITEM* item;

    if(((Context *)context)->bg)
        bg_bmp = ((Context *)context)->bg;

    SelectClipRect (dc_desktop, inv_rc);
    if (bg_bmp) {
        FillBoxWithBitmap (dc_desktop, 0, 0,
                g_rcDesktop.right, g_rcDesktop.bottom, bg_bmp);
    }else {
        SetBrushColor (dc_desktop, PIXEL_blue);
        FillBox(dc_desktop, g_rcDesktop.left, g_rcDesktop.top,
                RECTW(g_rcDesktop), RECTH(g_rcDesktop));
    }
    
    item = icon_info.app_items;
    for(i = 0; i < icon_info.nr_apps; i++, item++)
	{
        if(i == icon_info.focus)
        {
            SetBrushColor (dc_desktop, PIXEL_darkblue);
            FillBox(dc_desktop, item->hot_spot_rc.left, 
                    item->hot_spot_rc.top,
                    RECTW(item->hot_spot_rc), 
                    RECTH(item->hot_spot_rc));
        }
        FillBoxWithBitmap (dc_desktop,
                item->hot_spot_rc.left,
                item->hot_spot_rc.top,
                RECTW(item->hot_spot_rc),
                RECTH(item->hot_spot_rc), 
                &item->bmp);
        SetBkMode(dc_desktop, BM_TRANSPARENT);
        SetTextColor (dc_desktop, PIXEL_lightwhite);
        SelectFont(dc_desktop, font22);
        TextOut(dc_desktop, 
                item->text_rc.left, item->text_rc.top, item->name);
	}
}

static void this_keyboard_handler(void* context, int message, 
		                               WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case MSG_DT_KEYDOWN:
            break;
        case MSG_DT_KEYUP:
            break;
    }
}

static void this_customize_desktop_menu (void* context, HMENU hmnu, int start_pos)
{

}

static void this_mouse_handler(void* context, int message, 
		                            WPARAM wParam, LPARAM lParam)
{
    int x, y;
    int i = 0;
    DSPITEM* item;
    static int old_x = 0, old_y = 0;
    static int down_x, down_y;

    x = LOSWORD (lParam);
    y = HISWORD (lParam);
    switch(message)
    {
        case MSG_DT_LBUTTONDOWN:
            {
                item = icon_info.app_items;
                for(i = 0; i < icon_info.nr_apps; i++, item++) 
                {
                    if (PtInRect(&item->hot_spot_rc, x, y)) 
                    {
                        icon_info.focus = i;
                        old_x = x;
                        old_y = y;
                        down_x = x;
                        down_y = y;
                        SendMessage(HWND_DESKTOP, MSG_ERASEDESKTOP, 
                                0, (LPARAM)&item->hot_spot_rc);
                        break;
                    }
                }
                break;
            }

        case MSG_DT_LBUTTONUP:
            {
                icon_info.focus = 0xFFFF;
                SendMessage(HWND_DESKTOP, MSG_ERASEDESKTOP, 0, 0);
                if(abs(down_x - x) < 5 && abs(down_y - y) < 5) {
					item = icon_info.app_items;
					for(i = 0; i < icon_info.nr_apps; i++, item++) 
					{
						if (PtInRect(&item->hot_spot_rc, x, y)) 
						{
							if(strcmp(item->name, "setting") == 0) {
								setting(HWND_DESKTOP);
							}
							break;
						}
					}
                }
                break;
            }
#if 0
        case MSG_DT_MOUSEMOVE:
            {
                RECT rc;
                if (icon_info.focus == 0xFFFF) break;

                item = icon_info.app_items;
                for(i = 0; i < icon_info.nr_apps; i++, item++) 
                {
                    if (i == icon_info.focus ) 
                    {
                        GetBoundRect(&rc, &item->text_rc, &item->hot_spot_rc);
                        
                        item->hot_spot_rc.left      += x - old_x;
                        item->hot_spot_rc.right     += x - old_x;
                        item->hot_spot_rc.top       += y - old_y;
                        item->hot_spot_rc.bottom    += y - old_y;

                        item->text_rc.left      += x - old_x;
                        item->text_rc.right     += x - old_x;
                        item->text_rc.top       += y - old_y;
                        item->text_rc.bottom    += y - old_y;
                        
                        old_x = x;
                        old_y = y;
                        GetBoundRect(&rc, &rc, &item->hot_spot_rc);
                        GetBoundRect(&rc, &rc, &item->text_rc);
                        SendMessage(HWND_DESKTOP, MSG_ERASEDESKTOP, 0, (LPARAM)&rc);
                    }
                }
                break;
            }
#endif
    }
}

static void this_desktop_menucmd_handler (void* context, int id)
{
    return;
}


DESKTOPOPS mg_dsk_ops =
{
    this_init,
    this_deinit,
    this_paint_desktop,
    this_keyboard_handler,
    this_mouse_handler,
    this_customize_desktop_menu,
    this_desktop_menucmd_handler,
};

