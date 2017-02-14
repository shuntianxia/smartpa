#ifndef _BUTTON_EX_H
#define _BUTTON_EX_H

#include "ButtonEx.h"

typedef struct buttonex_info {
	char* spCaption;
	DWORD dwStyle;
	DWORD dwExStyle;
	int id;
	int x;
	int y;
	int w;
	int h;
	char *pbmpButton[3];
	char *pbmpIcon;
}buttonex_info_t;

void CreateButtonEx	(buttonex_info_t *btn_info, HWND hParentWnd);
#endif  //  end _BUTTON_EX_H

