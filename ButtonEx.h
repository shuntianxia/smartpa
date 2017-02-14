//********************************************************************
// Copyright (C) 2009 ~ 2010 mingming-killer
// Licence: GPL
//
// ��Ŀ: ControlEx
// �ļ�: ButtonEx.h
// �汾: V 1.3.3
//
// ˵��: �Զ��尴ť. 
// 		    with minigui 2.0.4 API.
//
// ����: mingming-killer
// 		    Email: mingming_killer@126.com
//
// ����: 2010.2.3
// 
//*********************************************************************

#ifndef BUTTONEX_H
#define BUTTONEX_H


#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus




//*********************************************************************
//********************  �ؼ����ݽṹ���� **********************
//*********************************************************************

// ButtonEx ����״̬
typedef enum _bexuistate_en
{
	BEXUI_NORMAL = 0,    // ����״̬
	BEXUI_FOCUS,            // ����״̬
	BEXUI_CLICK,             // ����״̬
	BEXUI_DISABLE         // ����״̬
	
} BEXUISTATE;


// ButtonEx �������ݽṹ
typedef struct _bexdata_st
{
	BEXUISTATE nState;                // �ؼ���ǰ��״̬

	// ͨ�ñ���
	PBITMAP pbmpButton[4];        // ͼƬָ������
	BOOL bTrans;                         // �Ƿ�ʹ��ͼƬ͸����ɫ
	POINT pointTrans;                  // ͸�����ص�λ��
 
	gal_pixel pixelTextNormal;    // �����ı���ɫ
	gal_pixel pixelTextDisable;    // �ؼ���Ч�ı���ɫ

	BOOL bClickEffect;                 // �Ƿ�ʹ��Click ״̬��Ч
	int      nClickEffect;                 // Click ״̬��Ч����
	

	// ICON ����
	PBITMAP pbmpIcon;                // ͼ��ͼƬָ��
	BOOL  bIconTrans;                  // �Ƿ�ʹ��ͼ��ͼƬ͸����ɫ
	POINT pointIconTrans;            // ͸�����ص�λ��
	
	int nIconLeft;                           // ͼ���ؼ��ͻ�����˾���
	int nIconDx;                             // ͼ����ı�����

	BOOL bIconDisableEffect;       // �Ƿ�ʹ��disable ״̬ICON ��Alpha �����Ч
	HDC hIconDC;                          // ICON Alpha ���DC
	unsigned char nIconDisableAlpha; // ICON Alpha ���ֵ 
	
} BEXDATA;
typedef BEXDATA* PBEXDATA;




// ButtonEx ���������ݽṹ
typedef struct _bexcdata_st
{
	// ��Ⱦ���������ɫ
	// ���ﱣ��RGB . ����Ϊ�����洫��pixel ��ֵ,
	// �����ؼ��ڲ�������ת����, ���ܺ�ԭ���Ĳ�һ����.
	RGB rgbRenderNormalBase;
	RGB rgbRenderNormalShade;
	
	
	gal_pixel pixelFocus;           // ����������ɫ
	gal_pixel pixelDisableBk;     // ��Ч״̬������ɫ
	gal_pixel pixelBorder;          // �߿���ɫ

} BEXCDATA;
typedef BEXCDATA* PBEXCDATA;

//*********************************************************************




//*********************************************************************
//***********************  �ӿں����б�  *************************
//*********************************************************************

//****************************************************
// ԭ��	: BOOL RegisterButtonExControl (void)
// ����	: void
// ����ֵ: TRUE:  ע��ɹ�
//			  FALSE: ע��ʧ��
// ˵��	: ע��ButtonEx �ؼ�, 
//			  ��ʹ��ButtonEx �ؼ�ǰ������ôκ�������ע��
//****************************************************
extern BOOL RegisterButtonExControl (void);


//****************************************************
// ԭ��	: BOOL UnregisterButtonExControl (void)
// ����	: void
// ����ֵ: TRUE:  ж�سɹ�
//			  FALSE: ж��ʧ��
// ˵��	: ж��ButtonEx �ؼ�
//*****************************************************
extern BOOL UnregisterButtonExControl (void);


//****************************************************
// ԭ��	: BOOL UnregisterButtonExControl (void)
// ����	: void
// ����ֵ: TRUE:  ж�سɹ�
//			  FALSE: ж��ʧ��
// ˵��	: ж��ButtonEx �ؼ�
//*****************************************************
extern int BexSetCData (const PBEXCDATA pSetCData, DWORD dwMask);


