#ifndef _MYBUTTON_H 
#define _MYBUTTON_H 
 
#define MY_CTRL_BUTTON "my_bmp_button" 
 
//button picture coordinate
#define	MBTNBITMAP_0_X			0 
#define	MBTNBITMAP_0_Y			0 
#define	MBTNBITMAP_0_LENGTH		14 
#define	MBTNBITMAP_0_HEIGHT		8 
 
 
#define	MBTNBITMAP_ELSE_X		10 
#define	MBTNBITMAP_ELSE_Y		3 
#define	MBTNBITMAP_ELSE_LENGTH		35 
#define	MBTNBITMAP_ELSE_HEIGHT		32 
 
//button color
#define	MBTNRGB_R			146 
#define	MBTNRGB_G			204 
#define	MBTNRGB_B			245 
 
 
//button text
#define	MBTNTEXTOUT_6_X			9 
#define	MBTNTEXTOUT_6_Y			38 
#define	MBTNTEXTOUT_8_X			3 
#define	MBTNTEXTOUT_8_Y			38 
#define	MBTNTEXTOUT_10_X		0 
#define	MBTNTEXTOUT_10_Y		38 
 
//register control
BOOL RegisterMyButtonControl (void); 
 
void UnregisterMyButtonControl (void); 
 
static int MyButtonControlProc (HWND hwnd, int message, WPARAM wParam, LPARAM lParam); 
 
#endif
