//********************************************************************
// Copyright (C) 2009 ~ 2010 mingming-killer
// Licence: GPL
//
// ��Ŀ: ControlEx
// �ļ�: ButtonEx.c
// �汾: V 1.3.3
//
// ˵��: �Զ��尴ť. 
// 		    with minigui 2.0.4 API.
//
// ����: minging-killer
// 		    Email: mingming_killer@126.com
//
// ����: 2010.2.3
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
//**********************   ˽�����ݽṹ **************************
//*********************************************************************

// ���޷��ų�����RGB ��ɫ
typedef struct _fixed_rgb_st
{
    fixed r;
    fixed g;
    fixed b;
} FIXED_RGB;


// �궨��
#define INCRGB(drgb, srgb) \
          (drgb.r) += (srgb.r);\
          (drgb.g) += (srgb.g);\
          (drgb.b) += (srgb.b)
          

#define FixedRGB2Pixel(hdc, r, g, b) \
          RGB2Pixel((hdc), (r)>>16, (g)>>16, (b)>>16)

//*********************************************************************



//*********************************************************************
//************************   ˽�к��� ******************************
//*********************************************************************

// ���ܸ�������
static int BEX_InitCtrl (HWND hCtrl);
static void BEX_Cleanup (HWND hCtrl, PBEXDATA pData);
static void BEX_DrawCtrl (HWND hCtrl, HDC hdc, PBEXDATA pData, DWORD dwStyle);
static void BEX_ImageStyleDraw (HWND hCtrl, HDC hdc, PBEXDATA pData);
static void BEX_BkImageStyleDraw (HWND hCtrl, HDC hdc, PBEXDATA pData);
static void BEX_DrawStyleDraw (HWND hCtrl, HDC hdc, PBEXDATA pData);
static FIXED_RGB BEX_CalcRenderRGB (RGB rgbBase, RGB rgbShade, int dist);
static inline int BEX_SetCDataToDefault (PBEXCDATA pCData);

// ��Ϣ������
static int ButtonExProc (HWND hCtrl, int message, WPARAM wParam, LPARAM lParam);

//*********************************************************************




//*********************************************************************
//***********************   �ļ��������� *************************
//*********************************************************************

// ButtonEx �ؼ�������, 
// ��һ���������е���CreateWindow() ������ButtonEx ʵ���о���Ч, 
// ���Խ�������
static PBEXCDATA pCData;

//*********************************************************************




//****************************************************
// ԭ��	: BOOL RegisterButtonExControl (void)
// ����	: void
// ����ֵ: TRUE:  ע��ɹ�
//			  FALSE: ע��ʧ��
// ˵��	: ע��ButtonEx �ؼ�, 
//			  ��ʹ��ButtonEx �ؼ�ǰ������ôκ�������ע��
//****************************************************
BOOL RegisterButtonExControl (void)
{
	WNDCLASS BEXClass;

	// ΪButton �����������ڴ�ռ�
	pCData = (PBEXCDATA)calloc (1, sizeof(BEXCDATA));
	if ( NULL == pCData )
		return FALSE;
	
	// ��д�ؼ�����Ϣ
	BEXClass.spClassName 	= CTRL_BUTTONEX;
	BEXClass.dwStyle		= WS_NONE;
	BEXClass.dwExStyle	= WS_EX_NONE;
	BEXClass.iBkColor		= PIXEL_lightwhite;
	//BEXClass.hCursor     	= GetSystemCursor (IDC_HAND_POINT);
	BEXClass.hCursor     	= GetSystemCursor(0);
	BEXClass.WinProc 		= ButtonExProc;
	BEXClass.dwAddData 	= (DWORD)pCData;

	
	// ��ʼ��������
	BEX_SetCDataToDefault (pCData);
	
	
	// ע��ButtonEx ��
	return RegisterWindowClass (&BEXClass);
	
} // end RegisterDigitEditControl() 