//****************************************************
// ԭ��	: BOOL UnregisterButtonExControl (void)
// ����	: void
// ����ֵ: TRUE:  ж�سɹ�
//			  FALSE: ж��ʧ��
// ˵��	: ж��ButtonEx �ؼ�
//*****************************************************
extern int BexGetCData (PBEXCDATA pGetCData, DWORD dwMask);


//*********************************************************************




//*********************************************************************
//*************************  ��Ϣ���� ******************************
//*********************************************************************

// ��Ϣ(message)
// �����Ŀ����û��Զ���Ϣ��ʼ����
// ��ֹ�Զ���Ϣ��ͻ((0x0800 ~ 0xEFFF))
// ռ��25 ��
#define MSG_BEXMBASE	0xEFFF


//****************************************************
// ��Ϣ   	: BEXM_SETCOMDATA
//
// ����	: PBEXDATA pSetData;   ����BEXDATA ָ��
//			  DWORD dwMask;  Ҫ���õ���������
//			  wParam = (WPARAM)pSetData;
//                   lParam = (LPARAM)dwMask;
//
// ����ֵ: BEX_OK:  ���óɹ�
//			  BEX_ERR:  ����ʧ��
//
// ˵��	: ���ÿؼ�ͨ������
//
//  			  BEX_COM_IMAGE: ����4 ��bitmap ָ��. ��4 ��ͼƬ��
// 			  	IMAGE, BKIMAGE �����������������(normal), ����(focus), 
// 			  	���(click), ��Ч(disable) 4 ��״̬. �������Ҫdisable ״̬,
// 			  	���Ը�pbmpButton[BEXUI_DISABLE] ��ֵNULL.
// 				������32 bit ��png (��Ҫpng ��֧��), ��16 bit ɫ���ܹ�����alpha ͸����Ϣ,
// 				�������png ��ɫ����̫���ԵĻ�, ��16bit ɫ�»�������ʧ��.
// 				bmp �Ŀ�������btrans ����, ����͸����ĳһ��bmp �ı�����ɫ,
// 				pointTrans ����͸����ɫ��bmp ������λ��.
// 				ע��Ҫ��֤bmp ͸����ɫ�ı�Ե��ɫ��Ҫ̫���ԵĽ���, 
// 				����������.
// 				��png ����bmp ����btrans, ��ʹ��CreateWindowEx () �����ؼ�, ����ָ��
// 				WS_EX_TRANSPARENT, ��������ʹ�ؼ�͸���Ĳ��ֳ��ָ����ڵ���ɫ,
// 				�Ӷ�����������ؼ���Ч��.
// 				pbmpButton[BEXUI_NORMAL]: ��ť����״̬ͼƬָ��.
// 				pbmpButton[BEXUI_FOCUS]: ��ť���ڽ���״̬(�����֮�µ�)ͼƬָ��.
// 				pbmpButton[BEXUI_CLICK]: ��ť����״̬ͼƬָ��.
// 				pbmpButton[BEXUI_DISABLE]: ��ť��Ч״̬ͼƬָ��.
// 				bTrans: ͼƬ͸������.
// 				pointTrans: ͼƬ͸��������ͼƬ�е�λ��(���ص�λ).
//
//  			  BEX_COM_TEXTCOLOR: ���ÿؼ�������normal ��disable �µ���ʾ��ɫ(RGB).
// 				pixelTextNormal: �����ı���ɫ.
// 				pixelTextDisable: ��Ч״̬�ı���ɫ.
//
// 			  BEX_COM_CLICKEFFCT: ����BKIMAGE, DRAW ����Click ״̬��̬��Ч.
// 				bClickEffect: ���Ч������.
// 				nClickEffect: ���Ч������.
// 				
// 			 
// 		    	  Ĭ��ֵ:
// 			  pbmpButton[BEXUI_NORMAL] = NULL;
// 			  pbmpButton[BEXUI_FOCUS] = NULL;
// 			  pbmpButton[BEXUI_CLICK] = NULL;
// 			  pbmpButton[BEXUI_DISABLE] = NULL;
//
// 			  bTrans = FASLE;
//  			  pointTrans.x = 0;
// 			  pointTrans.y = 0;
//
//  			  pixelTextNormal = PIXEL_black;
// 			  pixelTextDisable = RGB2Pixel (HDC_SCREEN, 128, 128, 128);
//
// 		      	  bClickEffect = TRUE;
// 			  nClickEffect = 2;
// 
//*****************************************************
#define BEXM_SETCOMDATA 		MSG_BEXMBASE - 1


