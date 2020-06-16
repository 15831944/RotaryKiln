/*
@file YoseenBasicTypes.h
@author yoseen
@date 2018-04-01
*/
#ifndef YOSEENBASICTYPES_H_
#define YOSEENBASICTYPES_H_

/**
��������
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
��������
*/
enum xxxdatatype {
	xxxdatatype_temp = 2,		///< �¶���
	xxxdatatype_video = 3,		///< ��Ƶ��
	xxxdatatype_rtsp = 4,		///< rtsp��
};

/*
�����ļ�����
*/
enum xxxmediafile {
	xxxmediafile_bmpx = 0,			///< ��֡�¶��ļ�, bmp��չ
	xxxmediafile_pngx,				///< ��֡�¶��ļ�, png��չ
	xxxmediafile_stream,			///< ��֡�¶��ļ�
	xxxmediafile_mp4,				///< mp4�ļ�
	xxxmediafile_jpgx,				///< ��֡�¶��ļ�, jpg��չ
};

/**
�豸�ļ�����
*/
enum xxxcamerafile {
	xxxcamerafile_bin = 0,			///< �豸�����
	xxxcamerafile_cali,				///< У׼��
	xxxcamerafile_com,				///<
	xxxcamerafile_log,				///< ��־

	xxxcamerafile_cali2,			///< У׼��2
	xxxcamerafile_cali3,			///< У׼��3

	xxxcamerafile_bad,				///< �����

	xxxcamerafile_c2m,				///< ���ζ��У׼
	xxxcamerafile_c2m2,				///< ���ζ��У׼2

	xxxcamerafile_cust,				///< �û�У׼��
	xxxcamerafile_cust2,			///< �û�У׼��2
};

/**
��ɫ������
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
�����ɫ��
*/
typedef struct _xxxpalettedata {
	char name[32];				///< ����
	bgra data[256];				///< ����
}xxxpalettedata;

/**
���������Ƿ�ʽ
*/
enum xxxdiscoverflags {
	xxxdiscoverflags_broadcast = 0x0001,		///< �㲥
	xxxdiscoverflags_multicast = 0x0002,		///< �鲥
	xxxdiscoverflags_broadcast2 = 0x0004,	///< �㲥2
};

/**
���½��
*/
typedef struct _xxxmeasure_result {
	float amin;			///< ��Сֵ
	float amax;			///< ���ֵ
	float aavg;			///< ƽ��ֵ
	s16 min;			///< ��Сֵ, ����
	s16 max;			///< ���ֵ, ����
	s16 avg;			///< ƽ��ֵ, ����

	u16 max_x;			///< ���ֵ����x
	u16 max_y;			///< ���ֵ����y

	u16 min_x;			///< ��Сֵ����x
	u16 min_y;			///< ��Сֵ����y
}xxxmeasure_result;

/**
��
*/
typedef struct _xxxpoint {
	u16 x;			///< ����x
	u16 y;			///< ����y
	u16 radius;		///< �뾶, ������(x,y), �߳�(radius*2+1)�ķ�������
}xxxpoint;

/**
��
*/
typedef struct _xxxline {
	u16 x0;			///< ���x
	u16 y0;			///< ���y
	u16 x1;			///< �յ�x
	u16 y1;			///< �յ�y
	u16 radius;		///< �߿�, δʹ��
}xxxline;

/**
����
*/
typedef struct _xxxrectangle {
	u16 xmin;			///< x��С
	u16 ymin;			///< y��С
	u16 xmax;			///< x���
	u16 ymax;			///< y���
}xxxrectangle;

/**
���¶�������
*/
enum xxxmeasure_type {
	xxxmeasure_type_point = 0,			///< ��
	xxxmeasure_type_line = 1,			///< ��
	xxxmeasure_type_rectangle = 2,		///< ����
	xxxmeasure_type_ellipse = 3,			///< ��Բ
};

/**
��������
*/
enum xxxalarm_type {
	xxxalarm_type_none = 0,				///< ��
	xxxalarm_type_max,					///< ���ֵ
	xxxalarm_type_min,					///< ��Сֵ
	xxxalarm_type_mid					///<
};

/**
���¶���
*/
typedef struct _xxxmeasure_object {
	char measure_id[16];				///< ����
	u8 measure_type;					///< ����
	u8 alarm_type;						///< ��������
	u8 pad[2];
	float alarm_min;				///< ������Сֵ
	float alarm_max;				///< �������ֵ
	float emissivity;				///< ������

	union {
		xxxpoint point;			///< ��
		xxxline line;				///< ��
		xxxrectangle rectangle;	///< ����
	}geometry;							///< ���ζ���

	u8 reserved[14];
}xxxmeasure_object;

/**
��������λ
*/
enum xxxspdatabits {
	xxxspdatabits_5 = 5,
	xxxspdatabits_6,
	xxxspdatabits_7,
	xxxspdatabits_8,
};

/**
����У��λ
*/
enum xxxspparity {
	xxxspparity_none = 0,
	xxxspparity_odd,
	xxxspparity_even,
};

/**
����ֹͣλ
*/
enum xxxspstopbits {
	xxxspstopbits_1 = 0,
	xxxspstopbits_2,
};

/**
���ڲ�����
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
�����÷�
*/
enum xxxspusage {
	xxxspusage_custom = 0,
	xxxspusage_pelcod,
	xxxspusage_pelcop
};

/**
GPIO��������
*/
enum xxxgpioinputtype {
	xxxgpioinputtype_none = 0,			///< �ر�
	xxxgpioinputtype_check,				///< ���
	xxxgpioinputtype_capture,			///< ��ͼ
};

/**
GPIO�������
*/
enum xxxgpiooutputtype {
	xxxgpiooutputtype_close = 0,		///< �ر�
	xxxgpiooutputtype_open,				///< ��
};

/**
�Խ�����
*/
typedef struct _xxxfocusrect {
	u16 xmin;		///< x��С
	u16 ymin;		///< y��С
	u16 xmax;		///< x���
	u16 ymax;		///< y���
}xxxfocusrect;

/**
GPIO��������
*/
enum xxxgpioalarmtype {
	xxxgpioalarmtype_manual = 0,		///< �ֶ�
	xxxgpioalarmtype_auto				///< �Զ�
};

/**
GPIO����ֵ
*/
enum xxxgpioalarmvalue {
	xxxgpioalarmvalue_low = 0,			///< ��
	xxxgpioalarmvalue_high,				///< ��
	xxxgpioalarmvalue_ignore,			///< ��
};

/**
GPIO����
*/
typedef struct _xxxgpioalarm {
	u8 value0;			///< ����ֵ0
	u8 type0;			///< ��������0
	u8 time0;			///< ����ʱ��0

	u8 value1;			///< ����ֵ1
	u8 type1;			///< ��������1
	u8 time1;			///< ����ʱ��1
}xxxgpioalarm;

/**
��������
*/
typedef struct _xxxserialportdata {
	u8 size;				///< ���ݴ�С
	u8 data[31];			///< ������
}xxxserialportdata;

/**
ʱ��
*/
typedef struct _xxxdatetime {
	s32 year;			///< ��
	s32 mon;			///< ��
	s32 day;			///< ��

	s32 hour;			///< ʱ
	s32 min;			///< ��
	s32 sec;			///< ��
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
