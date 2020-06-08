/*
@file YoseenTypes.h
@author yoseen
@date 2018-04-01
*/
#ifndef YOSEENTYPES_H_
#define YOSEENTYPES_H_

#include "YoseenBasicTypes.h"

/*
������
*/
enum YoseenErrorType{
	YET_None = 0,					///< ����
	YET_Undefined = -1,				///< δ����
	YET_NotImplemented = -2,			///< δʵ��
	YET_NotSupported = -3,			///< ��֧��
	YET_InvalidState = -4,			///< ��Ч״̬
	YET_InvalidArgs = -5,			///< ��Ч����

	//
	YET_SDKUninited = -10,			///< δ��ʼ��
	YET_InvalidHandle = -11,			///< ��Ч���
	YET_NoMemory = -12,			///< ���ڴ����
	YET_NoHandle = -13,			///< �޾������

	YET_PreviewOpenBuffer = -14,		///< Ԥ���򿪻�����ʧ��
	YET_PreviewOpenInput = -15,		///< Ԥ��������ʧ��
	YET_PreviewOpenDisplay = -16,		///< Ԥ������ʾʧ��
	YET_PreviewRecoverBegin = -17,		///< Ԥ���Զ��ָ���ʼ
	YET_PreviewRecoverEnd = -18,			///< Ԥ���Զ��ָ�����

	//
	YET_SocketOpen = -100,					///< Socket��ʧ��
	YET_SocketConn = -101,					///< Socket����ʧ��
	YET_SocketSend = -102,					///< Socket����ʧ��				
	YET_SocketRecv = -103,					///< Socket����ʧ��
	YET_SocketData = -104,					///< Socket�����д�

	//
	YET_FileOpen = -201,						///< �ļ���ʧ��
	YET_FileWrite = -202,					///< �ļ�дʧ��
	YET_FileRead = -203,						///< �ļ���ʧ��
	YET_FileType = -204,						///< �ļ���������
	YET_FileData = -205,						///< �ļ���������

	//
	YET_FfmpegDecode = -301,				///< ffmpeg����ʧ��
	YET_FfmpegMuxOpen = -302,			///< ffmpeg muxer��ʧ��
	YET_FfmpegMuxWrite = -303,			///< ffmpeg muxerд��ʧ��
	YET_FfmpegMuxClose = -304,			///< ffmpeg muxer�ر�ʧ��
};

/*
�豸������Ϣ
*/
enum TransformType{
	TransformType_None = 0,
	TransformType_Rotate90,
	TransformType_Rotate180,
	TransformType_Rotate270,

	//
	TransformType_MirrorX,
	TransformType_MirrorY,
};

typedef struct _CameraBasicInfo{
	char CameraId[32];				///< ���к�
	char CameraName[32];			///< ����
	char CameraType[16];			///< ����

	char FPAId[32];					///< ̽�������к�

	char HardwareId[32];			///< Ӳ���汾
	char SoftwareId[32];			///< ����汾

	u16 DataWidth;			///< ���ݿ��
	u16 DataHeight;			///< ���ݸ߶�

	/**����֡��
	�����¶�����֡��=DataFps/(DataRatio*SendRatio)
	������Ƶ����֡��=DataFps/DataRatio
	ģ����Ƶ֡��=DataFps/DataRatio
	*/
	u16 DataFps;

	u16 DataRatio;			///< ������, �豸ÿ������֡ȡһ֡����
	u16 SendRatio;			///< ������, �豸ÿ������֡��һ֡����

	u8 DataTransform;		///< ���ݱ任����
	u8 pad[25];

	/*
	Ϊ��֤��Ƶ�ϻ��Ƶ����֡����¶��󡢵�ɫ������,
	�¶����ݷֱ��ʵ���640x480��ͼ��Ŵ�640x480, ���ڴ˻����ϻ���, H264����, JPEG����.
	*/
	u16 VideoWidth;			///< ��Ƶ������
	u16 VideoHeight;		///< ��Ƶ����߶�
}CameraBasicInfo;

/**
�豸������Ϣ
*/
typedef struct _CameraNetworkInfo{
	bool UseStaticIp;				///< �Ƿ�ʹ�þ�̬IP
	u8 pad;
	u16 MulticastPort;				///< �鲥�˿�, δʹ��

	u32 StaticIp;					///< ��̬IP
	u32 SubnetMask;					///< ��������
	u32 Gateway;					///< ����

	u32 MulticastIp;				///< �鲥IP, δʹ��

	u8 MacAddr[6];					///< MAC��ַ

	u8 pad2[2];
	u32 Dns;						///< ��DNS
	u32 Dns2;						///< ��DNS
	u8 reserved[16];
}CameraNetworkInfo;

