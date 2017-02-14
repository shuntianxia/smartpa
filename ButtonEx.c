//********************************************************************
// Copyright (C) 2009 ~ 2010 mingming-killer
// Licence: GPL
//
// 项目: ControlEx
// 文件: ButtonEx.c
// 版本: V 1.3.3
//
// 说明: 自定义按钮. 
// 		    with minigui 2.0.4 API.
//
// 作者: minging-killer
// 		    Email: mingming_killer@126.com
//
// 日期: 2010.2.3
// 
//*********************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>
#include <minigui/fixedmath.h>

#include "ButtonEx.h"




//*********************************************************************
//**********************   私有数据结构 **************************
//*********************************************************************

// 长无符号长整形RGB 颜色
typedef struct _fixed_rgb_st
{
    fixed r;
    fixed g;
    fixed b;
} FIXED_RGB;


// 宏定义
#define INCRGB(drgb, srgb) \
          (drgb.r) += (srgb.r);\
          (drgb.g) += (srgb.g);\
          (drgb.b) += (srgb.b)
          

#define FixedRGB2Pixel(hdc, r, g, b) \
          RGB2Pixel((hdc), (r)>>16, (g)>>16, (b)>>16)

//*********************************************************************



//*********************************************************************
//************************   私有函数 ******************************
//*********************************************************************

// 功能辅助函数
static int BEX_InitCtrl (HWND hCtrl);
static void BEX_Cleanup (HWND hCtrl, PBEXDATA pData);
static void BEX_DrawCtrl (HWND hCtrl, HDC hdc, PBEXDATA pData, DWORD dwStyle);
static void BEX_ImageStyleDraw (HWND hCtrl, HDC hdc, PBEXDATA pData);
static void BEX_BkImageStyleDraw (HWND hCtrl, HDC hdc, PBEXDATA pData);
static void BEX_DrawStyleDraw (HWND hCtrl, HDC hdc, PBEXDATA pData);
static FIXED_RGB BEX_CalcRenderRGB (RGB rgbBase, RGB rgbShade, int dist);
static inline int BEX_SetCDataToDefault (PBEXCDATA pCData);

// 消息处理函数
static int ButtonExProc (HWND hCtrl, int message, WPARAM wParam, LPARAM lParam);

//*********************************************************************




//*********************************************************************
//***********************   文件变量定义 *************************
//*********************************************************************

// ButtonEx 控件类数据, 
// 这一数据在所有的由CreateWindow() 创建的ButtonEx 实例中均有效, 
// 所以叫类数据
static PBEXCDATA pCData;

//*********************************************************************




//****************************************************
// 原型	: BOOL RegisterButtonExControl (void)
// 参数	: void
// 返回值: TRUE:  注册成功
//			  FALSE: 注册失败
// 说明	: 注册ButtonEx 控件, 
//			  在使用ButtonEx 控件前必须调用次函数进行注册
//****************************************************
BOOL RegisterButtonExControl (void)
{
	WNDCLASS BEXClass;

	// 为Button 类数据申请内存空间
	pCData = (PBEXCDATA)calloc (1, sizeof(BEXCDATA));
	if ( NULL == pCData )
		return FALSE;
	
	// 填写控件类信息
	BEXClass.spClassName 	= CTRL_BUTTONEX;
	BEXClass.dwStyle		= WS_NONE;
	BEXClass.dwExStyle	= WS_EX_NONE;
	BEXClass.iBkColor		= PIXEL_lightwhite;
	//BEXClass.hCursor     	= GetSystemCursor (IDC_HAND_POINT);
	BEXClass.hCursor     	= GetSystemCursor(0);
	BEXClass.WinProc 		= ButtonExProc;
	BEXClass.dwAddData 	= (DWORD)pCData;

	
	// 初始化类数据
	BEX_SetCDataToDefault (pCData);
	
	
	// 注册ButtonEx 类
	return RegisterWindowClass (&BEXClass);
	
} // end RegisterDigitEditControl() 


//****************************************************
// 原型	: BOOL UnregisterDigitEditControl (void)
// 参数	: void
// 返回值: TRUE:  卸载成功
//			  FALSE: 卸载失败
// 说明	: 卸载ButtonEx 控件
//*****************************************************
BOOL UnregisterButtonExControl (void)
{
	// 释放ButtonEx 类数据
	if ( NULL != pCData )
	{
		free (pCData);
		pCData = NULL;
	}

	// 卸载ButtonEx 类
	return UnregisterWindowClass (CTRL_BUTTONEX);
}



//****************************************************
// 原型	: BOOL UnregisterDigitEditControl (void)
// 参数	: void
// 返回值: TRUE:  卸载成功
//			  FALSE: 卸载失败
// 说明	: 卸载ButtonEx 控件
//*****************************************************
int BexSetCData (const PBEXCDATA pSetCData, DWORD dwMask)
{
	if ( dwMask & BEX_CDATA_DEFAULT )
		return BEX_SetCDataToDefault (pCData);
	
	if ( NULL == pSetCData || NULL == pCData ) 
		return BEX_ERR;
	
	
	// 设置渲染颜色
	if ( dwMask & BEX_CDATA_RENDER )
	{
		SETRGB (pCData->rgbRenderNormalBase, 
			pSetCData->rgbRenderNormalBase.r, 
			pSetCData->rgbRenderNormalBase.g, 
			pSetCData->rgbRenderNormalBase.b);
		
		SETRGB (pCData->rgbRenderNormalShade, 
			pSetCData->rgbRenderNormalShade.r, 
			pSetCData->rgbRenderNormalShade.g, 
			pSetCData->rgbRenderNormalShade.b);

		pCData->pixelFocus = pSetCData->pixelFocus;
		pCData->pixelDisableBk = pSetCData->pixelDisableBk;
		pCData->pixelBorder = pSetCData->pixelBorder;
	}
	
	return BEX_OK;
	
}  //  end BexSetCData()


