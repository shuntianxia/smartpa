#ifndef IRDB_MANAGER_H
#define IRDB_MANAGER_H
#include <stdarg.h>     /* Needed for the definition of va_list */

/*
** Make sure we can call this stuff from C++.
*/
#ifdef __cplusplus
extern "C" {
#endif

//表名
extern char * const kPJTTableName;     //投影仪表名
extern char * const kFanTableName;     //风扇表名
extern char * const kTVBoxTableName;   //机顶盒表名
extern char * const kTVTableName;      //电视表名
extern char * const kIPTVTableName;    //iptv表名
extern char * const kDVDTableName;     //dvd表名
extern char * const kARCTableName;     //空调表名
extern char * const kHWaterTableName;  //热水器表名
extern char * const kAirTableName;     //空气净化器表名
extern char * const kSLRTableName;     //单反表名

//字段名
extern char * const fSerialFeild;      //索引序号字段（0开始）
extern char * const fBrandCNFeild;     //中文品牌字段
extern char * const fBrandENFeild;     //英文品牌字段
extern char * const fModelFeild;       //型号字段
extern char * const fPinYinFeild;      //中文名拼音字段
extern char * const fCodeFeild;        //码值字段

#if 0
- (NSMutableArray *)getTheWholeDeviceList:(DeviceType)type;

//品牌列表
- (NSMutableArray *)getAllBrandByDeviceType:(DeviceType)type;

//某品牌下所有型号(码库不一定有此品牌的具体型号)
- (NSMutableArray *)getAllModelByBrand:(char *)brand DeviceType:(DeviceType)type;
#endif

#ifdef __cplusplus
}  /* end of the 'extern "C"' block */
#endif

#endif /* IRDB_MANAGER_H */
