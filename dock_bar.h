#ifndef _DOCK_BAR_H
#define _DOCK_BAR_H
 

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

void init_dock_bar(HWND hdockbar);

void dock_bar_cmd_handler(HWND hdockbar, int code);


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _DOCK_BAR_H */