/**
����У����Ϣ
*/
typedef struct _FFCInfo{
	u8 frames_skipped_after_close;			///< ����رպ����֡��
	u8 frames_accumlated_when_closed;		///< ����رպ��ۻ�֡��
	u8 frames_skipped_after_open;			///< ����򿪺����֡��
	u8 pad;
	u16 time_interval;						///< ʱ�䴥�����, ��λ֡��
	u16 temp_interval;						///< �¶ȴ������, ��λmK

	s8 af_rebound;							///< �Զ��Խ��ص�, ��λ֡��
	u8 reserved[7];
}FFCInfo;

/**
����������Ϣ
*/
typedef struct _FixInfo{
	float AtmosphericTemperature;		///< ����
	float RelativeHumidity;				///< ���ʪ��
	float Visibility;					///< �ܼ���
	float RainfallIntensity;			///< ����ǿ��
	float SnowfallIntensity;			///< ��ѩǿ��
	float TargetDistance;				///< Ŀ�����

	float GlobalEmissivity;				///< ȫ�ַ�����
	float InfraredWindowTrans;			///< ���ⴰ��͸����
	float TempOffset;					///< �¶�Ư��

	bool EnableAtmFix;					///< �Ƿ񿪴�������
	u8 reserved[35];
}FixInfo;

/**
���¶�����Ϣ
*/
typedef struct _MeasureInfo{
	s32					MOC;		///< ���¶�������
	xxxmeasure_object	MOS[8];		///< ���¶�������
}MeasureInfo;

/**
ģ����Ƶ��Ϣ
*/
enum XuiDisplayFlags{
	XDF_Palette = 0x0001,				///< ��ʾ��ɫ��
	XDF_TrackHigh = 0x0002,				///< ��ʾ����׷��
	XDF_TrackLow = 0x0004,				///< ��ʾ����׷��

	XDF_NoGlobalMax = 0x0008,			///< ����ʾȫ�������
	XDF_NoGlobalMin = 0x0010,			///< ����ʾȫ�������
	XDF_NoLocalMeasures = 0x0020,		///< ����ʾ�ֲ����¶���
};

typedef struct _TvoutInfo{
	bool				EnableTvout;		///< �Ƿ���ģ����Ƶ
	u8					PaletteType;		///< ��ɫ������
	u8					pad;
	u8					Contrast;			///< �Աȶ�
	u8					Brightness;			///< ����
	u8					Zoom;				///< ���ֱ佹
	u16					XuiDisplayFlags;	///< ��ʾ��־λ, �μ�XuiDisplayFlags

	float				Gain;				///< ����
	u32					H264_Bitrate;		///< ��ý�������
	u16					H264_GopSize;		///< ��ý��ͼ�����С

	//add20180809
	u8					StrechType;			///< �㷨����
	u8					DDELevel;			///< DDE�ȼ�
	s16					LinearTemp[2];		///< �����¶�, 0.1C
	u8					LinearGray[2];		///< ���ԻҶ�
	u8					reserved[14];
}TvoutInfo;

/**
������Ϣ
*/
typedef struct _SerialPortInfo{
	u8 Usage;					///< �÷�
	u8 PortAddr;				///< ���ڵ�ַ
	u8 BaudRate;				///< ������
	u8 DataBits;				///< ����λ
	u8 StopBits;				///< ֹͣλ
	u8 Parity;					///< У��λ
	u8 reserved[10];
}SerialPortInfo;

/**
GPIO��Ϣ
*/
typedef struct _GpioInfo{
	u8 Input0;		///< ����0
	u8 Input1;		///< ����1
	u8 Output0;		///< ���0
	u8 Output1;		///< ���0
	u8 reserved[12];
}GpioInfo;

/**
�豸ʱ����Ϣ
*/
typedef struct _CameraTimeInfo{
	char	NtpServer[32];			///< ��NTP������
	char	NtpServer2[32];			///< ��NTP������
	char	TimeZone[32];			///< ʱ��
	u8		reserved[32];
}CameraTimeInfo;

/*
�豸OSD��Ϣ
*/
typedef struct _CameraOSDInfo{
	u16 X1;				///< ����X1
	u16 Y1;				///< ����Y1
	char Text1[64];		///< �ı�1, utf-8����

	u16 X2;				///< ����X2
	u16 Y2;				///< ����Y2
	char Text2[64];		///< �ı�2

	u16 X3;				///< ����X3
	u16 Y3;				///< ����Y3
	char Text3[64];		///< �ı�3

	u16 X4;				///< ����X4
	u16 Y4;				///< ����Y4
	char Text4[64];		///< �ı�4
}CameraOSDInfo;


