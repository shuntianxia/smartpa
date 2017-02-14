#ifndef _COMMON_H
#define _COMMON_H

#include "button_ex.h"
#include "font.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#define CFGFILE  "desktop.rc"
#define IDC_BACK 100
#define Y_OFFSET 300

int init_button_ex(HWND hWnd, buttonex_info_t *button_info, int size);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _COMMON_H */