//****************************************************
// ԭ��	: BOOL UnregisterDigitEditControl (void)
// ����	: void
// ����ֵ: TRUE:  ж�سɹ�
//			  FALSE: ж��ʧ��
// ˵��	: ж��ButtonEx �ؼ�
//*****************************************************
BOOL UnregisterButtonExControl (void)
{
	// �ͷ�ButtonEx ������
	if ( NULL != pCData )
	{
		free (pCData);
		pCData = NULL;
	}

	// ж��ButtonEx ��
	return UnregisterWindowClass (CTRL_BUTTONEX);
}



//****************************************************
// ԭ��	: BOOL UnregisterDigitEditControl (void)
// ����	: void
// ����ֵ: TRUE:  ж�سɹ�
//			  FALSE: ж��ʧ��
// ˵��	: ж��ButtonEx �ؼ�
//*****************************************************
int BexSetCData (const PBEXCDATA pSetCData, DWORD dwMask)
{
	if ( dwMask & BEX_CDATA_DEFAULT )
		return BEX_SetCDataToDefault (pCData);
	
	if ( NULL == pSetCData || NULL == pCData ) 
		return BEX_ERR;
	
	
	// ������Ⱦ��ɫ
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
// ԭ��	: BOOL UnregisterDigitEditControl (void)
// ����	: void
// ����ֵ: TRUE:  ж�سɹ�
//			  FALSE: ж��ʧ��
// ˵��	: ж��ButtonEx �ؼ�
//*****************************************************
int BexGetCData (PBEXCDATA pGetCData, DWORD dwMask)
{
	if ( dwMask & BEX_CDATA_DEFAULT )
		return BEX_SetCDataToDefault (pGetCData);
	
	if ( NULL == pGetCData || NULL == pCData ) 
		return BEX_ERR;
	
	
	// ��ȡ��Ⱦ��ɫ
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




// ����ָ��������ΪĬ��ֵ
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


// ��ʼ��ButtonEx ʵ��
static int BEX_InitCtrl (HWND hCtrl)
{
	// Ϊ�ؼ�ʵ�����������ڴ�ռ�, ����ʼ��Ϊ0
	PBEXDATA pData = (PBEXDATA)calloc (1, sizeof(BEXDATA));
	if ( NULL == pData ) 
		return BEX_ERR;

	
	// ��ʼ���ؼ���Ա����
	// ͨ������
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


	// ICON ����
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


// �ͷ�ButtonEx ʵ��ռ�õ���Դ
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


// ����Render ���Ľ���RGB ��ɫ
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

// ͸����ɫ��Ե�����(������, �㷨Ŀǰ���Ǵ��-_-||)
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




// ���ƿؼ���ʾ
static void BEX_DrawCtrl (HWND hCtrl, HDC hdc, PBEXDATA pData, DWORD dwStyle)
{
	// ���ݲ�ͬ�Ŀؼ����, ���в�ͬ�Ļ��ƴ���
	switch (dwStyle & BEXS_TYPEMASK)
	{
		// ͼƬ���(Ĭ�Ϸ��)
		case BEXS_IMAGE:
		default:
		{
			if ( NULL == pData || HDC_INVALID == hdc )
				break;
			
			BEX_ImageStyleDraw (hCtrl, hdc, pData);
			break;
		}

		// ����ͼƬ���
		case BEXS_BKIMAGE:
		{
			if ( NULL == pData || HDC_INVALID == hdc )
				break;

			BEX_BkImageStyleDraw (hCtrl, hdc, pData);
			break;
		}
		
		// ������Ʒ��
		case BEXS_DRAW:
		{
			if ( NULL == pCData || NULL == pData || HDC_INVALID == hdc )
				break;

			BEX_DrawStyleDraw (hCtrl, hdc, pData);
			break;
		}
		
	}// end switch()
	
}// end BEX_DrawCtrl()


// IMAGE �����ƺ���
static void BEX_ImageStyleDraw (HWND hCtrl, HDC hdc, PBEXDATA pData)
{
	PBITMAP pbmp = pData->pbmpButton[pData->nState];

	RECT rcClient;	
	GetClientRect (hCtrl, &rcClient);

	
	// ��ͼƬ����ȫ���ƿؼ�
	if ( NULL != pbmp )
	{
		// ͸������ͼƬ
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


// BKIMAGE �����ƺ���
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

	
	// 1: ���Ʊ���
	if ( NULL != pbmpBk )
	{
		// ͸������ͼƬ
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


				
	// 2: ����ͼ��
	if ( NULL != pbmpIcon )
	{
		nLeft = rcClient.left + pData->nIconLeft;
		nTop = (rcClient.bottom - pbmpIcon->bmHeight) / 2;

		// ͸��ͼ��
		if ( TRUE == pData->bIconTrans )
		{
			pbmpIcon->bmType |= BMP_TYPE_COLORKEY;
			pbmpIcon->bmColorKey = GetPixelInBitmap(pbmpIcon, pData->pointIconTrans.x, pData->pointIconTrans.y);
		}
		else
		{
			pbmpIcon->bmType &= ~BMP_TYPE_COLORKEY;
		}

		// ������ͼ��������ı�λ��
		rcText.left = rcClient.left + nLeft + pbmpIcon->bmWidth + pData->nIconDx;
		rcText.right = rcClient.right - 3;
		rcText.top = rcClient.top + 2;
		rcText.bottom = rcClient.bottom - 4;

		// �����Ч
		if ( BEXUI_CLICK == pData->nState && TRUE == pData->bClickEffect )
		{
			rcText.left += pData->nClickEffect;
			rcText.right += pData->nClickEffect;
			rcText.top += pData->nClickEffect;
			rcText.bottom += pData->nClickEffect;

			nLeft += pData->nClickEffect;
			nTop += pData->nClickEffect;
		}
		
		
		// disable ״̬ICON Alpha �����Ч
		// ����BMP_TYPE_PRIV_PIXEL (˽��ͼ���ʽ)��BMP_ALPHA (���Alpha ��ֵ) ��bitmap,
		// ����BMP_ALPHACHANNEL ��Ч��, ����ֻ�����ڴ�DC ������alpha �����. 
		// ��32bit ��png �������bitmap ����˽��ͼ���ʽ�����Alpah ��ֵ��.
		if ( BEXUI_DISABLE == pData->nState && TRUE == pData->bIconDisableEffect )
		{
			if ( HDC_INVALID != pData->hIconDC && NULL != pbmpBk )
			{
				// ע�����Լ������ڴ�DC ��ʱ��, Ҫ�û�ˢ��ԭ���Ķ�������ɾ�.
				// ��������һЩ����ֵ�ͼ�����ڴ�DC �ϵ�.
				SetBrushColor (pData->hIconDC, PIXEL_black);
				FillBox (pData->hIconDC, 0, 0, pbmpIcon->bmWidth, pbmpIcon->bmHeight);

				// �ñ���ͼƬ��ͼ���ص��Ĳ�������ڴ�DC
				FillBoxWithBitmapPart (pData->hIconDC, 0, 0, pbmpIcon->bmWidth, pbmpIcon->bmHeight, 
					0, 0, pbmpBk, nLeft, nTop);

				// ����ͼ��
				FillBoxWithBitmap (pData->hIconDC, 0, 0, 0, 0, pbmpIcon);

				// ����alpha ���ֵ
				// alpha ͨ��ֵ: 0 ~ 255
				// 0: ��ȫ͸��
				// 255: ��ȫ��͸��
				SetMemDCAlpha (pData->hIconDC, MEMDC_FLAG_SRCALPHA, pData->nIconDisableAlpha);

				// copy ��Ŀ��DC
				BitBlt (pData->hIconDC, 0, 0, pbmpIcon->bmWidth, pbmpIcon->bmHeight, hdc, nLeft, nTop, 0L);
				
			}
			
		}
		// �������ֱ�ӻ���ͼ��
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


	
	// 3: �����ı�
	// GetWindowText ��ʵ�ʾ��ǻ�ȡWindow Caption ������,
	// ���һ�Ҫ��һ����Ϣ, �ؼ��ڲ���˵����ֱ����GetWindowCaption 
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


// Draw �����ƺ���
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
	
	
	// 1: ���ư�ť����
	// ��Ч״̬
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
	// ����Ч״̬
	else
	{
		SETRGB (rgbBase, 
			pCData->rgbRenderNormalBase.r, pCData->rgbRenderNormalBase.g, pCData->rgbRenderNormalBase.b);
		SETRGB (rgbShade, 
			pCData->rgbRenderNormalShade.r, pCData->rgbRenderNormalShade.g, pCData->rgbRenderNormalShade.b);

		// ���㽥����ɫ
		frgbShade = BEX_CalcRenderRGB (rgbBase, rgbShade, RECTH(rcClient)/2);

	
		// ���ư�ť�ϰ벿��
		SETRGB (frgbBase, rgbBase.r<<16, rgbBase.g<<16, rgbBase.b<<16);	
		for ( i = rcClient.bottom/2; i >= rcClient.top+nCorner; i-- )
		{
			SetPenColor (hdc, FixedRGB2Pixel(hdc, frgbBase.r, frgbBase.g, frgbBase.b));
        		LineEx (hdc, rcClient.left, i, RECTW(rcClient), i);
       		INCRGB (frgbBase, frgbShade);
		}

		// �߽�
		for ( i = 0; i < nCorner; i++ )
		{
			SetPenColor (hdc, FixedRGB2Pixel(hdc, frgbBase.r, frgbBase.g, frgbBase.b));
       		LineEx (hdc, rcClient.left+(nCorner-1-i), rcClient.top+i, RECTW(rcClient)-(i+nCorner), rcClient.top+i);
       		INCRGB (frgbBase, frgbShade);
		}
			
	
		// ���ư�ť�°벿��
		SETRGB (frgbBase, rgbBase.r<<16, rgbBase.g<<16, rgbBase.b<<16);
		for ( i = rcClient.bottom/2+1; i <= rcClient.bottom-nCorner; i++ )
		{
			SetPenColor (hdc, FixedRGB2Pixel(hdc, frgbBase.r, frgbBase.g, frgbBase.b));
        		LineEx (hdc, rcClient.left, i, rcClient.right-rcClient.left, i);
       		INCRGB (frgbBase, frgbShade);
		}

		// �߽�
		for ( i = 0; i < nCorner; i++ )
		{
			SetPenColor (hdc, FixedRGB2Pixel(hdc, frgbBase.r, frgbBase.g, frgbBase.b));
       		LineEx (hdc, rcClient.left+(nCorner-1-i), rcClient.bottom-i, RECTW(rcClient)-(i+nCorner), rcClient.bottom-i);
       		INCRGB (frgbBase, frgbShade);
		}
		
	}  // end else()


	
	// 2: ����ͼ��
	if ( NULL != pbmpIcon )
	{
		nLeft = rcClient.left + pData->nIconLeft;
		nTop = (rcClient.bottom - pbmpIcon->bmHeight) / 2;
		
		// ͸��ͼ��
		if ( TRUE == pData->bIconTrans )
		{
			pbmpIcon->bmType |= BMP_TYPE_COLORKEY;
			pbmpIcon->bmColorKey = GetPixelInBitmap(pbmpIcon, pData->pointIconTrans.x, pData->pointIconTrans.y);
		}
		else
		{
			pbmpIcon->bmType &= ~BMP_TYPE_COLORKEY;
		}

		// ������ͼ��������ı�λ��
		rcText.left = rcClient.left + nLeft + pbmpIcon->bmWidth + pData->nIconDx;
		rcText.right = rcClient.right - 3;
		rcText.top = rcClient.top + 2;
		rcText.bottom = rcClient.bottom - 4;

		// ���Ч��
		if ( BEXUI_CLICK == pData->nState )
		{
			rcText.left += pData->nClickEffect;
			rcText.right += pData->nClickEffect;
			rcText.top += pData->nClickEffect;
			rcText.bottom += pData->nClickEffect;
			
			nLeft += pData->nClickEffect;
			nTop += pData->nClickEffect;
		}
				
		
		// disable ״̬ICON Alpha �����Ч
		if ( BEXUI_DISABLE == pData->nState && TRUE == pData->bIconDisableEffect )
		{
			if ( HDC_INVALID != pData->hIconDC )
			{
				// �ñ�����ɫ����ڴ�DC
				// ����������Ƶ���Ч������ɫΪ��ɫ���,
				// ŶҲ~~ ����������, ��Ȼû����ͼƬ��仹�����鷳. ^_^
				SetBrushColor (pData->hIconDC, pCData->pixelDisableBk);
				FillBox (pData->hIconDC, 0, 0, pbmpIcon->bmWidth, pbmpIcon->bmHeight);
					
				// ����ͼ��
				FillBoxWithBitmap (pData->hIconDC, 0, 0, 0, 0, pbmpIcon);
					
				// ����alpha ���ֵ
				SetMemDCAlpha (pData->hIconDC, MEMDC_FLAG_SRCALPHA, pData->nIconDisableAlpha);
					
				// copy ��Ŀ��DC
				BitBlt (pData->hIconDC, 0, 0, pbmpIcon->bmWidth, pbmpIcon->bmHeight, hdc, nLeft, nTop, 0L);
			}
		
		}
		// �������ֱ�ӻ���ͼ��
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
	

	
	// 3: ���ƽ�������
	if ( BEXUI_FOCUS == pData->nState || BEXUI_CLICK == pData->nState )
	{
		SetPenColor (hdc, pCData->pixelFocus);
		DrawHDotLine (hdc, rcClient.left+nCorner, rcClient.top+nCorner, RECTW(rcClient)-(nCorner+2));
		DrawHDotLine (hdc, rcClient.left+nCorner, rcClient.bottom-nCorner, RECTW(rcClient)-(nCorner+2));
		DrawVDotLine (hdc, rcClient.left+nCorner, rcClient.top+nCorner, RECTH(rcClient)-(nCorner+2));
		DrawVDotLine (hdc, rcClient.right-nCorner, rcClient.top+nCorner, RECTH(rcClient)-(nCorner+2));		
	}
	

	
	// 4: ���Ʊ߿�
	if ( BEXUI_DISABLE == pData->nState )
		SetPenColor (hdc, pData->pixelTextDisable);
	else
		SetPenColor (hdc, pCData->pixelBorder);

	// ��
	LineEx (hdc, rcClient.left+nCorner, rcClient.top, rcClient.right-nCorner, rcClient.top);
	LineEx (hdc, rcClient.left+nCorner, rcClient.top, rcClient.left, rcClient.top+nCorner);

	// ��
	LineEx (hdc, rcClient.left, rcClient.top+nCorner, rcClient.left, rcClient.bottom-nCorner);
	LineEx (hdc, rcClient.left, rcClient.bottom-nCorner, rcClient.left+nCorner, rcClient.bottom);

	// ��
	LineEx (hdc, rcClient.left+nCorner, rcClient.bottom, rcClient.right-nCorner, rcClient.bottom);
	LineEx (hdc, rcClient.right-nCorner, rcClient.bottom, rcClient.right, rcClient.bottom-nCorner);

	// ��
	LineEx (hdc, rcClient.right, rcClient.bottom-nCorner, rcClient.right, rcClient.top+nCorner);
	LineEx (hdc, rcClient.right, rcClient.top+nCorner, rcClient.right-nCorner, rcClient.top);


	
	// 5: �����ı�
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




// ButtonEx �ؼ���Ϣ������
static int ButtonExProc (HWND hCtrl, int message, WPARAM wParam, LPARAM lParam)
{
	int 		   nID;                    // �ؼ�ID
	DWORD     dwStyle;             // �ؼ����
	PBEXDATA pData = NULL;     // �ؼ�����

	dwStyle = GetWindowStyle (hCtrl);   // ��ȡ�ؼ����
	nID = GetDlgCtrlID (hCtrl);              // ��ȡ�ؼ�ID

	// ��ȡ�ؼ�ʵ������
	if ( message != MSG_CREATE )
		pData = (PBEXDATA) GetWindowAdditionalData2 (hCtrl);


	switch (message)
	{	
		// �����ؼ�
		case MSG_CREATE:
		{
			// ���ñ�����ɫΪ�����ڵı�����ɫ
			SetWindowBkColor (hCtrl, GetWindowBkColor((HWND)wParam));

			if ( BEX_ERR == BEX_InitCtrl(hCtrl) )
				return -1;
		
			// ���ǵ�����Ĭ�ϴ������ȽϺ�
			break;
		}


		// ���ٿؼ�
		case MSG_DESTROY:
		{
			BEX_Cleanup (hCtrl, pData);
			break;
		}


		// �ػ�ؼ�
		case MSG_PAINT:
		{
			HDC hdc = BeginPaint (hCtrl);
        		BEX_DrawCtrl (hCtrl, hdc, pData, dwStyle);
        		EndPaint (hCtrl, hdc);
        		return 0;
		}


		// ��������
		case MSG_ERASEBKGND:
		{
			// ���ָ���˴���͸������򲻴���
			DWORD dwExStyle = GetWindowExStyle (hCtrl);
			if ( dwExStyle & WS_EX_TRANSPARENT )
				return 0;
			
            		const RECT* clip = (const RECT*) lParam;		            		                 
                	HDC hdc = GetClientDC (hCtrl);
			
			RECT rcTmp;
			RECT rcClient;
			GetClientRect (hCtrl, &rcClient);

			// ָ��DC ���Ƶ�ǰ����Ч����
            		if ( NULL != clip )
			{
                		rcTmp = *clip;
                		ScreenToClient (hCtrl, &rcTmp.left, &rcTmp.top);
                		ScreenToClient (hCtrl, &rcTmp.right, &rcTmp.bottom);
                		IncludeClipRect (hdc, &rcTmp);
            		}

			// �ô��ڱ�����ɫ���ؼ�
			SetBrushColor (hdc, GetWindowBkColor(hCtrl));
            		FillBox (hdc, rcClient.left, rcClient.top, RECTW(rcClient), RECTH(rcClient));
			
               	ReleaseDC (hdc);
			return 0;
		}

		
		// ���ÿؼ�ͨ������
		case BEXM_SETCOMDATA: 
		{
			PBEXDATA pSetData = (PBEXDATA)wParam;
			DWORD dwMask = (DWORD)lParam;
			if ( NULL == pData || NULL == pSetData )
				return BEX_ERR;

			// ����ͼƬ��Ϣ
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

			// �����ı���ɫ
			if ( dwMask & BEX_COM_TEXTCOLOR )
			{
				pData->pixelTextNormal = pSetData->pixelTextNormal;
				pData->pixelTextDisable = pSetData->pixelTextDisable;
			}

			// ���õ����Ч
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
			
			// ������ʾ
			InvalidateRect (hCtrl, NULL, TRUE);
			return BEX_OK;
		}


		// ��ȡ�ؼ�ͨ������
		case BEXM_GETCOMDATA: 
		{
			PBEXDATA pGetData = (PBEXDATA)wParam;
			DWORD dwMask = (DWORD)lParam;
			if ( NULL == pData || NULL == pGetData )
				return BEX_ERR;

			// ��ȡͼƬ��Ϣ
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

			// ��ȡ�ı���ɫ
			if ( dwMask & BEX_COM_TEXTCOLOR )
			{
				pGetData->pixelTextNormal = pData->pixelTextNormal;
				pGetData->pixelTextDisable = pData->pixelTextDisable;
			}

			// ��ȡ�����Ч
			if ( dwMask & BEX_COM_CLICKEFFECT )
			{
				pGetData->bClickEffect = pData->bClickEffect;
				pGetData->nClickEffect = pData->nClickEffect;
			}
			
			return BEX_OK;
		}
		
		
		// ���ÿؼ�ICON ����
		case BEXM_SETICON:
		{
			PBEXDATA pSetData = (PBEXDATA)wParam;
			DWORD dwMask = (DWORD)lParam;
			if ( NULL == pData || NULL == pSetData )
				return BEX_ERR;

			// ����ͼ����Ϣ
			if ( dwMask & BEX_ICON_IMAGE )
			{
				pData->pbmpIcon = pSetData->pbmpIcon;

				pData->bIconTrans = pSetData->bIconTrans;
				pData->pointIconTrans.x = pSetData->pointIconTrans.x;
				pData->pointIconTrans.y = pSetData->pointIconTrans.y;
			}

			// ����ͼ��λ��
			if ( dwMask & BEX_ICON_POSTION )
			{
				pData->nIconLeft = pSetData->nIconLeft;
				pData->nIconDx = pSetData->nIconDx;
			}

			// ����disable ״̬��Ч
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
			
			// ������ʾ
			InvalidateRect (hCtrl, NULL, TRUE);
			return BEX_OK;
		}


		// ��ȡ�ؼ�ICON ����
		case BEXM_GETICON:
		{
			PBEXDATA pGetData = (PBEXDATA)wParam;
			DWORD dwMask = (DWORD)lParam;
			if ( NULL == pData || NULL == pGetData )
				return BEX_ERR;

			// ��ȡͼ����Ϣ
			if ( dwMask & BEX_ICON_IMAGE )
			{
				pGetData->pbmpIcon = pData->pbmpIcon;

				pGetData->bIconTrans = pData->bIconTrans;
				pGetData->pointIconTrans.x = pData->pointIconTrans.x;
				pGetData->pointIconTrans.y = pData->pointIconTrans.y;
			}

			// ��ȡͼ��λ��
			if ( dwMask & BEX_ICON_POSTION )
			{
				pGetData->nIconLeft = pData->nIconLeft;
				pGetData->nIconDx = pData->nIconDx;
			}

			// ��ȡdisable ״̬��Ч
			if ( dwMask & BEX_ICON_DISABLEEFFECT )
			{
				pGetData->bIconDisableEffect = pData->bIconDisableEffect;
				pGetData->nIconDisableAlpha = pData->nIconDisableAlpha;
			}
			
			return BEX_OK;
		}
		
		
		// �������, �Ƴ���Ϣ
		case MSG_MOUSEMOVEIN:
		{
			break;
			if ( NULL == pData ) 
				break;

			// �����ǰ�ؼ����ڽ���״̬, �򲻴���
			if ( BEXUI_DISABLE == pData->nState ) 
				break;

			BOOL bIn = (BOOL) wParam;
			if ( TRUE == bIn )     // ����
				SetFocus (hCtrl);
			else                         // �Ƴ�
				SetNullFocus (GetParent(hCtrl));

			break;
		}

		
		// ������������Ϣ
		case MSG_LBUTTONDOWN:
		{
			if ( NULL == pData )
				break;
			
			// �������
			if ( GetCapture() == hCtrl )
				break;
			SetCapture (hCtrl);

			// ���ó�click ״̬
			pData->nState = BEXUI_FOCUS;
			InvalidateRect (hCtrl, NULL, TRUE);
			break;
		}

		
		// ������������Ϣ
		case MSG_LBUTTONUP:
		{
			if ( NULL == pData )
				break;
			
			if ( FALSE == IsWindowEnabled(hCtrl) ) 
				break;

			// �ͷ����
			if ( GetCapture() == hCtrl ) 
            			ReleaseCapture ();

			// ԭ����client �����,
			// ����һ������겶���, �ͱ��srceen ������
			int nX = LOSWORD (lParam);   // �������ʱ��X ����
			int nY = HISWORD (lParam);    // �������ʱ��Y ����

			// �ؼ�λ�ü���С(Client ����)
			RECT rcCtrl;
			GetWindowRect (hCtrl, &rcCtrl);

			// ��screen ����ת��Ϊclient ����
            		ScreenToClient (GetParent (hCtrl), &nX, &nY);

			// ������ڿؼ��ͻ����ڲŷ���click ֪ͨ��
			if ( TRUE == PtInRect(&rcCtrl, nX, nY) )
			{
				//pData->nState = BEXUI_FOCUS;
				//InvalidateRect (hCtrl, NULL, TRUE);
				
				NotifyParent (hCtrl, nID, BEXN_CLICKED);
			}
			SetNullFocus (GetParent(hCtrl));
			break;
		}
		
		
		// ���¼�����Ϣ
		case MSG_KEYDOWN:
		{
			if ( NULL == pData )
				break;
			
			// ֻ��ӦEnter, Space
			if ( SCANCODE_SPACE != wParam && SCANCODE_ENTER != wParam 
				&& SCANCODE_KEYPADENTER != wParam )
                		break;
			
			// �������
            		if ( GetCapture() == hCtrl )
                		break;
          		
            		SetCapture (hCtrl);
			
			// ���ó�click ״̬
            		pData->nState = BEXUI_CLICK;
            		InvalidateRect (hCtrl, NULL, TRUE);
			break;
		}


		// ���������Ϣ
		case MSG_KEYUP:
		{
			if ( NULL == pData )
				break;

			if ( FALSE == IsWindowEnabled(hCtrl) )
				break;
			
			// ֻ��ӦEnter, Space
			if ( SCANCODE_SPACE != wParam && SCANCODE_ENTER != wParam 
				&& SCANCODE_KEYPADENTER != wParam )
                		break;

			// �ͷ����
            		if ( GetCapture() == hCtrl )
            			ReleaseCapture ();

			// ���ó�focus ״̬
			pData->nState = BEXUI_FOCUS;
            		InvalidateRect (hCtrl, NULL, TRUE);
			
			// ������ڽ���״̬, ���¼��̵��ض�����,
			// ��100% ����click ֪ͨ��Ϣ. ģ��minigui Ĭ��Button.
            		NotifyParent (hCtrl, nID, BEXN_CLICKED);
			break;
		}
		
		
		// �õ����뽹��
		case MSG_SETFOCUS:
		{
			if ( NULL == pData )
				break;

			if ( FALSE == IsWindowEnabled(hCtrl) )
				break;

			// ���ó�focus ״̬
			pData->nState = BEXUI_FOCUS;
			InvalidateRect (hCtrl, NULL, TRUE);
			break;
		}
		
		
		// ʧȥ���뽹��
		case MSG_KILLFOCUS:
		{
			if ( NULL == pData )
				break;

			if ( FALSE == IsWindowEnabled(hCtrl) )
				break;
			
			// �ͷ����
			if ( GetCapture() == hCtrl )
               		ReleaseCapture ();

			// ���ó�normal ״̬
			pData->nState = BEXUI_NORMAL;
			InvalidateRect (hCtrl, NULL, TRUE);
			break;
		}
		
		
		// �ؼ�����, ʹ����Ϣ
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


		// �����ı���Ϣ
		case MSG_SETTEXT:
		{
            		SetWindowCaption (hCtrl, (char*)lParam);
            		InvalidateRect (hCtrl, NULL, TRUE);
            		return 0;
		}


		// �ı�����
		case MSG_FONTCHANGED:
		{
            		InvalidateRect (hCtrl, NULL, TRUE);
            		return 0;
		}

		
		// ��ȡ�Ի��������Ϣ����.
		// ֻ�������ﷵ���ʵ���ֵ, 
		// ������ģʽ�Ի����н��յõ�
		// ���, ���̵�������Ϣ.
		case MSG_GETDLGCODE:
		{
			//  ���������CTRL_BUTTON �ķ���ֵ
			return DLGC_PUSHBUTTON;
		}
		
		default: 
			break;
		
	} // end switch() 
	
	// �ؼ�Ĭ����Ϣ����
	return DefaultControlProc (hCtrl, message, wParam, lParam);
	
} // end ButtonExProc()