/*
�¶�֡ͷ
*/
typedef struct _DataFrameHeader{
	u16 Width;						///< ���
	u16 Height;						///< �߶�
	u32 ComSize;					///< ѹ����С
	u8 DataType;					///< ��������
	u8 ComType;						///< ѹ������
	u16 Index;						///< ֡����

	/**
	ȫ֡�¶���16λ�з������������ʾ, �¶ȸ���ֵ=�¶�����ֵ/Slope+Offset
	*/
	u16 Slope;
	s16 Offset;

	s32 FPATemp;					///< ̽�����¶�, �ڲ�ʹ��
	s32 ShellTemp;					///< ��Ч�����¶�, �ڲ�ʹ��

	u8 pad;
	u8 GpioInput0;					///< GPIO����0
	u8 GpioInput1;					///< GPIO����1
	u8 pad2[5];
	s64 Timestamp;					///< ʱ���, ��λ100ns
	u8 reserved[88];
}DataFrameHeader;
#define DATA_FRAME_HEADER_SIZE					128
#define DataFrameHeader_Offset_Timestamp				32

/**
��������
*/
enum CtlType{
	CtlType_Debug = 0,				///< 
	CtlType_ChangeDataType,			///< 
	CtlType_MoveShutter,				///< 
	CtlType_StoreFactory,				///< 
	CtlType_RestartCamera,			///< �����豸


	CtlType_ManualFFC = 32,			///< �ֶ�����У��
	CtlType_RestoreFactory,			///< �ָ���������
	CtlType_FocusManual,			///< �ֶ��Խ�, ����ģʽ���Ƶ綯��ͷ���
	CtlType_FocusAuto,				///< �Զ��Խ�
	CtlType_SetAlarm,				///< ����GPIO����
	CtlType_DisableFFC,				///< ���õ���У��
	CtlType_FocusManual2,			///< �ֶ��Խ�, ��ͣģʽ���Ƶ綯��ͷ���
};

/**
����
*/
typedef struct _Ctl{
	u16 Type;					///< ����
	union {
		u8 reserved[8];				///< �������С
		u8 DataType;				///< �¶���������
		u8 ShutterState;			///< 0���忪, 1�����
		s16 FocusDelta;				///< ����ģʽ, �綯��ͷ���ת��ʱ��, ��λ5ms, ����-Զ��, ����-����
		xxxfocusrect FocusRect;		///< �Զ��Խ�����
		u8	FocusType;				///< ��ͣģʽ, �綯��ͷ���ת����ʽ, 0 ֹͣ, 1 Զ��ת��, 2 ����ת��
		xxxgpioalarm Alarm;			///< GPIO����
		u8 DisableFFC;				///< 1����, 0����
	}Data;
}Ctl;

/**
����X����
*/
enum CtlXType{
	CtlXType_GetTime = 0,	///< ��ȡ�豸ʱ��, UTC
	CtlXType_SetTime,		///< �����豸��ʱ��, UTC
	CtlXType_GetGear,		///< ��ȡ���µ�λ
	CtlXType_SetGear,		///< ���ò��µ�λ
	CtlXType_AutoFocus,		///< �Զ��Խ�

	//add20180921
	CtlXType_XfGetConfig,	///< λ�ü���綯��ͷ, ��ȡ����
	CtlXType_XfSetConfig,	///< λ�ü���綯��ͷ, ��������, �ڲ�ʹ��
	CtlXType_XfGetCur,		///< λ�ü���綯��ͷ, ��ȡλ��
	CtlXType_XfMove,		///< λ�ü���綯��ͷ, ����λ��
	CtlXType_XfReset,		///< λ�ü���綯��ͷ, ����λ��
	CtlXType_XfCheckDelta,	///< λ�ü���綯��ͷ, ������
};

/**
���Ʋ���
*/
typedef struct _CtlX{
	s32 Type;						///< ����
	union {
		u8				Reserved[64];
		s32				Error;			///< ������
		xxxdatetime		Time;			///< �豸ʱ��
		s32				GearLevel;		///< ���µ�λ
		xxxfocusrect	FocusRect;		///< �Զ��Խ�����

		xf_config	XfConfig;			///< λ�ü���綯��ͷ, ����
		s32			XfMove;				///< λ�ü���綯��ͷ, λ��-�̶�ֵ, [0-1000]
		s32			XfCurType;			///< λ�ü���綯��ͷ, 0-����ֵ, 1-�̶�ֵ
	}Data;
}CtlX;

/**
������������Ӧ2
*/
typedef struct _DiscoverCameraResp2{
	CameraBasicInfo BasicInfo;				///< ������Ϣ
	u32 CameraIp;							///< �豸IP
	struct _DiscoverCameraResp2* pNext;		///< ��һ����Ӧ
}DiscoverCameraResp2;


