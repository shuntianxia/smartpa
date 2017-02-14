#ifndef KEYBOARD_MAP_H
#define KEYBOARD_MAP_H

typedef enum
{
    PJT_On           = 1,   //����
    PJT_Off          = 3,   //�ػ�
    PJT_Computer     = 5,   //����
    PJT_Video        = 7,   //��Ƶ
    PJT_SignalSource = 9,   //�ź�Դ
    PJT_FocusAdd     = 11,  //�佹��
    PJT_FocusRed     = 13,  //�佹��
    PJT_PictureAdd   = 15,  //���棫
    PJT_PictureRed   = 17,  //���棭
    PJT_Menu         = 19,  //�˵�
    PJT_Confirm      = 21,  //ȷ��
    PJT_Up           = 23,  //��
    PJT_Left         = 25,  //��
    PJT_Right        = 27,  //��
    PJT_Down         = 29,  //��
    PJT_Quit         = 31,  //�˳�
    PJT_VolAdd       = 33,  //������
    PJT_VolRed       = 35,  //������
    PJT_Mute         = 37,  //����
    PJT_Auto         = 39,  //�Զ�
    PJT_Pause        = 41,  //��ͣ
    PJT_MCD          = 43   //����
    
}PJTKeyBoadMap;

typedef enum
{
    FAN_On_Off      =1,   //����
    FAN_On_speed    =3,   //��������
    FAN_Shake       =5,   //ҡͷ
    FAN_Mode        =7,   //���ࣨģʽ��
    FAN_Timer       =9,   //��ʱ
    FAN_Light       =11,  //�ƹ�
    FAN_Anion       =13,  //������
    FAN_1           =15,  //1
    FAN_2           =17,  //2
    FAN_3           =19,  //3
    FAN_4           =21,  //4
    FAN_5           =23,  //5
    FAN_6           =25,  //6
    FAN_7           =27,  //7
    FAN_8           =29,  //8
    FAN_9           =31,  //9
    FAN_Sleep       =33,  //˯��
    FAN_Cold        =35,  //����
    FAN_AirVol      =37,  //����
    FAN_LowSpeed    =39,  //����
    FAN_MiddleSpeed =41,  //����
    FAN_HighSpeed   =43   //����

}FanKeyBoadMap;

typedef enum
{
    TVBOX_Wait    = 1,  //����
    TVBOX_1       = 3,  //1
    TVBOX_2       = 5,  //2
    TVBOX_3       = 7,  //3
    TVBOX_4       = 9,  //4
    TVBOX_5       = 11, //5
    TVBOX_6       = 13, //6
    TVBOX_7       = 15, //7
    TVBOX_8       = 17, //8
    TVBOX_9       = 19, //9
    TVBOX_Lead    = 21, //����
    TVBOX_0       = 23, //0
    TVBOX_Back    = 25, //����
    TVBOX_Up      = 27, //��
    TVBOX_Left    = 29, //��
    TVBOX_Comfirm = 31, //ȷ��
    TVBOX_Right   = 33, //��
    TVBOX_Down    = 35, //��
    TVBOX_VolAdd  = 37, //������
    TVBOX_VolRed  = 39, //������
    TVBOX_ChAdd   = 41, //Ƶ����
    TVBOX_ChRed   = 43, //Ƶ����
    TVBOX_Menu    = 45  //�˵�
    
}TVBoxKeyBoadMap;

typedef enum
{
    IPTV_Power  = 1,  //��Դ
    IPTV_Mute   = 3,  //����
    IPTV_VolAdd = 5,  //������
    IPTV_VolRed = 7,  //������
    IPTV_ChAdd  = 9,  //Ƶ����
    IPTV_ChRed  = 11, //Ƶ��-
    IPTV_Up     = 13, //��
    IPTV_Left   = 15, //��
    IPTV_OK     = 17, //OK
    IPTV_Right  = 19, //��
    IPTV_Down   = 21, //��
    IPTV_Play   = 23, //���ţ���ͣ
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
    IPTV_BACK   = 45  //����
    
}IPTVKeyBoadMap;

typedef enum
{
    TV_VolAdd  = 1,   //������
    TV_ChAdd   = 3,   //Ƶ����
    TV_Menu    = 5,   //�˵�
    TV_ChRed   = 7,   //Ƶ����
    TV_VolRed  = 9,   //������
    TV_Power   = 11,  //��Դ
    TV_Mute    = 13,  //����
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
    TV_Back    = 39,  //����
    TV_Comfirm = 41,  //ȷ��
    TV_Up      = 43,  //��
    TV_Left    = 45,  //��
    TV_Right   = 47,  //��
    TV_Down    = 49,  //��
    
}TVKeyBoadMap;

typedef enum
{
    DVD_Left    = 1,   //��
    DVD_Up      = 3,   //��
    DVD_Ok      = 5,   //ok
    DVD_Down    = 7,   //��
    DVD_Right   = 9,   //��
    DVD_Power   = 11,  //��Դ
    DVD_Mute    = 13,  //����
    DVD_FBack   = 15,  //�쵹
    DVD_Play    = 17,  //����
    DVD_FForwad = 19,  //���
    DVD_Prev    = 21,  //��һ��
    DVD_Stop    = 23,  //ֹͣ
    DVD_Next    = 25,  //��һ��
    DVD_Format  = 27,  //��ʽ
    DVD_Pause   = 29,  //��ͣ
    DVD_Title   = 31,  //����
    DVD_SK      = 33,  //���ز�
    DVD_Menu    = 35,  //����
    DVD_Back    = 37   //����
    
}DVDKeyBoadMap;

typedef enum
{
    ARC_Power  = 0x01,  //��Դ
    ARC_Mode   = 0x02,  //ģʽ
    ARC_Vol    = 0x03,  //����
    ARC_M      = 0x04,  //�ֶ�����
    ARC_A      = 0x05,  //�Զ�����
    ARC_tmpAdd = 0x06,  //�¶ȣ�
    ARC_tmpRed = 0x07,  //�¶ȣ�
    
}ARCKeyBoadMap;

typedef enum
{
    WH_Power   = 1,  //��Դ
    WH_Set     = 3,  //����
    WH_TemAdd  = 5,  //�¶ȣ�
    WH_TemRed  = 7,  //�¶ȣ�
    WH_Mode    = 9,  //ģʽ
    WH_Confirm = 11, //ȷ��
    WH_Timer   = 13, //��ʱ
    WH_Ant     = 15, //ԤԼ
    WH_Time    = 17, //ʱ��
    WH_Stem    = 19, //����
    
}WheaterKeyBoadMap;

typedef enum
{
    AIR_Power   = 1,  //��Դ
    AIR_Auto    = 3,  //�Զ�
    AIR_AirVol  = 5,  //����
    AIR_Timer   = 7,  //��ʱ
    AIR_Mode    = 9,  //ģʽ
    AIR_Anion   = 11, //������
    AIR_Comfort = 13, //����
    AIR_Mute    = 15,  //����
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
    SLR_TPIC    = 1,  //����
    
}SLRKeyBoadMap;

#endif /* KEYBOARD_MAP_H */
