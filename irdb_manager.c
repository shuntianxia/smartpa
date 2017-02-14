#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#include "irdb_manager.h"
#include "dev_info.h"

#define TBL_NAME_PJT	"PJT_TABLE";
#define TBL_NAME_FAN	"FAN_TABLE";
#define TBL_NAME_TVBOX	"TVBOX_TABLE";
#define TBL_NAME_TV		"TV_TABLE";
#define TBL_NAME_IPTV	"IPTV_TABLE";
#define TBL_NAME_DVD	"DVD_TABLE";
#define TBL_NAME_ARC	"ARC_TABLE";
#define TBL_NAME_WATER	"WATER_TABLE";
#define TBL_NAME_AIR	"AIR_TABLE";
#define TBL_NAME_SLR	"SLR_TABLE";

char * const fSerialFeild = "SERIAL";
char * const fBrandCNFeild = "BRAND_CN";
char * const fBrandENFeild = "BRAND_EN";
char * const fModelFeild = "MODEL";
char * const fPinYinFeild = "PINYIN";
char * const fCodeFeild = "CODE";

char* get_dev_tbl_name(dev_type_t dev_type)
{
	char *dev_tbl = NULL;
	
	switch(dev_type)
	{
		case DEV_TYPE_AIR_CLEANER:
			dev_tbl = TBL_NAME_AIR;
			break;

		case DEV_TYPE_AIR_CONDITION:
			dev_tbl = TBL_NAME_ARC;
			break;

		case DEV_TYPE_DVD:
			dev_tbl = TBL_NAME_DVD;
			break;

		case DEV_TYPE_FAN:
			dev_tbl = TBL_NAME_FAN;
			break;

		case DEV_TYPE_IPTV:
			dev_tbl = TBL_NAME_IPTV;
			break;

		case DEV_TYPE_PROJECTOR:
			dev_tbl = TBL_NAME_PJT;
			break;

		case DEV_TYPE_TV:
			dev_tbl = TBL_NAME_TV;
			break;

		case DEV_TYPE_TVBOX:
			dev_tbl = TBL_NAME_TVBOX;
			break;

		case DEV_TYPE_WATER_HEATER:
			dev_tbl = TBL_NAME_WATER;
			break;

		default:
			break;
	}
    return (char *)dev_tbl;
}

#if 0
- (NSMutableArray *)getTheWholeDeviceList:(DeviceType)type
{
    __block NSMutableArray * array = [[NSMutableArray alloc] initWithCapacity:0];
    
    char *tableName = [self deviceTypeConvert2TableName:type];
    if(![tableName length])return nil;
    
    char *sql = [char stringWithFormat:@"SELECT * FROM %@;",tableName];
    [self.dbQueue inDatabase:^(FMDatabase *db)
     {
         if (![db open])
         {
             [db close];
             return;
         }
         FMResultSet * rs= [db executeQuery:sql];
         while ([rs next])
         {
             DeviceM * device = [[DeviceM alloc] init];
             device.type = type;
             device.serial = [rs intForColumn:fSerialFeild];
             device.brandCN = [rs stringForColumn:fBrandCNFeild];
             device.brandEN = [rs stringForColumn:fBrandENFeild];
             device.model = [rs stringForColumn:fModelFeild];
             device.code = [rs dataForColumn:fCodeFeild];
             [array addObject:device];
         }
         [rs close];
     }];
    return array;
}

- (NSMutableArray *)getAllBrandByDeviceType:(DeviceType)type
{
    __block NSMutableArray * array = [[NSMutableArray alloc] initWithCapacity:0];
    
    char *tableName = [self deviceTypeConvert2TableName:type];
    if(![tableName length])return nil;
    
    char *sql = [char stringWithFormat:@"SELECT %@ FROM %@ GROUP BY %@;",fBrandCNFeild,tableName,fPinYinFeild];
    [self.dbQueue inDatabase:^(FMDatabase *db)
     {
         if (![db open])
         {
             [db close];
             return;
         }
         FMResultSet * rs= [db executeQuery:sql];
         while ([rs next])
         {
             [array addObject:[rs stringForColumn:fBrandCNFeild]];
         }
         [rs close];
     }];
    return array;
}

- (NSMutableArray *)getAllModelByBrand:(char *)brand DeviceType:(DeviceType)type
{
    char *tableName = [self deviceTypeConvert2TableName:type];
    if(!brand.length||!tableName.length)return nil;
    
    __block NSMutableArray * array = [[NSMutableArray alloc] initWithCapacity:0];
    char *sql = [char stringWithFormat:@"SELECT * FROM %@ WHERE BRAND_CN = ? AND MODEL != ? ",tableName];
    [self.dbQueue inDatabase:^(FMDatabase *db)
     {
         if (![db open])
         {
             [db close];
             return;
         }
         FMResultSet * rs= [db executeQuery:sql,brand,@""];
         while ([rs next])
         {
             DeviceM * device = [[DeviceM alloc] init];
             device.type = type;
             device.serial = [rs intForColumn:fSerialFeild];
             device.brandCN = [rs stringForColumn:fBrandCNFeild];
             device.brandEN = [rs stringForColumn:fBrandENFeild];
             device.model = [rs stringForColumn:fModelFeild];
             device.code = [rs dataForColumn:fCodeFeild];
             [array addObject:device];
         }
         [rs close];
     }];
    return array;
}
#endif