/*
������Ϣ
*/
typedef struct _YoseenLoginInfo{
	char CameraAddr[128];			///< �豸��ַ
	char Username[32];				///< �û���
	char Password[32];				///< ����
}YoseenLoginInfo;

/*
H264��ʼ֡
*/
typedef struct _H264FrameStart{
	u32 EncDataSize;				///< ���������ݴ�С
	u16 Width;						///< ���ݿ��
	u16 Height;						///< ���ݸ߶�
	u8 EncData[120];				///< ����������

	u16 FpsNum;						///< ֡�ʷ���
	u16 FpsDen;						///< ֡�ʷ�ĸ
	u8 Reserved[124];
}H264FrameStart;
#define H264FrameStart_Size				256

/*
H264֡���½��
*/
typedef struct _H264_MeasureResult{
	s8	Index;				//���; 0:ȫ�ֲ��½��; 1:���Ĳ��½��; [2-9]:�ֲ����½��; x��Ч���½��
	s8	Pad[3];				//

	float Max;				//�����
	float Min;				//�����
	float Avg;				//ƽ����

	u16 MaxX;				//����µ�����X
	u16 MaxY;				//����µ�����Y
	u16 MinX;				//����µ�����X
	u16 MinY;				//����µ�����Y
}H264_MeasureResult;

/*
H264֡ͷ
*/
typedef struct _H264FrameHeader{
	s32 Size;						//֡��С
	u16 Width;						//���ݿ��
	u16 Height;						//���ݸ߶�

	float FPATemp;					//̽�����¶�

	H264_MeasureResult	GlobalResult;			//ȫ�ֲ��½��
	H264_MeasureResult	CenterResult;			//���Ĳ��½��
	H264_MeasureResult LocalResults[8];			//�ֲ����½��

	u8 Reserved[772];
}H264FrameHeader;
#define H264FrameHeader_Size			1024

/*
����֡
*/
typedef struct _DataFrame{
	void* Head;					///< �¶�����֡ͷ
	void* Temp;					///< �¶�����
	void* Com;					///< �¶�����ѹ��
	void* Bmp;					///< λͼ����
	void* H264;					///< H264֡ͷ
	struct _DataFrame* Prev;	///< ǰһ֡
}DataFrame;

/*
Ԥ���ص�
@param errorCode ������
@param dataFrame ����֡
@param customData �û�����
*/
typedef void(__stdcall *YoseenPreviewCallback)(s32 errorCode, DataFrame* dataFrame, void* customData);

/*
Ԥ����Ϣ
*/
typedef struct _YoseenPreviewInfo{
	s32 DataType;							///< xxxdatatype_video ��Ƶ��, xxxdatatype_temp �¶���
	void* Hwnd;									///< ��ʾ���ھ��, ����Ϊ��
	YoseenPreviewCallback CustomCallback;		///< Ԥ���ص�
	void* CustomData;							///< �û�����

	/*
	�û���������, Ԥ���ɹ���, �û��ݴ˵�����ʾ���ڿ�߱�
	*/
	u16 OutputWidth;							///< ��ʾ������
	u16 OutputHeight;							///< ��ʾ����߶�
}YoseenPreviewInfo;

typedef struct _YoseenRtspInfo{
	char Url[256];

	u16 CropX;
	u16 CropY;
	u16 CropWidth;
	u16 CropHeight;

	//
	u16 DataWidth;
	u16 DataHeight;
}YoseenRtspInfo;

/**
�¶�תλͼ�㷨�����޸ı�־
*/
enum StrechControlFlags{
	SCF_StrechType = 0x0001,	///< �㷨����
	SCF_Contrast = 0x0002,		///< �Աȶ�
	SCF_Brightness = 0x0004,	///< ����
	SCF_PHEGain = 0x0008,		///< PHE����
	SCF_Linear = 0x0010,		///< LINEAR����
	SCF_PHEStream = 0x0020,		///< PHE����
	SCF_DDELevel = 0x0040,		///< DDE�ȼ�
};

/**
�¶�תλͼ�㷨����
*/
enum StrechType{
	StrechType_PHE = 1,			///< PHE
	StrechType_LINEAR,			///< LINEAR
};

/**
�¶�תλͼ�㷨����
*/
typedef struct _strech_control{
	s32 flags;						///< ��־

	u8 strech_type;					///< �㷨����
	u8 contrast;					///< �Աȶ�
	u8 brightness;					///< ����
	u8 phe_stream;					///< PHE����
	float phe_gain;					///< PHE����

	s16	linear_temp[2];				///< �ֶ������¶�, ��λ0.1C
	u8	linear_gray[2];				///< �ֶ����ԻҶ�

	u8	dde_level;					///< DDE�ȼ�0-8, 0�ر�DDE
	u8 reserved[5];
}strech_control;

#endif