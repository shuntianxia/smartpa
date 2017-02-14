#ifndef _DEV_INFO_H
#define _DEV_INFO_H

#include "keyboard_map.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

typedef enum {
	DEV_TYPE_AIR_CLEANER,
	DEV_TYPE_AIR_CONDITION,
	DEV_TYPE_WATER_HEATER,
	DEV_TYPE_FAN,
	DEV_TYPE_PROJECTOR,
	DEV_TYPE_DVD,
	DEV_TYPE_TV,
	DEV_TYPE_TVBOX,
	DEV_TYPE_IPTV,
	//DEV_TYPE_SLR
}dev_type_t;

typedef enum {
	DEV_MFR_NULL,
	DEV_MFR_GREE,
	DEV_MFR_MIDEA,
	DEV_MFR_HISENSE,
	DEV_MFR_HAIER,
	DEV_MFR_CHIGO
}dev_mfr_t;

typedef enum {
	MATCH_CODE_AUTO,
	MATCH_CODE_MANUAL,
	MATCH_CODE_MODEL
}match_code_mode_t;

typedef struct {
	dev_type_t	type;
	char dev_brand[32];
	char dev_model[32];
	char ir_code[512];
	int code_len;
	int match_code_key;
	match_code_mode_t match_code;
}dev_info_t;

typedef enum {
	WIND_VOLUME_AUTO = 1,
	WIND_VOLUME_LOW = 2,
	WIND_VOLUME_MEDIUM = 3,
	WIND_VOLUME_HIGH = 4
}wind_volume_t;

typedef enum {
	WIND_DIR_UP = 1,
	WIND_DIR_MIDDLE = 2,
	WIND_DIR_DOWN = 3
}wind_dir_t;

typedef enum {
	AUTO_WIND_DIR_OFF = 0,
	AUTO_WIND_DIR_ON = 1
}auto_wind_dir_t;

typedef enum {
	POWER_OFF = 0,
	POWER_ON = 1
}power_status_t;

typedef enum {
	MODE_Automatic = 0,
	MODE_Refngeration = 2,
	MODE_Dehumidification = 3,
	MODE_Air_supply = 4,
	MODE_Heating = 5
}arc_mode_t;

typedef struct arc_info {
	int temp;
	wind_volume_t wind_volume;
	wind_dir_t wind_dir;
	auto_wind_dir_t auto_wind_dir;
	power_status_t power_status;
	ARCKeyBoadMap key_code;
	arc_mode_t arc_mode;
}arc_info_t;

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _DEV_INFO_H */

