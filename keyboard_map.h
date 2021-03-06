#ifndef KEYBOARD_MAP_H
#define KEYBOARD_MAP_H

typedef enum
{
    PJT_On           = 1,   //开机
    PJT_Off          = 3,   //关机
    PJT_Computer     = 5,   //电脑
    PJT_Video        = 7,   //视频
    PJT_SignalSource = 9,   //信号源
    PJT_FocusAdd     = 11,  //变焦＋
    PJT_FocusRed     = 13,  //变焦－
    PJT_PictureAdd   = 15,  //画面＋
    PJT_PictureRed   = 17,  //画面－
    PJT_Menu         = 19,  //菜单
    PJT_Confirm      = 21,  //确认
    PJT_Up           = 23,  //上
    PJT_Left         = 25,  //左
    PJT_Right        = 27,  //右
    PJT_Down         = 29,  //下
    PJT_Quit         = 31,  //退出
    PJT_VolAdd       = 33,  //音量＋
    PJT_VolRed       = 35,  //音量－
    PJT_Mute         = 37,  //静音
    PJT_Auto         = 39,  //自动
    PJT_Pause        = 41,  //暂停
    PJT_MCD          = 43   //亮度
    
}PJTKeyBoadMap;

typedef enum
{
    FAN_On_Off      =1,   //开关
    FAN_On_speed    =3,   //开／风速
    FAN_Shake       =5,   //摇头
    FAN_Mode        =7,   //风类（模式）
    FAN_Timer       =9,   //定时
    FAN_Light       =11,  //灯光
    FAN_Anion       =13,  //负离子
    FAN_1           =15,  //1
    FAN_2           =17,  //2
    FAN_3           =19,  //3
    FAN_4           =21,  //4
    FAN_5           =23,  //5
    FAN_6           =25,  //6
    FAN_7           =27,  //7
    FAN_8           =29,  //8
    FAN_9           =31,  //9
    FAN_Sleep       =33,  //睡眠
    FAN_Cold        =35,  //制冷
    FAN_AirVol      =37,  //风量
    FAN_LowSpeed    =39,  //低速
    FAN_MiddleSpeed =41,  //中速
    FAN_HighSpeed   =43   //高速

}FanKeyBoadMap;

typedef enum
{
    TVBOX_Wait    = 1,  //待机
    TVBOX_1       = 3,  //1
    TVBOX_2       = 5,  //2
    TVBOX_3       = 7,  //3
    TVBOX_4       = 9,  //4
    TVBOX_5       = 11, //5
    TVBOX_6       = 13, //6
    TVBOX_7       = 15, //7
    TVBOX_8       = 17, //8
    TVBOX_9       = 19, //9
    TVBOX_Lead    = 21, //导视
    TVBOX_0       = 23, //0
    TVBOX_Back    = 25, //返回
    TVBOX_Up      = 27, //上
    TVBOX_Left    = 29, //左
    TVBOX_Comfirm = 31, //确定
    TVBOX_Right   = 33, //右
    TVBOX_Down    = 35, //下
    TVBOX_VolAdd  = 37, //声音＋
    TVBOX_VolRed  = 39, //声音－
    TVBOX_ChAdd   = 41, //频道＋
    TVBOX_ChRed   = 43, //频道－
    TVBOX_Menu    = 45  //菜单
    
}TVBoxKeyBoadMap;

typedef enum
{
    IPTV_Power  = 1,  //电源
    IPTV_Mute   = 3,  //静音
    IPTV_VolAdd = 5,  //音量＋
    IPTV_VolRed = 7,  //音量－
    IPTV_ChAdd  = 9,  //频道＋
    IPTV_ChRed  = 11, //频道-
    IPTV_Up     = 13, //上
    IPTV_Left   = 15, //左
    IPTV_OK     = 17, //OK
    IPTV_Right  = 19, //右
    IPTV_Down   = 21, //下
    IPTV_Play   = 23, //播放／暂停
    IPTV_1      = 25, //1
    IPTV_2      = 27, //2
    IPTV_3      = 29, //3
    IPTV_4      = 31, //4
    IPTV_5      = 33, //5
    IPTV_6      = 35, //6
    IPTV_7      = 37, //7
    IPTV_8      = 39, //8
    IPTV_9      = 41, //9
    IPTV_0      = 43, //0
    IPTV_BACK   = 45  //返回
    
}IPTVKeyBoadMap;

