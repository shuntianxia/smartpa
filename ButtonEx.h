//********************************************************************
// Copyright (C) 2009 ~ 2010 mingming-killer
// Licence: GPL
//
// 项目: ControlEx
// 文件: ButtonEx.h
// 版本: V 1.3.3
//
// 说明: 自定义按钮. 
// 		    with minigui 2.0.4 API.
//
// 作者: mingming-killer
// 		    Email: mingming_killer@126.com
//
// 日期: 2010.2.3
// 
//*********************************************************************

#ifndef BUTTONEX_H
#define BUTTONEX_H


#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus




//*********************************************************************
//********************  控件数据结构定义 **********************
//*********************************************************************

// ButtonEx 界面状态
typedef enum _bexuistate_en
{
	BEXUI_NORMAL = 0,    // 正常状态
	BEXUI_FOCUS,            // 焦点状态
	BEXUI_CLICK,             // 按下状态
	BEXUI_DISABLE         // 禁用状态
	
} BEXUISTATE;


// ButtonEx 数据数据结构
typedef struct _bexdata_st
{
	BEXUISTATE nState;                // 控件当前的状态

	// 通用变量
	PBITMAP pbmpButton[4];        // 图片指针数组
	BOOL bTrans;                         // 是否使用图片透明颜色
	POINT pointTrans;                  // 透明像素点位置
 
	gal_pixel pixelTextNormal;    // 正常文本颜色
	gal_pixel pixelTextDisable;    // 控件无效文本颜色

	BOOL bClickEffect;                 // 是否使用Click 状态特效
	int      nClickEffect;                 // Click 状态特效幅度
	

	// ICON 变量
	PBITMAP pbmpIcon;                // 图标图片指针
	BOOL  bIconTrans;                  // 是否使用图标图片透明颜色
	POINT pointIconTrans;            // 透明像素点位置
	
	int nIconLeft;                           // 图标距控件客户区左端距离
	int nIconDx;                             // 图标距文本距离

	BOOL bIconDisableEffect;       // 是否使用disable 状态ICON 的Alpha 混合特效
	HDC hIconDC;                          // ICON Alpha 混合DC
	unsigned char nIconDisableAlpha; // ICON Alpha 混合值 
	
} BEXDATA;
typedef BEXDATA* PBEXDATA;




// ButtonEx 类数据数据结构
typedef struct _bexcdata_st
{
	// 渲染风格像素颜色
	// 这里保存RGB . 是因为从外面传入pixel 数值,
	// 经过控件内部的特殊转化后, 可能和原来的不一样了.
	RGB rgbRenderNormalBase;
	RGB rgbRenderNormalShade;
	
	
	gal_pixel pixelFocus;           // 焦点虚线颜色
	gal_pixel pixelDisableBk;     // 无效状态背景颜色
	gal_pixel pixelBorder;          // 边框颜色

} BEXCDATA;
typedef BEXCDATA* PBEXCDATA;

//*********************************************************************




//*********************************************************************
//***********************  接口函数列表  *************************
//*********************************************************************

//****************************************************
// 原型	: BOOL RegisterButtonExControl (void)
// 参数	: void
// 返回值: TRUE:  注册成功
//			  FALSE: 注册失败
// 说明	: 注册ButtonEx 控件, 
//			  在使用ButtonEx 控件前必须调用次函数进行注册
//****************************************************
extern BOOL RegisterButtonExControl (void);


//****************************************************
// 原型	: BOOL UnregisterButtonExControl (void)
// 参数	: void
// 返回值: TRUE:  卸载成功
//			  FALSE: 卸载失败
// 说明	: 卸载ButtonEx 控件
//*****************************************************
extern BOOL UnregisterButtonExControl (void);


//****************************************************
// 原型	: BOOL UnregisterButtonExControl (void)
// 参数	: void
// 返回值: TRUE:  卸载成功
//			  FALSE: 卸载失败
// 说明	: 卸载ButtonEx 控件
//*****************************************************
extern int BexSetCData (const PBEXCDATA pSetCData, DWORD dwMask);


//****************************************************
// 原型	: BOOL UnregisterButtonExControl (void)
// 参数	: void
// 返回值: TRUE:  卸载成功
//			  FALSE: 卸载失败
// 说明	: 卸载ButtonEx 控件
//*****************************************************
extern int BexGetCData (PBEXCDATA pGetCData, DWORD dwMask);


//*********************************************************************




//*********************************************************************
//*************************  消息定义 ******************************
//*********************************************************************

// 消息(message)
// 从最大的可用用户自定消息开始定义
// 防止自定消息冲突((0x0800 ~ 0xEFFF))
// 占用25 个
#define MSG_BEXMBASE	0xEFFF