//****************************************************
// 原型	: BOOL UnregisterDigitEditControl (void)
// 参数	: void
// 返回值: TRUE:  卸载成功
//			  FALSE: 卸载失败
// 说明	: 卸载ButtonEx 控件
//*****************************************************
int BexGetCData (PBEXCDATA pGetCData, DWORD dwMask)
{
	if ( dwMask & BEX_CDATA_DEFAULT )
		return BEX_SetCDataToDefault (pGetCData);
	
	if ( NULL == pGetCData || NULL == pCData ) 
		return BEX_ERR;
	
	
	// 获取渲染颜色
	if ( dwMask & BEX_CDATA_RENDER )
	{
		SETRGB (pGetCData->rgbRenderNormalBase, 
			pCData->rgbRenderNormalBase.r, 
			pCData->rgbRenderNormalBase.g, 
			pCData->rgbRenderNormalBase.b);
		
		SETRGB (pGetCData->rgbRenderNormalShade, 
			pCData->rgbRenderNormalShade.r, 
			pCData->rgbRenderNormalShade.g, 
			pCData->rgbRenderNormalShade.b);

		pGetCData->pixelFocus = pCData->pixelFocus;
		pGetCData->pixelDisableBk = pCData->pixelDisableBk;
		pGetCData->pixelBorder = pCData->pixelBorder;
	}
	
	return BEX_OK;
	
}  //  end BexGetCData()




// 设置指定类数据为默认值
static inline int BEX_SetCDataToDefault (PBEXCDATA pCData)
{
	if ( NULL == pCData )
		return BEX_ERR;

	SETRGB (pCData->rgbRenderNormalBase, 
		BEX_DEFCDATA_BASECOLOR_R, 
		BEX_DEFCDATA_BASECOLOR_G, 
		BEX_DEFCDATA_BASECOLOR_B);
	
	SETRGB (pCData->rgbRenderNormalShade, 
		BEX_DEFCDATA_SHADECOLOR_R, 
		BEX_DEFCDATA_SHADECOLOR_G, 
		BEX_DEFCDATA_SHADECOLOR_B);
	
	pCData->pixelFocus = BEX_DEFCDATA_PIXELFOCUS;
	pCData->pixelBorder = BEX_DEFCDATA_PIXELBORDER;
	pCData->pixelDisableBk = BEX_DEFCDATA_PIXELDISABLE;

	return BEX_OK;
	
}


// 初始化ButtonEx 实例
static int BEX_InitCtrl (HWND hCtrl)
{
	// 为控件实例数据申请内存空间, 并初始化为0
	PBEXDATA pData = (PBEXDATA)calloc (1, sizeof(BEXDATA));
	if ( NULL == pData ) 
		return BEX_ERR;

	
	// 初始化控件成员变量
	// 通用数据
	pData->nState = BEXUI_NORMAL;

	pData->pbmpButton[0] = NULL;
	pData->pbmpButton[1] = NULL;
	pData->pbmpButton[2] = NULL;
	pData->pbmpButton[3] = NULL;

	pData->bTrans = FALSE;
	pData->pointTrans.x = 0;
	pData->pointTrans.y = 0;

	pData->bClickEffect = TRUE;
	pData->nClickEffect = 2;
	
	pData->pixelTextNormal = PIXEL_black;
	pData->pixelTextDisable = RGB2Pixel (HDC_SCREEN, 128, 128, 128);


	// ICON 数据
	pData->pbmpIcon = NULL;
	
	pData->bIconTrans = FALSE;
	pData->pointIconTrans.x = 0;
	pData->pointIconTrans.y = 0;

	pData->nIconLeft = 5;
	pData->nIconDx = 4;

	pData->bIconDisableEffect = FALSE;
	pData->hIconDC = HDC_INVALID;
	pData->nIconDisableAlpha = 128;
	
	
	SetWindowAdditionalData2 (hCtrl, (DWORD)pData);
	return BEX_OK;
	
}  // end BEX_InitCtrl()


// 释放ButtonEx 实例占用的资源
static void BEX_Cleanup (HWND hCtrl, PBEXDATA pData)
{
	if ( NULL != pData ) 
	{	
		if ( HDC_INVALID != pData->hIconDC )
			DeleteMemDC (pData->hIconDC);
	
		free (pData);
		pData = NULL;
	}

	SetWindowAdditionalData2 (hCtrl, 0L);
}


// 计算Render 风格的渐变RGB 颜色
static FIXED_RGB BEX_CalcRenderRGB (RGB rgbBase, RGB rgbShade, int dist)
{
	fixed d;
    	FIXED_RGB drgb;
    	
    	if ( 0 == dist )
	{
       	drgb.r = rgbBase.r << 16;
        	drgb.g = rgbBase.g << 16;
        	drgb.b = rgbBase.b << 16;

        	return drgb;
    	}

    	d = abs(dist) << 16;

    	drgb.r = fixdiv((rgbShade.r - rgbBase.r) << 16, d);
    	drgb.g = fixdiv((rgbShade.g - rgbBase.g) << 16, d);
    	drgb.b = fixdiv((rgbShade.b - rgbBase.b) << 16, d);

    	return drgb;

}  // end BEX_CalcRGB()


#define BEX_AA 45