typedef enum
{
    TV_VolAdd  = 1,   //声音＋
    TV_ChAdd   = 3,   //频道＋
    TV_Menu    = 5,   //菜单
    TV_ChRed   = 7,   //频道－
    TV_VolRed  = 9,   //声音－
    TV_Power   = 11,  //电源
    TV_Mute    = 13,  //静音
    TV_1       = 15,  //1
    TV_2       = 17,  //2
    TV_3       = 19,  //3
    TV_4       = 21,  //4
    TV_5       = 23,  //5
    TV_6       = 25,  //6
    TV_7       = 27,  //7
    TV_8       = 29,  //8
    TV_9       = 31,  //9
    TV_Res     = 33,  //--/-
    TV_0       = 35,  //0
    TV_AV_TV   = 37,  //AV/TV
    TV_Back    = 39,  //返回
    TV_Comfirm = 41,  //确定
    TV_Up      = 43,  //上
    TV_Left    = 45,  //左
    TV_Right   = 47,  //右
    TV_Down    = 49,  //下
    
}TVKeyBoadMap;

typedef enum
{
    DVD_Left    = 1,   //左
    DVD_Up      = 3,   //上
    DVD_Ok      = 5,   //ok
    DVD_Down    = 7,   //下
    DVD_Right   = 9,   //右
    DVD_Power   = 11,  //电源
    DVD_Mute    = 13,  //静音
    DVD_FBack   = 15,  //快倒
    DVD_Play    = 17,  //播放
    DVD_FForwad = 19,  //快进
    DVD_Prev    = 21,  //上一曲
    DVD_Stop    = 23,  //停止
    DVD_Next    = 25,  //下一曲
    DVD_Format  = 27,  //制式
    DVD_Pause   = 29,  //暂停
    DVD_Title   = 31,  //标题
    DVD_SK      = 33,  //开关仓
    DVD_Menu    = 35,  //静音
    DVD_Back    = 37   //返回
    
}DVDKeyBoadMap;

typedef enum
{
    ARC_Power  = 0x01,  //电源
    ARC_Mode   = 0x02,  //模式
    ARC_Vol    = 0x03,  //风量
    ARC_M      = 0x04,  //手动风向
    ARC_A      = 0x05,  //自动风向，
    ARC_tmpAdd = 0x06,  //温度＋
    ARC_tmpRed = 0x07,  //温度－
    
}ARCKeyBoadMap;

typedef enum
{
    WH_Power   = 1,  //电源
    WH_Set     = 3,  //设置
    WH_TemAdd  = 5,  //温度＋
    WH_TemRed  = 7,  //温度－
    WH_Mode    = 9,  //模式
    WH_Confirm = 11, //确定
    WH_Timer   = 13, //定时
    WH_Ant     = 15, //预约
    WH_Time    = 17, //时间
    WH_Stem    = 19, //保温
    
}WheaterKeyBoadMap;

typedef enum
{
    AIR_Power   = 1,  //电源
    AIR_Auto    = 3,  //自动
    AIR_AirVol  = 5,  //风量
    AIR_Timer   = 7,  //定时
    AIR_Mode    = 9,  //模式
    AIR_Anion   = 11, //负离子
    AIR_Comfort = 13, //舒适
    AIR_Mute    = 15,  //静音
    AIR_LightOff = 17,
    AIR_Strong	= 19,
    AIR_Natural	= 21,
    AIR_Close = 23,
    AIR_Sleep = 25,
    AIR_Smart = 27,
    AIR_Light1 = 29,
    AIR_Light2 = 31,
    AIR_Light3 = 33,
    AIR_UV = 35    
}AirKeyBoadMap;

typedef enum
{
    SLR_TPIC    = 1,  //拍照
    
}SLRKeyBoadMap;

#endif /* KEYBOARD_MAP_H */
