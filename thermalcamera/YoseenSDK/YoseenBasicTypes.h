/*
@file YoseenBasicTypes.h
@author yoseen
@date 2018-04-01
*/
#ifndef YOSEENBASICTYPES_H_
#define YOSEENBASICTYPES_H_

/**
基本类型
*/
typedef signed char s8;
typedef unsigned char u8;

typedef signed short s16;
typedef unsigned short u16;

typedef signed int s32;
typedef unsigned int u32;

typedef signed long long s64;
typedef unsigned long long u64;

#define S8_MIN			(-128)
#define S8_MAX			127
#define S16_MIN			(-32768)
#define S16_MAX			32767
#define S16_RANGE		(65536)
#define U16_MAX			(65535)

#define S32_MIN			(-2147483647 - 1)
#define U32_MAX			0xffffffff

typedef struct _bgra {
	u8 b;
	u8 g;
	u8 r;
	u8 a;
}bgra;

/**
*/
#ifdef SDK_EXPORT
#define SDK_API __declspec(dllexport)
#else
#define SDK_API __declspec(dllimport)
#endif

#define _CRT_SECURE_NO_WARNINGS
#define ARRAY_SIZE(x)			(sizeof(x)/sizeof(x[0]))
#define MIN(a,b)				((a)<(b)?(a):(b))
#define MAX(a,b)				((a)>(b)?(a):(b))
#define ABS(a)					((a)>0?(a):-(a))
#define XMALLOC(size,alignment)	_aligned_malloc(size,alignment)
#define XFREE(x)				_aligned_free(x)

/*
数据类型
*/
enum xxxdatatype {
	xxxdatatype_temp = 2,		///< 温度流
	xxxdatatype_video = 3,		///< 视频流
	xxxdatatype_rtsp = 4,		///< rtsp流
};

/*
数据文件类型
*/
enum xxxmediafile {
	xxxmediafile_bmpx = 0,			///< 单帧温度文件, bmp扩展
	xxxmediafile_pngx,				///< 单帧温度文件, png扩展
	xxxmediafile_stream,			///< 多帧温度文件
	xxxmediafile_mp4,				///< mp4文件
	xxxmediafile_jpgx,				///< 单帧温度文件, jpg扩展
};

/**
设备文件类型
*/
enum xxxcamerafile {
	xxxcamerafile_bin = 0,			///< 设备程序包
	xxxcamerafile_cali,				///< 校准表
	xxxcamerafile_com,				///<
	xxxcamerafile_log,				///< 日志

	xxxcamerafile_cali2,			///< 校准表2
	xxxcamerafile_cali3,			///< 校准表3

	xxxcamerafile_bad,				///< 坏点表

	xxxcamerafile_c2m,				///< 二次多段校准
	xxxcamerafile_c2m2,				///< 二次多段校准2

	xxxcamerafile_cust,				///< 用户校准表
	xxxcamerafile_cust2,			///< 用户校准表2
};

/**
调色板类型
*/
enum xxxpalette {
	xxxpalette_Autumn = 0,
	xxxpalette_BlackHot,
	xxxpalette_GlowBow,
	xxxpalette_HighContrast,
	xxxpalette_HotMetal,
	xxxpalette_IronBow,
	xxxpalette_Jet,
	xxxpalette_RainBow,
	xxxpalette_RedSaturation,
	xxxpalette_WhiteHot,
	xxxpalette_Winter,
	xxxpalette_IronBow2,
};

/**
红外调色板
*/
typedef struct _xxxpalettedata {
	char name[32];				///< 名称
	bgra data[256];				///< 数据
}xxxpalettedata;

/**
发现热像仪方式
*/
enum xxxdiscoverflags {
	xxxdiscoverflags_broadcast = 0x0001,		///< 广播
	xxxdiscoverflags_multicast = 0x0002,		///< 组播
	xxxdiscoverflags_broadcast2 = 0x0004,	///< 广播2
};

/**
测温结果
*/
typedef struct _xxxmeasure_result {
	float amin;			///< 最小值
	float amax;			///< 最大值
	float aavg;			///< 平均值
	s16 min;			///< 最小值, 整数
	s16 max;			///< 最大值, 整数
	s16 avg;			///< 平均值, 整数

	u16 max_x;			///< 最大值坐标x
	u16 max_y;			///< 最大值坐标y

	u16 min_x;			///< 最小值坐标x
	u16 min_y;			///< 最小值坐标y
}xxxmeasure_result;

/**
点
*/
typedef struct _xxxpoint {
	u16 x;			///< 坐标x
	u16 y;			///< 坐标y
	u16 radius;		///< 半径, 以中心(x,y), 边长(radius*2+1)的方块代表点
}xxxpoint;