//****************************************************
// ��Ϣ   	: BEXM_GETCOMDATA
//
// ����	: PBEXDATA pGetData;   ��ȡBEXDATA ָ��
//			  DWORD dwMask;  Ҫ��ȡ����������
//			  wParam = (WPARAM)pGetData;
//                   lParam = (LPARAM)dwMask;
//
// ����ֵ: BEX_OK:  ��ȡ�ɹ�
//			  BEX_ERR:  ��ȡʧ��
//
// ˵��	: ��ȡ�ؼ�ͨ������
//  			  ������ο�BEXM_SETCOMDATA.
//*****************************************************
#define BEXM_GETCOMDATA  		MSG_BEXMBASE - 2


//****************************************************
// ��Ϣ   	: BEXM_SETICON
// 
// ����	: PBEXDATA pSetData;   ����BEXDATA ָ��
//			  DWORD dwMask;  Ҫ���õ���������
//			  wParam = (WPARAM)pSetData;
//                   lParam = (LPARAM)dwMask;
//
// ����ֵ: BEX_OK:  ���óɹ�
//			  BEX_ERR:  ����ʧ��
//
// ˵��	: ���ÿؼ�ICON ����
//
//  			  BEX_ICON_IMAGE: ����ͼ��ָ��.
// 			  	������32bit png, ��16 bit ɫ���ܱ���alpha ͸����Ϣ.
// 				������gif . ����gif �� 16 bit ɫ�²��ܱ���alpha ͸����Ϣ, 
//  				�뿪��bIconTrans ѡ��, ��pointIconTrans ָ��͸��gif ��͸������λ��.
//  				pbmpIcon: ͼ��ͼƬָ��.
// 				bIconTrans: ͼ��ͼƬ͸������.
// 				pointsIconTrans: ͸��������ԭͼƬ�е�λ��(���ص�λ).
//
//  			  BEX_ICON_POSITON: ���õ�λ��.
// 				nIconLeft : ����ͼ�������˿ͻ�����λ��.
// 				nIconDx : ����ͼ�����ؼ����ֵľ���.
//
// 			  BEX_ICON_DISABLEEFFCT: ����ͼ��disable ״̬��Ч.
// 				bIconDisableEffect : ����disable ״̬��Ч. ���Ч��������disable ״̬��ͼƬ,
// 					��һ����Alpha ֵ ��ϵ�ͼ����, �ﵽͼ��Ҳ��ʾdisable ״̬��Ч��.
// 				nIconDisableAlpha : ����disableEffect ��alpha ��ֵ(0 ~ 255).
// 					0: ��ȫ͸��~ 255: ��ȫ��͸��.
//			
//			
// 			  Ĭ��ֵ:
// 			  pbmpIcon = NULL;
//
// 			  bIconTrans = FASLE;
//  			  pointIconTrans.x = 0;
// 			  pointIconTrans.y = 0;
//
// 			  nIconLeft = 5;
// 			  nIconDx = 4;
//
// 		      	  bIconDisableEffect = FALSE;
// 			  nIconDisableAlpha = 128;
// 
//*****************************************************
#define BEXM_SETICON  		MSG_BEXMBASE - 3


//****************************************************
// ��Ϣ   	: BEXM_GETICON
//
// ����	: PBEXDATA pGetData;   ��ȡBEXDATA ָ��
//			  DWORD dwMask;  Ҫ��ȡ����������
//			  wParam = (WPARAM)pGetData;
//                   lParam = (LPARAM)dwMask;
//
// ����ֵ: BEX_OK:  ���óɹ�
//			  BEX_ERR:  ����ʧ��
//
// ˵��	: ��ȡ�ؼ�ICON ����
//  			  ������ο�BEXM_SETICON
//*****************************************************
#define BEXM_GETICON  		MSG_BEXMBASE - 4




// ֪ͨ��(notify code)
//****************************************************
// ֪ͨ��: BEXN_CLICKED
// ˵��	: �ڿؼ��ͻ����ڵ�������������,
// 			  ���һֱ��ס������, Ȼ���ڿͻ�����ſ��Ļ�,
// 			  �򲻻ᷢ��(ģ��minigui ԭ�е�Button)
//*****************************************************
#define BEXN_CLICKED  0   


//****************************************************
// ֪ͨ��: BEXN_DBLCLK
// ˵��	: ��ʱ����
//*****************************************************
#define BEXN_DBLCLK	   1




// ButtonEx ���(style)
// �����������
#define BEXS_TYPEMASK     	0x0000FFFFL