//****************************************************
// 消息   	: BEXM_SETCOMDATA
//
// 参数	: PBEXDATA pSetData;   设置BEXDATA 指针
//			  DWORD dwMask;  要设置的数据类型
//			  wParam = (WPARAM)pSetData;
//                   lParam = (LPARAM)dwMask;
//
// 返回值: BEX_OK:  设置成功
//			  BEX_ERR:  设置失败
//
// 说明	: 设置控件通用数据
//
//  			  BEX_COM_IMAGE: 设置4 张bitmap 指针. 这4 张图片在
// 			  	IMAGE, BKIMAGE 风格下用来绘制正常(normal), 焦点(focus), 
// 			  	点击(click), 无效(disable) 4 种状态. 如果不需要disable 状态,
// 			  	可以给pbmpButton[BEXUI_DISABLE] 赋值NULL.
// 				可以是32 bit 的png (需要png 库支持), 在16 bit 色下能够保留alpha 透明信息,
// 				不过如果png 颜色渐变太明显的话, 在16bit 色下会有稍许失真.
// 				bmp 的可以设置btrans 变量, 开启透明掉某一种bmp 的背景颜色,
// 				pointTrans 设置透明颜色在bmp 的像素位置.
// 				注意要保证bmp 透明颜色的边缘颜色不要太明显的渐变, 
// 				否则锯齿严重.
// 				用png 或是bmp 开启btrans, 请使用CreateWindowEx () 创建控件, 并且指定
// 				WS_EX_TRANSPARENT, 这样可以使控件透明的部分呈现父窗口的颜色,
// 				从而创建不规则控件的效果.
// 				pbmpButton[BEXUI_NORMAL]: 按钮正常状态图片指针.
// 				pbmpButton[BEXUI_FOCUS]: 按钮处于焦急状态(在鼠标之下的)图片指针.
// 				pbmpButton[BEXUI_CLICK]: 按钮按下状态图片指针.
// 				pbmpButton[BEXUI_DISABLE]: 按钮无效状态图片指针.
// 				bTrans: 图片透明开关.
// 				pointTrans: 图片透明像素在图片中的位置(像素单位).
//
//  			  BEX_COM_TEXTCOLOR: 设置控件文字在normal 和disable 下的显示颜色(RGB).
// 				pixelTextNormal: 正常文本颜色.
// 				pixelTextDisable: 无效状态文本颜色.
//
// 			  BEX_COM_CLICKEFFCT: 设置BKIMAGE, DRAW 风格的Click 状态动态特效.
// 				bClickEffect: 点击效果开关.
// 				nClickEffect: 点击效果幅度.
// 				
// 			 
// 		    	  默认值:
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
// 消息   	: BEXM_GETCOMDATA
//
// 参数	: PBEXDATA pGetData;   获取BEXDATA 指针
//			  DWORD dwMask;  要获取的数据类型
//			  wParam = (WPARAM)pGetData;
//                   lParam = (LPARAM)dwMask;
//
// 返回值: BEX_OK:  获取成功
//			  BEX_ERR:  获取失败
//
// 说明	: 获取控件通用数据
//  			  具体请参看BEXM_SETCOMDATA.
//*****************************************************
#define BEXM_GETCOMDATA  		MSG_BEXMBASE - 2


//****************************************************
// 消息   	: BEXM_SETICON
// 
// 参数	: PBEXDATA pSetData;   设置BEXDATA 指针
//			  DWORD dwMask;  要设置的数据类型
//			  wParam = (WPARAM)pSetData;
//                   lParam = (LPARAM)dwMask;
//
// 返回值: BEX_OK:  设置成功
//			  BEX_ERR:  设置失败
//
// 说明	: 设置控件ICON 数据
//
//  			  BEX_ICON_IMAGE: 设置图标指针.
// 			  	可以是32bit png, 在16 bit 色下能保留alpha 透明信息.
// 				可以是gif . 不过gif 在 16 bit 色下不能保留alpha 透明信息, 
//  				请开启bIconTrans 选项, 用pointIconTrans 指定透明gif 的透明像素位置.
//  				pbmpIcon: 图标图片指针.
// 				bIconTrans: 图标图片透明开关.
// 				pointsIconTrans: 透明像素在原图片中的位置(像素单位).
//
//  			  BEX_ICON_POSITON: 设置的位置.
// 				nIconLeft : 设置图标距离左端客户区的位置.
// 				nIconDx : 设置图标距离控件文字的距离.
//
// 			  BEX_ICON_DISABLEEFFCT: 设置图标disable 状态特效.
// 				bIconDisableEffect : 开启disable 状态特效. 这个效果就是用disable 状态的图片,
// 					以一定的Alpha 值 混合到图标上, 达到图标也显示disable 状态的效果.
// 				nIconDisableAlpha : 设置disableEffect 的alpha 数值(0 ~ 255).
// 					0: 完全透明~ 255: 完全不透明.
//			
//			
// 			  默认值:
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
// 消息   	: BEXM_GETICON
//
// 参数	: PBEXDATA pGetData;   获取BEXDATA 指针
//			  DWORD dwMask;  要获取的数据类型
//			  wParam = (WPARAM)pGetData;
//                   lParam = (LPARAM)dwMask;
//
// 返回值: BEX_OK:  设置成功
//			  BEX_ERR:  设置失败
//
// 说明	: 获取控件ICON 数据
//  			  具体请参看BEXM_SETICON
//*****************************************************
#define BEXM_GETICON  		MSG_BEXMBASE - 4