// 透明颜色边缘抗锯齿(测试用, 算法目前还是错的-_-||)
static void BEX_SmoothTransEdge (PBITMAP pbmp, gal_pixel transColor, HDC hdc)
{
	if ( NULL == pbmp )
		return;

	unsigned int i;
	RGB rgbTrans;
	RGB rgbMax;
	RGB rgbMin;
	gal_pixel pixelMax;
	gal_pixel pixelMin;

	Pixel2RGB (hdc, transColor, &rgbTrans.r, &rgbTrans.g, &rgbTrans.b);
	
	printf ("pixel:%d, r:%d, g:%d, b:%d\n", transColor, rgbTrans.r, rgbTrans.g, rgbTrans.b);

	RGB tmp;
	gal_pixel gtmp;
	
	gtmp = GetPixelInBitmap (pbmp, 7, 22);
	Pixel2RGB (hdc, gtmp, &tmp.r, &tmp.g, &tmp.b);
	printf ("test: pixel:%d, r:%d, g:%d, b:%d\n", gtmp, tmp.r, tmp.g, tmp.b);


	if ( rgbTrans.r <= BEX_AA )
		rgbMin.r = 0;
	else
		rgbMin.r = rgbTrans.r - BEX_AA;

	if ( rgbTrans.g <= BEX_AA )
		rgbMin.g = 0;
	else
		rgbMin.g = rgbTrans.g - BEX_AA;

	if ( rgbTrans.b <= BEX_AA )
		rgbMin.b = 0;
	else
		rgbMin.b = rgbTrans.b - BEX_AA;
	

	if ( rgbTrans.r >= 255-BEX_AA )
		rgbMax.r = 255;
	else
		rgbMax.r = rgbTrans.r + BEX_AA;

	if ( rgbTrans.g >= 255-BEX_AA )
		rgbMax.g = 255;
	else
		rgbMax.g = rgbTrans.g + BEX_AA;

	if ( rgbTrans.b >= 255-BEX_AA )
		rgbMax.b = 255;
	else
		rgbMax.b = rgbTrans.b + BEX_AA;

	
	pixelMin = RGB2Pixel (hdc, rgbMin.r, rgbMin.g, rgbMin.b);
	pixelMax = RGB2Pixel (hdc, rgbMax.r, rgbMax.g, rgbMax.b);
	
	printf ("min:%d, r:%d, g:%d, b:%d\n", pixelMin, rgbMin.r, rgbMin.g, rgbMin.b);
	printf ("max:%d, r:%d, g:%d, b:%d\n", pixelMax, rgbMax.r, rgbMax.g, rgbMax.b);

	for ( i = pixelMin; i <= pixelMax; i++ )
	{
		ReplaceBitmapColor (hdc, pbmp, i, transColor);
	}

	gtmp = GetPixelInBitmap (pbmp, 7, 22);
	Pixel2RGB (hdc, gtmp, &tmp.r, &tmp.g, &tmp.b);
	printf ("after: pixel:%d, r:%d, g:%d, b:%d\n", gtmp, tmp.r, tmp.g, tmp.b);
	
}  // end BEX_SmoothTransEdge()




// 绘制控件显示
static void BEX_DrawCtrl (HWND hCtrl, HDC hdc, PBEXDATA pData, DWORD dwStyle)
{
	// 根据不同的控件风格, 进行不同的绘制处理
	switch (dwStyle & BEXS_TYPEMASK)
	{
		// 图片风格(默认风格)
		case BEXS_IMAGE:
		default:
		{
			if ( NULL == pData || HDC_INVALID == hdc )
				break;
			
			BEX_ImageStyleDraw (hCtrl, hdc, pData);
			break;
		}

		// 背景图片风格
		case BEXS_BKIMAGE:
		{
			if ( NULL == pData || HDC_INVALID == hdc )
				break;

			BEX_BkImageStyleDraw (hCtrl, hdc, pData);
			break;
		}
		
		// 代码绘制风格
		case BEXS_DRAW:
		{
			if ( NULL == pCData || NULL == pData || HDC_INVALID == hdc )
				break;

			BEX_DrawStyleDraw (hCtrl, hdc, pData);
			break;
		}
		
	}// end switch()
	
}// end BEX_DrawCtrl()