/**
线
*/
typedef struct _xxxline {
	u16 x0;			///< 起点x
	u16 y0;			///< 起点y
	u16 x1;			///< 终点x
	u16 y1;			///< 终点y
	u16 radius;		///< 线宽, 未使用
}xxxline;

/**
矩形
*/
typedef struct _xxxrectangle {
	u16 xmin;			///< x最小
	u16 ymin;			///< y最小
	u16 xmax;			///< x最大
	u16 ymax;			///< y最大
}xxxrectangle;

/**
测温对象类型
*/
enum xxxmeasure_type {
	xxxmeasure_type_point = 0,			///< 点
	xxxmeasure_type_line = 1,			///< 线
	xxxmeasure_type_rectangle = 2,		///< 矩形
	xxxmeasure_type_ellipse = 3,			///< 椭圆
};

/**
报警类型
*/
enum xxxalarm_type {
	xxxalarm_type_none = 0,				///< 无
	xxxalarm_type_max,					///< 最大值
	xxxalarm_type_min,					///< 最小值
	xxxalarm_type_mid					///<
};

/**
测温对象
*/
typedef struct _xxxmeasure_object {
	char measure_id[16];				///< 名称
	u8 measure_type;					///< 类型
	u8 alarm_type;						///< 报警类型
	u8 pad[2];
	float alarm_min;				///< 报警最小值
	float alarm_max;				///< 报警最大值
	float emissivity;				///< 发射率

	union {
		xxxpoint point;			///< 点
		xxxline line;				///< 线
		xxxrectangle rectangle;	///< 矩形
	}geometry;							///< 几何对象

	u8 reserved[14];
}xxxmeasure_object;

/**
串口数据位
*/
enum xxxspdatabits {
	xxxspdatabits_5 = 5,
	xxxspdatabits_6,
	xxxspdatabits_7,
	xxxspdatabits_8,
};

/**
串口校验位
*/
enum xxxspparity {
	xxxspparity_none = 0,
	xxxspparity_odd,
	xxxspparity_even,
};

/**
串口停止位
*/
enum xxxspstopbits {
	xxxspstopbits_1 = 0,
	xxxspstopbits_2,
};

/**
串口波特率
*/
enum xxxspbaudrate {
	xxxspbaudrate_2400 = 0,
	xxxspbaudrate_4800,
	xxxspbaudrate_9600,

	xxxspbaudrate_19200,
	xxxspbaudrate_38400,
	xxxspbaudrate_57600,
	xxxspbaudrate_115200,
};

/**
串口用法
*/
enum xxxspusage {
	xxxspusage_custom = 0,
	xxxspusage_pelcod,
	xxxspusage_pelcop
};

/**
GPIO输入类型
*/
enum xxxgpioinputtype {
	xxxgpioinputtype_none = 0,			///< 关闭
	xxxgpioinputtype_check,				///< 检查
	xxxgpioinputtype_capture,			///< 截图
};

/**
GPIO输出类型
*/
enum xxxgpiooutputtype {
	xxxgpiooutputtype_close = 0,		///< 关闭
	xxxgpiooutputtype_open,				///< 打开
};

/**
对焦区域
*/
typedef struct _xxxfocusrect {
	u16 xmin;		///< x最小
	u16 ymin;		///< y最小
	u16 xmax;		///< x最大
	u16 ymax;		///< y最大
}xxxfocusrect;

/**
GPIO报警类型
*/
enum xxxgpioalarmtype {
	xxxgpioalarmtype_manual = 0,		///< 手动
	xxxgpioalarmtype_auto				///< 自动
};

/**
GPIO报警值
*/
enum xxxgpioalarmvalue {
	xxxgpioalarmvalue_low = 0,			///< 低
	xxxgpioalarmvalue_high,				///< 高
	xxxgpioalarmvalue_ignore,			///< 无
};

/**
GPIO报警
*/
typedef struct _xxxgpioalarm {
	u8 value0;			///< 报警值0
	u8 type0;			///< 报警类型0
	u8 time0;			///< 报警时间0

	u8 value1;			///< 报警值1
	u8 type1;			///< 报警类型1
	u8 time1;			///< 报警时间1
}xxxgpioalarm;

/**
串口数据
*/
typedef struct _xxxserialportdata {
	u8 size;				///< 数据大小
	u8 data[31];			///< 数据区
}xxxserialportdata;

/**
时间
*/
typedef struct _xxxdatetime {
	s32 year;			///< 年
	s32 mon;			///< 月
	s32 day;			///< 日

	s32 hour;			///< 时
	s32 min;			///< 分
	s32 sec;			///< 秒
}xxxdatetime;

typedef struct _xf_config {
	short ms_full;//ms
	short fc_full;//fc
	short fc1;//fc, cc0
	short fc2;//fc, cc1000

	short fc_delta;//fc
	short fc_small;//fc, 300
	u8	pad[4];
}xf_config;

#endif