// 通知码(notify code)
//****************************************************
// 通知码: BEXN_CLICKED
// 说明	: 在控件客户区内单击鼠标左键发送,
// 			  如果一直按住鼠标左键, 然后在客户区外放开的话,
// 			  则不会发送(模仿minigui 原有的Button)
//*****************************************************
#define BEXN_CLICKED  0   


//****************************************************
// 通知码: BEXN_DBLCLK
// 说明	: 暂时无用
//*****************************************************
#define BEXN_DBLCLK	   1




// ButtonEx 风格(style)
// 风格类型掩码
#define BEXS_TYPEMASK     	0x0000FFFFL

//****************************************************
// 风格: BEXS_IMAGE
// 说明: 完全用4 张图片来绘制控件.
// 		    默认风格, 即时在CreateWindow () 中不指定风格, 也会使用这个风格.
// 		    实例属性风格. (每个控件使用自己的数据)
// 		    可以使用bTrans 选项.
// 		    无法使用ICON 数据.
// 		    无法添加文字.
// 		    无法使用bClickEffect 选项.
//*****************************************************
#define BEXS_IMAGE 			0x00000001L    // 0...0001b


//****************************************************
// 风格: BEX_BKIMAGE
// 说明: 用4 张图片来充当控件背景.
// 		    请在CreateWindow () 或是CreateWindowEx () 中指定BEXS_BKIMAGE.
// 		    实例属性风格.
// 		    可以使用bTrans 选项. 
// 		    可以使用ICON 数据.
// 		    可以添加文字(单行).
// 		    可以使用bClickEffect 选项.
//*****************************************************
#define BEXS_BKIMAGE 		0x00000002L     // 0...0010b


//****************************************************
// 风格: BEXS_DRAW
// 说明: 用代码编程方式绘制控件.
// 		    请在CreateWindow () 或是CreateWindowEx () 中指定BEXS_DRAW.
// 	      	    类属性风格.(控件使用某些共同的数据)
// 		    无法使用bTrans 选项.
// 		    可以使用ICON 数据.
// 		    可以添加文字(单行).
// 		    强制使用bClickEffect 选项. 
// 		    但是修改nClickEffect 有效, 但是你不能设置这个数值< 2.
// 		    因为这个风格完全靠这个特效来表现click 状态了.
// 		    其实这个风格蛮鸡肋的, 因为有BKIMAGE 风格, 
// 		    图片做得好的话完全可以胜过这个风格. 
//	     	    但是如果你是PS 苦手还是可以考虑用这个风格.
// 		    PS: 这个风格的颜色色彩绘图算法是照抄飞漫的. -_-||
// 		    本人纯粹是为了好玩才弄这个风格的.
//*****************************************************
#define BEXS_DRAW  			0x00000004L     // 0...0100b


//*********************************************************************




//*********************************************************************
//**************************  其他定义 *****************************
//*********************************************************************

// ButtonEx 控件类名
#define CTRL_BUTTONEX   	"button_ex"

 
// 操作返回值
#define BEX_ERR		-1   // 操作失败
#define BEX_OK    	0     // 操作成功


// 通用数据设置
#define BEX_COM_IMAGE 			0x00000001L
#define BEX_COM_TEXTCOLOR 		0x00000002L
#define BEX_COM_CLICKEFFECT 	0x00000004L
#define BEX_COM_ALL 			((BEX_COM_IMAGE) | (BEX_COM_TEXTCOLOR) | (BEX_COM_CLICKEFFECT))


// ICON 数据设置操作
#define BEX_ICON_IMAGE 			0x00000001L
#define BEX_ICON_POSTION 		0x00000002L
#define BEX_ICON_DISABLEEFFECT 0x00000004L
#define BEX_ICON_ALL 			((BEX_ICON_IMAGE) | (BEX_ICON_POSTION) | (BEX_ICON_DISABLEEFFECT))


// 类控件数据类型
#define BEX_CDATA_RENDER 		0x00000001L
#define BEX_CDATA_DEFAULT		0x00000002L
#define BEX_CDATA_ALL  			((BEX_CDATA_RENDER))


#define SETRGB(rgb, rdata, gdata, bdata) \
	(rgb.r) = (rdata);\
	(rgb.g) = (gdata);\
	(rgb.b) = (bdata)



// Draw 风格类数据默认数值
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