// IMAGE 风格绘制函数
static void BEX_ImageStyleDraw (HWND hCtrl, HDC hdc, PBEXDATA pData)
{
	PBITMAP pbmp = pData->pbmpButton[pData->nState];

	RECT rcClient;	
	GetClientRect (hCtrl, &rcClient);

	
	// 用图片来完全绘制控件
	if ( NULL != pbmp )
	{
		// 透明背景图片
		if ( TRUE == pData->bTrans )
		{
			pbmp->bmType |= BMP_TYPE_COLORKEY;
			pbmp->bmColorKey = GetPixelInBitmap(pbmp, pData->pointTrans.x, pData->pointTrans.y);
		}
		else
		{
			pbmp->bmType &= ~BMP_TYPE_COLORKEY;
		}
			
		FillBoxWithBitmap (hdc, rcClient.left, rcClient.top, RECTW(rcClient), RECTH(rcClient), pbmp);
		
	}
	else
	{
		SetTextColor (hdc, PIXEL_black);
		DrawText (hdc, "lost image", -1, &rcClient, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	}
	
}  // end BEX_ImageStyleDraw()


// BKIMAGE 风格绘制函数
static void BEX_BkImageStyleDraw (HWND hCtrl, HDC hdc, PBEXDATA pData)
{
	int nTop;
	int nLeft;
	const char* spCaption = NULL;

	PBITMAP pbmpBk = pData->pbmpButton[pData->nState];
	PBITMAP pbmpIcon = pData->pbmpIcon;
	
	RECT rcText;
	RECT rcClient;
	
	GetClientRect (hCtrl, &rcClient);

	
	// 1: 绘制背景
	if ( NULL != pbmpBk )
	{
		// 透明背景图片
		if ( TRUE == pData->bTrans )
		{
			pbmpBk->bmType |= BMP_TYPE_COLORKEY;
			pbmpBk->bmColorKey = GetPixelInBitmap(pbmpBk, pData->pointTrans.x, pData->pointTrans.y);
		}
		else
		{
			pbmpBk->bmType &= ~BMP_TYPE_COLORKEY;
		}

		FillBoxWithBitmap (hdc, rcClient.left, rcClient.top, RECTW(rcClient), RECTH(rcClient), pbmpBk);
	}


				
	// 2: 绘制图标
	if ( NULL != pbmpIcon )
	{
		nLeft = rcClient.left + pData->nIconLeft;
		nTop = (rcClient.bottom - pbmpIcon->bmHeight) / 2;

		// 透明图标
		if ( TRUE == pData->bIconTrans )
		{
			pbmpIcon->bmType |= BMP_TYPE_COLORKEY;
			pbmpIcon->bmColorKey = GetPixelInBitmap(pbmpIcon, pData->pointIconTrans.x, pData->pointIconTrans.y);
		}
		else
		{
			pbmpIcon->bmType &= ~BMP_TYPE_COLORKEY;
		}

		// 计算有图标情况下文本位置
		rcText.left = rcClient.left + nLeft + pbmpIcon->bmWidth + pData->nIconDx;
		rcText.right = rcClient.right - 3;
		rcText.top = rcClient.top + 2;
		rcText.bottom = rcClient.bottom - 4;

		// 点击特效
		if ( BEXUI_CLICK == pData->nState && TRUE == pData->bClickEffect )
		{
			rcText.left += pData->nClickEffect;
			rcText.right += pData->nClickEffect;
			rcText.top += pData->nClickEffect;
			rcText.bottom += pData->nClickEffect;

			nLeft += pData->nClickEffect;
			nTop += pData->nClickEffect;
		}
		
		
		// disable 状态ICON Alpha 混合特效
		// 具有BMP_TYPE_PRIV_PIXEL (私有图像格式)和BMP_ALPHA (逐点Alpha 数值) 的bitmap,
		// 设置BMP_ALPHACHANNEL 无效了, 所以只好用内存DC 来进行alpha 混合了. 
		// 用32bit 的png 在载入的bitmap 是有私有图像格式和逐点Alpah 数值的.
		if ( BEXUI_DISABLE == pData->nState && TRUE == pData->bIconDisableEffect )
		{
			if ( HDC_INVALID != pData->hIconDC && NULL != pbmpBk )
			{
				// 注意是自己创建内存DC 的时候, 要用画刷把原来的东西清楚干净.
				// 否则会残留一些很奇怪的图像在内存DC 上的.
				SetBrushColor (pData->hIconDC, PIXEL_black);
				FillBox (pData->hIconDC, 0, 0, pbmpIcon->bmWidth, pbmpIcon->bmHeight);

				// 用背景图片与图标重叠的部分填充内存DC
				FillBoxWithBitmapPart (pData->hIconDC, 0, 0, pbmpIcon->bmWidth, pbmpIcon->bmHeight, 
					0, 0, pbmpBk, nLeft, nTop);

				// 绘制图标
				FillBoxWithBitmap (pData->hIconDC, 0, 0, 0, 0, pbmpIcon);

				// 设置alpha 混合值
				// alpha 通道值: 0 ~ 255
				// 0: 完全透明
				// 255: 完全不透明
				SetMemDCAlpha (pData->hIconDC, MEMDC_FLAG_SRCALPHA, pData->nIconDisableAlpha);

				// copy 到目标DC
				BitBlt (pData->hIconDC, 0, 0, pbmpIcon->bmWidth, pbmpIcon->bmHeight, hdc, nLeft, nTop, 0L);
				
			}
			
		}
		// 其他情况直接绘制图标
		else
		{
			FillBoxWithBitmap (hdc, nLeft, nTop, 0, 0, pbmpIcon);
		}
				
	}
	else
	{
		rcText.left = rcClient.left + 2;
		rcText.right = rcClient.right - 3;
		rcText.top = rcClient.top + 2;
		rcText.bottom = rcClient.bottom - 4;

		if ( BEXUI_CLICK == pData->nState && TRUE == pData->bClickEffect )
		{
			rcText.left += pData->nClickEffect;
			rcText.right += pData->nClickEffect;
			rcText.top += pData->nClickEffect;
			rcText.bottom += pData->nClickEffect;
		}
	}


	
	// 3: 绘制文本
	// GetWindowText 的实质就是获取Window Caption 的字体,
	// 而且还要发一次消息, 控件内部来说不如直接用GetWindowCaption 
	spCaption = GetWindowCaption (hCtrl);
	if ( NULL != spCaption )
	{
		if (  BEXUI_DISABLE == pData->nState )
			SetTextColor (hdc, pData->pixelTextDisable);
		else
			SetTextColor (hdc, pData->pixelTextNormal);
				
		SetBkMode (hdc, BM_TRANSPARENT);
		DrawText (hdc, spCaption, -1, &rcText, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	}
	
}  // end BEX_BKImageStyleDraw()


// Draw 风格绘制函数
static void BEX_DrawStyleDraw (HWND hCtrl, HDC hdc, PBEXDATA pData)
{
	int i;
	int nCorner = 2;
	const char* spCaption = NULL;
	
	int nLeft;
	int nTop;
	PBITMAP pbmpIcon = pData->pbmpIcon;

	RGB rgbBase;
	RGB rgbShade;
    	FIXED_RGB frgbBase;
	FIXED_RGB frgbShade;

	RECT rcText;
	RECT rcClient;
	
	GetClientRect (hCtrl, &rcClient);
	rcClient.right -= 1;
	rcClient.bottom -= 1;
	
	
	// 1: 绘制按钮背景
	// 无效状态
	if ( BEXUI_DISABLE == pData->nState )
	{
		SetBrushColor (hdc, pCData->pixelDisableBk);
		FillBox (hdc, rcClient.left, rcClient.top+nCorner, RECTW(rcClient), RECTH(rcClient)-nCorner);

		SetPenColor (hdc, pCData->pixelDisableBk);
		for ( i = 0; i < nCorner; i++ )
		{
       		LineEx (hdc, rcClient.left+(nCorner-1-i), rcClient.top+i, RECTW(rcClient)-(i+nCorner), rcClient.top+i);
			LineEx (hdc, rcClient.left+(nCorner-1-i), rcClient.bottom-i, RECTW(rcClient)-(i+nCorner), rcClient.bottom-i);
		}
		
	}
	// 非无效状态
	else
	{
		SETRGB (rgbBase, 
			pCData->rgbRenderNormalBase.r, pCData->rgbRenderNormalBase.g, pCData->rgbRenderNormalBase.b);
		SETRGB (rgbShade, 
			pCData->rgbRenderNormalShade.r, pCData->rgbRenderNormalShade.g, pCData->rgbRenderNormalShade.b);

		// 计算渐变颜色
		frgbShade = BEX_CalcRenderRGB (rgbBase, rgbShade, RECTH(rcClient)/2);

	
		// 绘制按钮上半部分
		SETRGB (frgbBase, rgbBase.r<<16, rgbBase.g<<16, rgbBase.b<<16);	
		for ( i = rcClient.bottom/2; i >= rcClient.top+nCorner; i-- )
		{
			SetPenColor (hdc, FixedRGB2Pixel(hdc, frgbBase.r, frgbBase.g, frgbBase.b));
        		LineEx (hdc, rcClient.left, i, RECTW(rcClient), i);
       		INCRGB (frgbBase, frgbShade);
		}

		// 边角
		for ( i = 0; i < nCorner; i++ )
		{
			SetPenColor (hdc, FixedRGB2Pixel(hdc, frgbBase.r, frgbBase.g, frgbBase.b));
       		LineEx (hdc, rcClient.left+(nCorner-1-i), rcClient.top+i, RECTW(rcClient)-(i+nCorner), rcClient.top+i);
       		INCRGB (frgbBase, frgbShade);
		}
			
	
		// 绘制按钮下半部分
		SETRGB (frgbBase, rgbBase.r<<16, rgbBase.g<<16, rgbBase.b<<16);
		for ( i = rcClient.bottom/2+1; i <= rcClient.bottom-nCorner; i++ )
		{
			SetPenColor (hdc, FixedRGB2Pixel(hdc, frgbBase.r, frgbBase.g, frgbBase.b));
        		LineEx (hdc, rcClient.left, i, rcClient.right-rcClient.left, i);
       		INCRGB (frgbBase, frgbShade);
		}

		// 边角
		for ( i = 0; i < nCorner; i++ )
		{
			SetPenColor (hdc, FixedRGB2Pixel(hdc, frgbBase.r, frgbBase.g, frgbBase.b));
       		LineEx (hdc, rcClient.left+(nCorner-1-i), rcClient.bottom-i, RECTW(rcClient)-(i+nCorner), rcClient.bottom-i);
       		INCRGB (frgbBase, frgbShade);
		}
		
	}  // end else()


	
	// 2: 绘制图标
	if ( NULL != pbmpIcon )
	{
		nLeft = rcClient.left + pData->nIconLeft;
		nTop = (rcClient.bottom - pbmpIcon->bmHeight) / 2;
		
		// 透明图标
		if ( TRUE == pData->bIconTrans )
		{
			pbmpIcon->bmType |= BMP_TYPE_COLORKEY;
			pbmpIcon->bmColorKey = GetPixelInBitmap(pbmpIcon, pData->pointIconTrans.x, pData->pointIconTrans.y);
		}
		else
		{
			pbmpIcon->bmType &= ~BMP_TYPE_COLORKEY;
		}

		// 计算有图标情况下文本位置
		rcText.left = rcClient.left + nLeft + pbmpIcon->bmWidth + pData->nIconDx;
		rcText.right = rcClient.right - 3;
		rcText.top = rcClient.top + 2;
		rcText.bottom = rcClient.bottom - 4;

		// 点击效果
		if ( BEXUI_CLICK == pData->nState )
		{
			rcText.left += pData->nClickEffect;
			rcText.right += pData->nClickEffect;
			rcText.top += pData->nClickEffect;
			rcText.bottom += pData->nClickEffect;
			
			nLeft += pData->nClickEffect;
			nTop += pData->nClickEffect;
		}
				
		
		// disable 状态ICON Alpha 混合特效
		if ( BEXUI_DISABLE == pData->nState && TRUE == pData->bIconDisableEffect )
		{
			if ( HDC_INVALID != pData->hIconDC )
			{
				// 用背景颜色填充内存DC
				// 这里正好设计的无效背景颜色为单色填充,
				// 哦也~~ 救了我命了, 不然没背景图片填充还真是麻烦. ^_^
				SetBrushColor (pData->hIconDC, pCData->pixelDisableBk);
				FillBox (pData->hIconDC, 0, 0, pbmpIcon->bmWidth, pbmpIcon->bmHeight);
					
				// 绘制图标
				FillBoxWithBitmap (pData->hIconDC, 0, 0, 0, 0, pbmpIcon);
					
				// 设置alpha 混合值
				SetMemDCAlpha (pData->hIconDC, MEMDC_FLAG_SRCALPHA, pData->nIconDisableAlpha);
					
				// copy 到目标DC
				BitBlt (pData->hIconDC, 0, 0, pbmpIcon->bmWidth, pbmpIcon->bmHeight, hdc, nLeft, nTop, 0L);
			}
		
		}
		// 其他情况直接绘制图标
		else
		{
			FillBoxWithBitmap (hdc, nLeft, nTop, 0, 0, pbmpIcon);
		}
				
	}
	else
	{
		SetRect (&rcText, rcClient.left+nCorner+2, rcClient.top+nCorner+2, 
			rcClient.right-nCorner-2, rcClient.bottom-nCorner-2);
		
		if ( BEXUI_CLICK == pData->nState )
		{
			rcText.left += pData->nClickEffect;
			rcText.right += pData->nClickEffect;
			rcText.top += pData->nClickEffect;
			rcText.bottom += pData->nClickEffect;
		}
	}
	

	
	// 3: 绘制焦点虚线
	if ( BEXUI_FOCUS == pData->nState || BEXUI_CLICK == pData->nState )
	{
		SetPenColor (hdc, pCData->pixelFocus);
		DrawHDotLine (hdc, rcClient.left+nCorner, rcClient.top+nCorner, RECTW(rcClient)-(nCorner+2));
		DrawHDotLine (hdc, rcClient.left+nCorner, rcClient.bottom-nCorner, RECTW(rcClient)-(nCorner+2));
		DrawVDotLine (hdc, rcClient.left+nCorner, rcClient.top+nCorner, RECTH(rcClient)-(nCorner+2));
		DrawVDotLine (hdc, rcClient.right-nCorner, rcClient.top+nCorner, RECTH(rcClient)-(nCorner+2));		
	}
	

	
	// 4: 绘制边框
	if ( BEXUI_DISABLE == pData->nState )
		SetPenColor (hdc, pData->pixelTextDisable);
	else
		SetPenColor (hdc, pCData->pixelBorder);

	// 上
	LineEx (hdc, rcClient.left+nCorner, rcClient.top, rcClient.right-nCorner, rcClient.top);
	LineEx (hdc, rcClient.left+nCorner, rcClient.top, rcClient.left, rcClient.top+nCorner);

	// 左
	LineEx (hdc, rcClient.left, rcClient.top+nCorner, rcClient.left, rcClient.bottom-nCorner);
	LineEx (hdc, rcClient.left, rcClient.bottom-nCorner, rcClient.left+nCorner, rcClient.bottom);

	// 下
	LineEx (hdc, rcClient.left+nCorner, rcClient.bottom, rcClient.right-nCorner, rcClient.bottom);
	LineEx (hdc, rcClient.right-nCorner, rcClient.bottom, rcClient.right, rcClient.bottom-nCorner);

	// 右
	LineEx (hdc, rcClient.right, rcClient.bottom-nCorner, rcClient.right, rcClient.top+nCorner);
	LineEx (hdc, rcClient.right, rcClient.top+nCorner, rcClient.right-nCorner, rcClient.top);


	
	// 5: 绘制文本
	spCaption = GetWindowCaption (hCtrl);
	if ( NULL != spCaption )
	{
		if (  BEXUI_DISABLE == pData->nState )
			SetTextColor (hdc, pData->pixelTextDisable);
		else
			SetTextColor (hdc, pData->pixelTextNormal);

		SetBkMode (hdc, BM_TRANSPARENT);
		DrawText (hdc, spCaption, -1, &rcText, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	}
	
}  // end BEX_DrawStyleDraw()




// ButtonEx 控件消息处理函数
static int ButtonExProc (HWND hCtrl, int message, WPARAM wParam, LPARAM lParam)
{
	int 		   nID;                    // 控件ID
	DWORD     dwStyle;             // 控件风格
	PBEXDATA pData = NULL;     // 控件数据

	dwStyle = GetWindowStyle (hCtrl);   // 获取控件风格
	nID = GetDlgCtrlID (hCtrl);              // 获取控件ID

	// 获取控件实例数据
	if ( message != MSG_CREATE )
		pData = (PBEXDATA) GetWindowAdditionalData2 (hCtrl);


	switch (message)
	{	
		// 创建控件
		case MSG_CREATE:
		{
			// 设置背景颜色为父窗口的背景颜色
			SetWindowBkColor (hCtrl, GetWindowBkColor((HWND)wParam));

			if ( BEX_ERR == BEX_InitCtrl(hCtrl) )
				return -1;
		
			// 还是调用下默认处理函数比较好
			break;
		}


		// 销毁控件
		case MSG_DESTROY:
		{
			BEX_Cleanup (hCtrl, pData);
			break;
		}


		// 重绘控件
		case MSG_PAINT:
		{
			HDC hdc = BeginPaint (hCtrl);
        		BEX_DrawCtrl (hCtrl, hdc, pData, dwStyle);
        		EndPaint (hCtrl, hdc);
        		return 0;
		}


		// 擦除背景
		case MSG_ERASEBKGND:
		{
			// 如果指定了窗口透明风格则不处理
			DWORD dwExStyle = GetWindowExStyle (hCtrl);
			if ( dwExStyle & WS_EX_TRANSPARENT )
				return 0;
			
            		const RECT* clip = (const RECT*) lParam;		            		                 
                	HDC hdc = GetClientDC (hCtrl);
			
			RECT rcTmp;
			RECT rcClient;
			GetClientRect (hCtrl, &rcClient);

			// 指定DC 绘制当前的无效区域
            		if ( NULL != clip )
			{
                		rcTmp = *clip;
                		ScreenToClient (hCtrl, &rcTmp.left, &rcTmp.top);
                		ScreenToClient (hCtrl, &rcTmp.right, &rcTmp.bottom);
                		IncludeClipRect (hdc, &rcTmp);
            		}

			// 用窗口背景颜色填充控件
			SetBrushColor (hdc, GetWindowBkColor(hCtrl));
            		FillBox (hdc, rcClient.left, rcClient.top, RECTW(rcClient), RECTH(rcClient));
			
               	ReleaseDC (hdc);
			return 0;
		}

		
		// 设置控件通用数据
		case BEXM_SETCOMDATA: 
		{
			PBEXDATA pSetData = (PBEXDATA)wParam;
			DWORD dwMask = (DWORD)lParam;
			if ( NULL == pData || NULL == pSetData )
				return BEX_ERR;

			// 设置图片信息
			if ( dwMask & BEX_COM_IMAGE )
			{
				pData->pbmpButton[BEXUI_NORMAL] = pSetData->pbmpButton[BEXUI_NORMAL];
				pData->pbmpButton[BEXUI_FOCUS] 	= pSetData->pbmpButton[BEXUI_FOCUS];
				pData->pbmpButton[BEXUI_CLICK] 	= pSetData->pbmpButton[BEXUI_CLICK];
				pData->pbmpButton[BEXUI_DISABLE] = pSetData->pbmpButton[BEXUI_DISABLE];

				pData->bTrans = pSetData->bTrans;
				pData->pointTrans.x = pSetData->pointTrans.x;
				pData->pointTrans.y = pSetData->pointTrans.y;
			}

			// 设置文本颜色
			if ( dwMask & BEX_COM_TEXTCOLOR )
			{
				pData->pixelTextNormal = pSetData->pixelTextNormal;
				pData->pixelTextDisable = pSetData->pixelTextDisable;
			}

			// 设置点击特效
			if ( dwMask & BEX_COM_CLICKEFFECT )
			{
				pData->bClickEffect = pSetData->bClickEffect;

				if ( dwStyle & BEXS_DRAW )
				{
					if ( pSetData->nClickEffect < 2 )
						pData->nClickEffect = 2;
					else
						pData->nClickEffect = pSetData->nClickEffect;
				}
				else
				{
					pData->nClickEffect = pSetData->nClickEffect;
				}
			}
			
			// 更新显示
			InvalidateRect (hCtrl, NULL, TRUE);
			return BEX_OK;
		}


		// 获取控件通用数据
		case BEXM_GETCOMDATA: 
		{
			PBEXDATA pGetData = (PBEXDATA)wParam;
			DWORD dwMask = (DWORD)lParam;
			if ( NULL == pData || NULL == pGetData )
				return BEX_ERR;

			// 获取图片信息
			if ( dwMask & BEX_COM_IMAGE )
			{
				pGetData->pbmpButton[BEXUI_NORMAL] 	= pData->pbmpButton[BEXUI_NORMAL];
				pGetData->pbmpButton[BEXUI_FOCUS] 	= pData->pbmpButton[BEXUI_FOCUS];
				pGetData->pbmpButton[BEXUI_CLICK] 	= pData->pbmpButton[BEXUI_CLICK];
				pGetData->pbmpButton[BEXUI_DISABLE] 	= pData->pbmpButton[BEXUI_DISABLE];

				pGetData->bTrans = pData->bTrans;
				pGetData->pointTrans.x = pData->pointTrans.x;
				pGetData->pointTrans.y = pData->pointTrans.y;
			}

			// 获取文本颜色
			if ( dwMask & BEX_COM_TEXTCOLOR )
			{
				pGetData->pixelTextNormal = pData->pixelTextNormal;
				pGetData->pixelTextDisable = pData->pixelTextDisable;
			}

			// 获取点击特效
			if ( dwMask & BEX_COM_CLICKEFFECT )
			{
				pGetData->bClickEffect = pData->bClickEffect;
				pGetData->nClickEffect = pData->nClickEffect;
			}
			
			return BEX_OK;
		}
		
		
		// 设置控件ICON 数据
		case BEXM_SETICON:
		{
			PBEXDATA pSetData = (PBEXDATA)wParam;
			DWORD dwMask = (DWORD)lParam;
			if ( NULL == pData || NULL == pSetData )
				return BEX_ERR;

			// 设置图标信息
			if ( dwMask & BEX_ICON_IMAGE )
			{
				pData->pbmpIcon = pSetData->pbmpIcon;

				pData->bIconTrans = pSetData->bIconTrans;
				pData->pointIconTrans.x = pSetData->pointIconTrans.x;
				pData->pointIconTrans.y = pSetData->pointIconTrans.y;
			}

			// 设置图标位置
			if ( dwMask & BEX_ICON_POSTION )
			{
				pData->nIconLeft = pSetData->nIconLeft;
				pData->nIconDx = pSetData->nIconDx;
			}

			// 设置disable 状态特效
			if ( dwMask & BEX_ICON_DISABLEEFFECT )
			{
				pData->bIconDisableEffect = pSetData->bIconDisableEffect;
				pData->nIconDisableAlpha = pSetData->nIconDisableAlpha;

				if ( TRUE == pData->bIconDisableEffect )
				{
					if ( HDC_INVALID == pData->hIconDC && NULL != pData->pbmpIcon )
					{
						pData->hIconDC = CreateCompatibleDCEx (HDC_SCREEN, 
							(pData->pbmpIcon)->bmWidth, (pData->pbmpIcon)->bmHeight);
					}
				}
				else
				{
					if ( HDC_INVALID != pData->hIconDC )
						DeleteMemDC (pData->hIconDC);

					pData->hIconDC = HDC_INVALID;	
				}
				
			}
			
			// 更新显示
			InvalidateRect (hCtrl, NULL, TRUE);
			return BEX_OK;
		}


		// 获取控件ICON 数据
		case BEXM_GETICON:
		{
			PBEXDATA pGetData = (PBEXDATA)wParam;
			DWORD dwMask = (DWORD)lParam;
			if ( NULL == pData || NULL == pGetData )
				return BEX_ERR;

			// 获取图标信息
			if ( dwMask & BEX_ICON_IMAGE )
			{
				pGetData->pbmpIcon = pData->pbmpIcon;

				pGetData->bIconTrans = pData->bIconTrans;
				pGetData->pointIconTrans.x = pData->pointIconTrans.x;
				pGetData->pointIconTrans.y = pData->pointIconTrans.y;
			}

			// 获取图标位置
			if ( dwMask & BEX_ICON_POSTION )
			{
				pGetData->nIconLeft = pData->nIconLeft;
				pGetData->nIconDx = pData->nIconDx;
			}

			// 获取disable 状态特效
			if ( dwMask & BEX_ICON_DISABLEEFFECT )
			{
				pGetData->bIconDisableEffect = pData->bIconDisableEffect;
				pGetData->nIconDisableAlpha = pData->nIconDisableAlpha;
			}
			
			return BEX_OK;
		}
		
		
		// 鼠标移入, 移出消息
		case MSG_MOUSEMOVEIN:
		{
			break;
			if ( NULL == pData ) 
				break;

			// 如果当前控件处于禁用状态, 则不处理
			if ( BEXUI_DISABLE == pData->nState ) 
				break;

			BOOL bIn = (BOOL) wParam;
			if ( TRUE == bIn )     // 移入
				SetFocus (hCtrl);
			else                         // 移出
				SetNullFocus (GetParent(hCtrl));

			break;
		}

		
		// 按下鼠标左键消息
		case MSG_LBUTTONDOWN:
		{
			if ( NULL == pData )
				break;
			
			// 捕获鼠标
			if ( GetCapture() == hCtrl )
				break;
			SetCapture (hCtrl);

			// 设置成click 状态
			pData->nState = BEXUI_FOCUS;
			InvalidateRect (hCtrl, NULL, TRUE);
			break;
		}

		
		// 弹起鼠标左键消息
		case MSG_LBUTTONUP:
		{
			if ( NULL == pData )
				break;
			
			if ( FALSE == IsWindowEnabled(hCtrl) ) 
				break;

			// 释放鼠标
			if ( GetCapture() == hCtrl ) 
            			ReleaseCapture ();

			// 原本是client 坐标的,
			// 但是一经过鼠标捕获后, 就变成srceen 坐标了
			int nX = LOSWORD (lParam);   // 弹起左键时的X 坐标
			int nY = HISWORD (lParam);    // 弹起左键时的Y 坐标

			// 控件位置及大小(Client 坐标)
			RECT rcCtrl;
			GetWindowRect (hCtrl, &rcCtrl);

			// 将screen 坐标转化为client 坐标
            		ScreenToClient (GetParent (hCtrl), &nX, &nY);

			// 点击落在控件客户区内才发送click 通知码
			if ( TRUE == PtInRect(&rcCtrl, nX, nY) )
			{
				//pData->nState = BEXUI_FOCUS;
				//InvalidateRect (hCtrl, NULL, TRUE);
				
				NotifyParent (hCtrl, nID, BEXN_CLICKED);
			}
			SetNullFocus (GetParent(hCtrl));
			break;
		}
		
		
		// 按下键盘消息
		case MSG_KEYDOWN:
		{
			if ( NULL == pData )
				break;
			
			// 只响应Enter, Space
			if ( SCANCODE_SPACE != wParam && SCANCODE_ENTER != wParam 
				&& SCANCODE_KEYPADENTER != wParam )
                		break;
			
			// 捕获鼠标
            		if ( GetCapture() == hCtrl )
                		break;
          		
            		SetCapture (hCtrl);
			
			// 设置成click 状态
            		pData->nState = BEXUI_CLICK;
            		InvalidateRect (hCtrl, NULL, TRUE);
			break;
		}


		// 弹起键盘消息
		case MSG_KEYUP:
		{
			if ( NULL == pData )
				break;

			if ( FALSE == IsWindowEnabled(hCtrl) )
				break;
			
			// 只响应Enter, Space
			if ( SCANCODE_SPACE != wParam && SCANCODE_ENTER != wParam 
				&& SCANCODE_KEYPADENTER != wParam )
                		break;

			// 释放鼠标
            		if ( GetCapture() == hCtrl )
            			ReleaseCapture ();

			// 设置成focus 状态
			pData->nState = BEXUI_FOCUS;
            		InvalidateRect (hCtrl, NULL, TRUE);
			
			// 如果处于焦点状态, 按下键盘的特定按键,
			// 则100% 发送click 通知消息. 模仿minigui 默认Button.
            		NotifyParent (hCtrl, nID, BEXN_CLICKED);
			break;
		}
		
		
		// 得到输入焦点
		case MSG_SETFOCUS:
		{
			if ( NULL == pData )
				break;

			if ( FALSE == IsWindowEnabled(hCtrl) )
				break;

			// 设置成focus 状态
			pData->nState = BEXUI_FOCUS;
			InvalidateRect (hCtrl, NULL, TRUE);
			break;
		}
		
		
		// 失去输入焦点
		case MSG_KILLFOCUS:
		{
			if ( NULL == pData )
				break;

			if ( FALSE == IsWindowEnabled(hCtrl) )
				break;
			
			// 释放鼠标
			if ( GetCapture() == hCtrl )
               		ReleaseCapture ();

			// 设置成normal 状态
			pData->nState = BEXUI_NORMAL;
			InvalidateRect (hCtrl, NULL, TRUE);
			break;
		}
		
		
		// 控件禁用, 使能消息
		case MSG_ENABLE:
		{
			if ( NULL == pData )
				break;
			
			BOOL bEnabled = (BOOL)wParam;
			if ( TRUE == bEnabled )
			{
				if ( GetFocus(GetParent(hCtrl)) == hCtrl )
					pData->nState = BEXUI_FOCUS;
				else
					pData->nState = BEXUI_NORMAL;
			}
			else
			{
				pData->nState = BEXUI_DISABLE;
			}

			InvalidateRect (hCtrl, NULL, TRUE);
			break;
		}


		// 设置文本信息
		case MSG_SETTEXT:
		{
            		SetWindowCaption (hCtrl, (char*)lParam);
            		InvalidateRect (hCtrl, NULL, TRUE);
            		return 0;
		}


		// 改变字体
		case MSG_FONTCHANGED:
		{
            		InvalidateRect (hCtrl, NULL, TRUE);
            		return 0;
		}

		
		// 获取对话框代码消息处理.
		// 只有在这里返回适当的值, 
		// 才能在模式对话框中接收得到
		// 鼠标, 键盘等输入消息.
		case MSG_GETDLGCODE:
		{
			//  这里参照了CTRL_BUTTON 的返回值
			return DLGC_PUSHBUTTON;
		}
		
		default: 
			break;
		
	} // end switch() 
	
	// 控件默认消息处理
	return DefaultControlProc (hCtrl, message, wParam, lParam);
	
} // end ButtonExProc()