//****************************************************
// ���: BEXS_IMAGE
// ˵��: ��ȫ��4 ��ͼƬ�����ƿؼ�.
// 		    Ĭ�Ϸ��, ��ʱ��CreateWindow () �в�ָ�����, Ҳ��ʹ��������.
// 		    ʵ�����Է��. (ÿ���ؼ�ʹ���Լ�������)
// 		    ����ʹ��bTrans ѡ��.
// 		    �޷�ʹ��ICON ����.
// 		    �޷��������.
// 		    �޷�ʹ��bClickEffect ѡ��.
//*****************************************************
#define BEXS_IMAGE 			0x00000001L    // 0...0001b


//****************************************************
// ���: BEX_BKIMAGE
// ˵��: ��4 ��ͼƬ���䵱�ؼ�����.
// 		    ����CreateWindow () ����CreateWindowEx () ��ָ��BEXS_BKIMAGE.
// 		    ʵ�����Է��.
// 		    ����ʹ��bTrans ѡ��. 
// 		    ����ʹ��ICON ����.
// 		    �����������(����).
// 		    ����ʹ��bClickEffect ѡ��.
//*****************************************************
#define BEXS_BKIMAGE 		0x00000002L     // 0...0010b


//****************************************************
// ���: BEXS_DRAW
// ˵��: �ô����̷�ʽ���ƿؼ�.
// 		    ����CreateWindow () ����CreateWindowEx () ��ָ��BEXS_DRAW.
// 	      	    �����Է��.(�ؼ�ʹ��ĳЩ��ͬ������)
// 		    �޷�ʹ��bTrans ѡ��.
// 		    ����ʹ��ICON ����.
// 		    �����������(����).
// 		    ǿ��ʹ��bClickEffect ѡ��. 
// 		    �����޸�nClickEffect ��Ч, �����㲻�����������ֵ< 2.
// 		    ��Ϊ��������ȫ�������Ч������click ״̬��.
// 		    ��ʵ�����������ߵ�, ��Ϊ��BKIMAGE ���, 
// 		    ͼƬ���úõĻ���ȫ����ʤ��������. 
//	     	    �����������PS ���ֻ��ǿ��Կ�����������.
// 		    PS: ���������ɫɫ�ʻ�ͼ�㷨���ճ�������. -_-||
// 		    ���˴�����Ϊ�˺����Ū�������.
//*****************************************************
#define BEXS_DRAW  			0x00000004L     // 0...0100b


//*********************************************************************




//*********************************************************************
//**************************  �������� *****************************
//*********************************************************************

// ButtonEx �ؼ�����
#define CTRL_BUTTONEX   	"button_ex"

 
// ��������ֵ
#define BEX_ERR		-1   // ����ʧ��
#define BEX_OK    	0     // �����ɹ�


// ͨ����������
#define BEX_COM_IMAGE 			0x00000001L
#define BEX_COM_TEXTCOLOR 		0x00000002L
#define BEX_COM_CLICKEFFECT 	0x00000004L
#define BEX_COM_ALL 			((BEX_COM_IMAGE) | (BEX_COM_TEXTCOLOR) | (BEX_COM_CLICKEFFECT))


// ICON �������ò���
#define BEX_ICON_IMAGE 			0x00000001L
#define BEX_ICON_POSTION 		0x00000002L
#define BEX_ICON_DISABLEEFFECT 0x00000004L
#define BEX_ICON_ALL 			((BEX_ICON_IMAGE) | (BEX_ICON_POSTION) | (BEX_ICON_DISABLEEFFECT))


// ��ؼ���������
#define BEX_CDATA_RENDER 		0x00000001L
#define BEX_CDATA_DEFAULT		0x00000002L
#define BEX_CDATA_ALL  			((BEX_CDATA_RENDER))


#define SETRGB(rgb, rdata, gdata, bdata) \
	(rgb.r) = (rdata);\
	(rgb.g) = (gdata);\
	(rgb.b) = (bdata)



// Draw ���������Ĭ����ֵ
#define BEX_DEFCDATA_BASECOLOR_R 		255
#define BEX_DEFCDATA_BASECOLOR_G 		255
#define BEX_DEFCDATA_BASECOLOR_B 		255

#define BEX_DEFCDATA_SHADECOLOR_R 	180
#define BEX_DEFCDATA_SHADECOLOR_G 	180
#define BEX_DEFCDATA_SHADECOLOR_B 	180

#define BEX_DEFCDATA_PIXELFOCUS 		(PIXEL_black)
#define BEX_DEFCDATA_PIXELBORDER 		(PIXEL_black)
#define BEX_DEFCDATA_PIXELDISABLE 	( RGB2Pixel(HDC_SCREEN, 248, 247, 242) )


//*********************************************************************

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // end BUTTONEX_H 


