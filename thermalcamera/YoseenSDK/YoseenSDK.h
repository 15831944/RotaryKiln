/*
@file YoseenSDK.h
@author yoseen
@date 2018-04-01
*/
#ifndef YOSEENSDK_H_
#define YOSEENSDK_H_

#include "YoseenTypes.h"
//#include "YoseenFile.h"

extern "C" {
	/*
	建立SDK
	@return 错误码
	*/
	SDK_API s32 Yoseen_InitSDK();

	/*
	释放SDK
	*/
	SDK_API void Yoseen_FreeSDK();

	/*
	登入热像仪, 成功登入会填充热像仪基本信息.
	目前只验证热像仪地址, 不验证用户名和密码; 热像仪不限制登入的用户.

	@param loginInfo 登录信息
	@param [out] cameraBasicInfo 热像仪基本信息
	@return >=0用户句柄, <0 错误码
	*/
	SDK_API s32 Yoseen_Login(const YoseenLoginInfo* loginInfo, CameraBasicInfo* cameraBasicInfo);

	/*
	登出热像仪
	@param userHandle 用户句柄
	@return 错误码
	*/
	SDK_API s32 Yoseen_Logout(s32 userHandle);

	/*
	保存单帧温度到文件(jpg扩展格式), 不需要预览的支持. 热像仪支持同一时刻处理16个保存请求,
	降低了在同一时刻请求过多时, 后面请求失败的几率.
	@param userHandle 用户句柄
	@param fileName 文件名
	@param withTemp 是否带温度扩展
	@return 错误码
	*/
	SDK_API s32 Yoseen_SaveFrame(s32 userHandle, const char* fileName, s32 withTemp);

	/*
	保存单帧温度数据到内存, 用户提供frameFile, SDK填充frameFile的各个数据指针.
	frameFile的各个数据指针指向的都是内部数据缓冲区, 对同一userHandle的保存请求会更新此内部数据缓冲区的内容.
	@param userHandle 用户句柄
	@param frameFile 文件数据指针结构体
	@return 错误码
	*/
	//SDK_API s32 Yoseen_SaveFrameToMem(s32 userHandle, TempFrameFile* frameFile);

	/*
	开始预览, 一台热像仪能同时支持 "1对16的视频流预览" , "1对1的温度流" 预览.

	视频流预览, 后端接收到的是H264编码的图像数据和特征温度数据, 带宽要求低.
	温度流预览, 后端接收到的是全帧温度数据, 带宽要求高.

	@param userHandle 用户句柄
	@param previewInfo 预览信息
	@return >=0 预览句柄, <0 错误码
	*/
	SDK_API s32 Yoseen_StartPreview(s32 userHandle, YoseenPreviewInfo* previewInfo);

	/*
	开始预览, 预览低分辨率的温度数据(小于160x120)时, 放大温度数据.
	只支持取数据，不支持显示.

	@param userHandle 用户句柄
	@param previewInfo 预览信息
	@param tempScale 温度数据放大, 支持2,3,4倍放大
	@return >=0 预览句柄, <0 错误码
	*/
	SDK_API s32 Yoseen_StartPreviewTempScale(s32 userHandle, YoseenPreviewInfo* previewInfo, s32 tempScale);

	/*
	开始预览,  按标准rtsp协议, 预览视频数据. 只需要可见光设备或红外设备支持rtsp协议.

	@param rtspInfo rtsp流信息
	@param previewInfo 预览信息
	@return >=0 预览句柄, <0 错误码
	*/
	SDK_API s32 Yoseen_StartPreviewRtsp(YoseenRtspInfo* rtspInfo, YoseenPreviewInfo* previewInfo);

	/*
	停止预览
	@param previewHandle 预览句柄
	@return 错误码
	*/
	SDK_API s32 Yoseen_StopPreview(s32 previewHandle);

	/*
	暂停预览
	@param previewHandle 预览句柄
	@param pause 0-非暂停, 1-暂停
	@return 错误码
	*/
	SDK_API s32 Yoseen_PausePreview(s32 previewHandle, s32 pause);

	/*
	开始保存, 温度流预览支持保存.stream和.mp4文件, 视频流预览支持保存.mp4文件.
	@param previewHandle 预览句柄
	@param fn 文件名
	@param fileType 文件类型
	@return 错误码
	*/
	SDK_API s32 Yoseen_BeginSave(s32 previewHandle, const char* fn, s32 fileType);

	/*
	停止保存
	@param previewHandle 预览句柄
	@param fileType 文件类型
	@return 错误码
	*/
	SDK_API s32 Yoseen_EndSave(s32 previewHandle, s32 fileType);

	/*
	设置温度流预览, 温度转位图算法参数; 视频流预览使用Yoseen_SetTvoutInfo.
	@param previewHandle 预览句柄
	@param strechControl 温度转位图算法参数
	@param paletteType 调色板
	@return 错误码
	*/
	SDK_API s32 Yoseen_PreviewSetImage(s32 previewHandle, const strech_control* strechControl, s32 paletteType);

	/*
	设置温度流预览, 测温对象参数; 视频流预览使用Yoseen_SetMeasureInfo.
	@param previewHandle 预览句柄
	@param measureInfo 测温对象参数
	@return 错误码
	*/
	SDK_API s32 Yoseen_PreviewSetMeasure(s32 previewHandle, const MeasureInfo* measureInfo);

	/*
	异步地获取温度流或视频流中当前帧的特征温度信息(全局高低温, 中心温度, 局部高低温).
	建议在预览回调函数中, 同步地访问当前帧的特征温度信息.

	@param previewHandle 预览句柄
	@param result 特征温度信息
	@return 错误码
	*/
	SDK_API s32 Yoseen_PreviewGetResult(s32 previewHandle, H264FrameHeader* result);

	/**
	发现热像仪
	@param discoverFlags 0x01广播, 0x02组播, 0x04 广播2
	@return 响应链表
	*/
	SDK_API DiscoverCameraResp2* Yoseen_DiscoverCameras(s32 discoverFlags);

	/*
	释放响应链表
	@param pp 响应链表
	*/
	SDK_API void Yoseen_DiscoverCamerasFree(DiscoverCameraResp2** pp);

	/**
	获取热像仪基本信息
	@param userHandle 用户句柄
	@param [out] cameraBasicInfo 基本信息
	@return 错误码
	*/
	SDK_API s32 Yoseen_GetCameraBasicInfo(s32 userHandle, CameraBasicInfo* cameraBasicInfo);

	/**
	设置热像仪基本信息
	@param userHandle 用户句柄
	@param cameraBasicInfo 基本信息
	@return 错误码
	*/
	SDK_API s32 Yoseen_SetCameraBasicInfo(s32 userHandle, const CameraBasicInfo* cameraBasicInfo);

	/**
	获取热像仪网络信息

	@param userHandle 用户句柄
	@param [out] cameraNetworkInfo 网络信息
	@return 错误码
	*/
	SDK_API s32 Yoseen_GetCameraNetworkInfo(s32 userHandle, CameraNetworkInfo* cameraNetworkInfo);

	/**
	设置热像仪网络信息, 成功设置后, 热像仪IP可能发生变化, 请重新发现热像仪.
	若配置成使用静态IP, 但是忘记IP, 请使用热像仪的GPIO复位热像仪.

	@param userHandle 用户句柄
	@param cameraNetworkInfo 网络信息
	@return 错误码
	*/
	SDK_API s32 Yoseen_SetCameraNetworkInfo(s32 userHandle, const CameraNetworkInfo* cameraNetworkInfo);

	/**
	获取挡板校零信息
	@param userHandle 用户句柄
	@param [out] ffcInfo 挡板校零信息
	@return 错误码
	*/
	SDK_API s32 Yoseen_GetFFCInfo(s32 userHandle, FFCInfo* ffcInfo);

	/**
	设置挡板校零信息, 设置成功后, 会触发一次挡板校零.
	@param userHandle 用户句柄
	@param ffcInfo 挡板校零信息
	@return 错误码
	*/
	SDK_API s32 Yoseen_SetFFCInfo(s32 userHandle, const FFCInfo* ffcInfo);

	/**
	获取模拟视频信息, 模拟视频信息也包含图像算法参数\流媒体参数.
	@param userHandle 用户句柄
	@param [out] tvoutInfo 模拟视频信息
	@return 错误码
	*/
	SDK_API s32 Yoseen_GetTvoutInfo(s32 userHandle, TvoutInfo* tvoutInfo);

	/**
	设置模拟视频信息.
	@param userHandle 用户句柄
	@param tvoutInfo 模拟视频信息
	@return 错误码
	*/
	SDK_API s32 Yoseen_SetTvoutInfo(s32 userHandle, const TvoutInfo* tvoutInfo);

	/**
	获取测温修正信息
	@param userHandle 用户句柄
	@param [out] fixInfo 测温修正信息
	@return 错误码
	*/
	SDK_API s32 Yoseen_GetFixInfo(s32 userHandle, FixInfo* fixInfo);

	/**
	设置测温修正信息
	@param userHandle 用户句柄
	@param fixInfo 测温修正信息
	@return 错误码
	*/
	SDK_API s32 Yoseen_SetFixInfo(s32 userHandle, const FixInfo* fixInfo);

	/**
	获取测温对象信息
	@param userHandle 用户句柄
	@param [out] measureInfo 测温对象信息
	@return 错误码
	*/
	SDK_API s32 Yoseen_GetMeasureInfo(s32 userHandle, MeasureInfo* measureInfo);

	/**
	设置测温对象信息
	@param userHandle 用户句柄
	@param measureInfo 测温对象信息
	@return 错误码
	*/
	SDK_API s32 Yoseen_SetMeasureInfo(s32 userHandle, const MeasureInfo* measureInfo);

	/*
	获取OSD信息
	@param userHandle 用户句柄
	@param osdInfo osd信息
	@return 错误码
	*/
	SDK_API s32 Yoseen_GetCameraOSDInfo(s32 userHandle, CameraOSDInfo* osdInfo);

	/*
	设置OSD信息
	@param userHandle 用户句柄
	@param osdInfo osd信息
	@return 错误码
	*/
	SDK_API s32 Yoseen_SetCameraOSDInfo(s32 userHandle, const CameraOSDInfo* osdInfo);

	/**
	获取热像仪时间信息
	@param userHandle 用户句柄
	@param [out] cameraTimeInfo 时间信息
	@return 错误码
	*/
	SDK_API s32 Yoseen_GetCameraTimeInfo(s32 userHandle, CameraTimeInfo* cameraTimeInfo);

	/**
	设置热像仪时间信息
	@param userHandle 用户句柄
	@param cameraTimeInfo 时间信息
	@return 错误码
	*/
	SDK_API s32 Yoseen_SetCameraTimeInfo(s32 userHandle, const CameraTimeInfo* cameraTimeInfo);

	/**
	获取GPIO信息
	@param userHandle 用户句柄
	@param [out] gpioInfo GPIO信息
	@return 错误码
	*/
	SDK_API s32 Yoseen_GetGpioInfo(s32 userHandle, GpioInfo* gpioInfo);

	/**
	设置GPIO信息
	@param userHandle 用户句柄
	@param gpioInfo GPIO信息
	@return 错误码
	*/
	SDK_API s32 Yoseen_SetGpioInfo(s32 userHandle, const GpioInfo* gpioInfo);

	/**
	获取串口信息
	@param userHandle 用户句柄
	@param [out] serialPortInfo 串口信息
	@return 错误码
	*/
	SDK_API s32 Yoseen_GetSerialPortInfo(s32 userHandle, SerialPortInfo* serialPortInfo);

	/**
	设置串口信息
	@param userHandle 用户句柄
	@param serialPortInfo 串口信息
	@return 错误码
	*/
	SDK_API s32 Yoseen_SetSerialPortInfo(s32 userHandle, const SerialPortInfo* serialPortInfo);

	/**
	发送控制信息
	@param userHandle 用户句柄
	@param ctl 控制信息
	@return 错误码
	*/
	SDK_API s32 Yoseen_SendControl(s32 userHandle, Ctl* ctl);

	/**
	发送控制信息X
	@param userHandle 用户句柄
	@param ctlx 控制信息x
	@return 错误码
	*/
	SDK_API s32 Yoseen_SendControlX(s32 userHandle, CtlX* ctlx);

	/*
	发送串口数据到热像仪, 热像仪接收到数据, 直接写到串口
	@param userHandle 用户句柄
	@param spd 串口数据
	@return 错误码
	*/
	SDK_API s32 Yoseen_WriteSerialPortData(s32 userHandle, const xxxserialportdata* spd);

	/*
	上传本地文件到热像仪, 用于热像仪程序更新、坏点补偿等
	@param userHandle 用户句柄
	@param fn 文件名称
	@param fileType 文件类型
	@return 错误码
	*/
	SDK_API s32 Yoseen_UploadFile(s32 userHandle, const char* fn, s32 fileType);

	/*
	下载热像仪文件到本地, 用于热像仪日志分析等
	@param userHandle 用户句柄
	@param fn 文件名称
	@param fileType 文件类型
	@return 错误码
	*/
	SDK_API s32 Yoseen_DownloadFile(s32 userHandle, const char* fn, s32 fileType);
}

#endif