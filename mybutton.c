#include <stdio.h>   
#include <stdlib.h>   
#include <string.h>   
   
#include <minigui/common.h>   
#include <minigui/minigui.h>   
#include <minigui/gdi.h>   
#include <minigui/window.h>   
#include <minigui/control.h>   

#include "mybutton.h"

BOOL RegisterMyButtonControl (void)   
{   
    WNDCLASS MyButtonClass;   
       
    MyButtonClass.spClassName = CTRL_BUTTON;   
    GetWindowClassInfo(&MyButtonClass);   
   
    MyButtonClass.spClassName = MY_CTRL_BUTTON;   
    MyButtonClass.dwStyle     = WS_NONE;   
    MyButtonClass.dwExStyle   = WS_EX_NONE;   
    MyButtonClass.hCursor     = GetSystemCursor (IDC_ARROW);   
    MyButtonClass.iBkColor    = COLOR_lightwhite;   
    MyButtonClass.WinProc     = MyButtonControlProc;   
   
    return RegisterWindowClass (&MyButtonClass);   
}   
   
void UnregisterMyButtonControl (void)   
{   
    UnregisterWindowClass (MY_CTRL_BUTTON);   
}   
   
   
static int MyButtonControlProc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)   
{   
    /*customer content add*/   
    HDC hdc,hPaintdc;   
    RECT rect;   
    int temp,x,y,namelen;   
    temp = GetWindowAdditionalData(hWnd); 
       
    switch(message)   
    {
    case MSG_CREATE:   
        break;   
   
    case MSG_LBUTTONDOWN:   
        SetCapture(hWnd);   
        hdc = GetClientDC(hWnd);
		FillBoxWithBitmap( hdc, MBTNBITMAP_0_X+1, MBTNBITMAP_0_X+1, MBTNBITMAP_0_LENGTH, MBTNBITMAP_0_HEIGHT, &(add_data[temp+1].bitmap) );   
		ReleaseDC(hdc);   
		break;   
   
    case MSG_LBUTTONUP:
        ReleaseCapture();   
        hdc = GetClientDC(hWnd);
        FillBoxWithBitmap( hdc, MBTNBITMAP_0_X, MBTNBITMAP_0_X, MBTNBITMAP_0_LENGTH, MBTNBITMAP_0_HEIGHT, &(add_data[temp].bitmap) );   
        ReleaseDC(hdc);   
        PostMessage(GetParent(hWnd), MSG_COMMAND, GetDlgCtrlID(hWnd),0);   
        break;
   
    case MSG_PAINT:   
        hPaintdc = BeginPaint(hWnd);
        FillBoxWithBitmap( hPaintdc, MBTNBITMAP_0_X, MBTNBITMAP_0_X, MBTNBITMAP_0_LENGTH, MBTNBITMAP_0_HEIGHT, &(add_data[temp].bitmap) );   
        EndPaint(hWnd, hPaintdc);   
        break;   
    }   
	return  DefaultControlProc (hWnd, message, wParam, lParam);   
}
