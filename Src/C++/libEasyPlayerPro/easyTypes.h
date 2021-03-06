
#ifndef __EASY_TYPES_H__
#define __EASY_TYPES_H__

#ifdef _WIN32
#define LIB_EASYPLAYER_PRO_API  __declspec(dllexport)
#ifndef LIB_APICALL
#define LIB_APICALL  __stdcall
#endif
#define WIN32_LEAN_AND_MEAN
#else
#define LIB_EASYPLAYER_PRO_API
#define LIB_APICALL 
#endif



//媒体类型
#ifndef MEDIA_TYPE_VIDEO
#define MEDIA_TYPE_VIDEO		0x00000001
#endif
#ifndef MEDIA_TYPE_AUDIO
#define MEDIA_TYPE_AUDIO		0x00000002
#endif
#ifndef MEDIA_TYPE_EVENT
#define MEDIA_TYPE_EVENT		0x00000004
#endif
#ifndef MEDIA_TYPE_RTP
#define MEDIA_TYPE_RTP			0x00000008
#endif
#ifndef MEDIA_TYPE_SDP
#define MEDIA_TYPE_SDP			0x00000010
#endif
#ifndef MEDIA_TYPE_CODEC_INFO
#define	MEDIA_TYPE_CODEC_INFO	0x00000020
#endif


typedef enum __RTSP_SERVER_ERROR_CODE_ENUM
{
	RTSP_SERVER_ERR_NoErr			=	0,
	RTSP_SERVER_ERR_NotInitialized	=	-1,			//没有初始化
	RTSP_SERVER_ERR_BadArgument		=	-2,			//参数错误
	RTSP_SERVER_ERR_ALLOC_MEMORY	=	-3,			//内存分配失败
	RTSP_SERVER_ERR_OPERATE			=	-4,			//操作失败
}RTSP_SERVER_ERROR_CODE_ENUM;


//video codec
#define	VIDEO_CODEC_H264	0x1C
#define VIDEO_CODEC_H265	0xAE	/* 174 */
#define	VIDEO_CODEC_MJPEG	0x08
#define	VIDEO_CODEC_MPEG4	0x0D
//audio codec
#define AUDIO_CODEC_MP4A	0x15002		//86018
#define AUDIO_CODEC_PCMU	0x10006		//65542	g711 ulaw
#define AUDIO_CODEC_PCMA	0x10007		//g711 alaw
#define AUDIO_CODEC_G726	0x1100B		//69643	g726
//event codec
#define	EVENT_CODEC_ERROR	0x63657272	//cerr
#define EVENT_CODEC_RECONN	0x7265636F	//reco
#define	EVENT_CODEC_EXIT	0x65786974	//exit


//视频关键字标识
//#define RTSP_VIDEO_FRAME_I		0x01		//I帧
#define FRAME_TYPE_I		0x01		//I帧
#define FRAME_TYPE_P		0x02		//P帧
#define FRAME_TYPE_B		0x03		//B帧
#define FRAME_TYPE_J		0x04		//JPEG



//连接类型
typedef enum __RTP_CONNECT_TYPE_ENUM
{
	RTP_OVER_TCP	=	0x01,
	RTP_OVER_UDP
}RTP_CONNECT_TYPE_ENUM;

// 播放速度
typedef enum __PLAY_SPEED_ENUM
{	
	PLAY_SPEED_UNKNOWN	=	-1,
	PLAY_SPEED_NORMAL	=	0x00,		// 正常播放
	PLAY_SPEED_PAUSED,					// 暂停
	PLAY_SPEED_SLOW_X2,					// 1/2
	PLAY_SPEED_SLOW_X4,					// 1/4
	PLAY_SPEED_SLOW_X8,					// 1/8
	PLAY_SPEED_FAST_X2,					// x2
	PLAY_SPEED_FAST_X4,					// x4
	PLAY_SPEED_FAST_X8,					// x8
	PLAY_SPEED_REWIND_X2,				// -2x
	PLAY_SPEED_REWIND_X4,				// -4x
	PLAY_SPEED_REWIND_X8,				// -8x	

	PLAY_SPEED_SINGLE_FRAME,			//单帧播放,手动单击
}PLAY_SPEED_ENUM;


//媒体信息
typedef struct __EASY_MEDIA_INFO_T
{
	unsigned int videoCodec;			//视频编码类型
	unsigned int videoFps;				//视频帧率
	int			 videoWidth;
	int			 videoHeight;
	float		 videoBitrate;

	unsigned int audioCodec;			//音频编码类型
	unsigned int audioSampleRate;		//音频采样率
	unsigned int audioChannel;			//音频通道数
	unsigned int audioBitsPerSample;	//音频采样精度

	unsigned int metadataCodec;			//Metadata类型

	unsigned int vpsLength;				//视频vps帧长度
	unsigned int spsLength;				//视频sps帧长度
	unsigned int ppsLength;				//视频pps帧长度
	unsigned int seiLength;				//视频sei帧长度
	unsigned char	 vps[255];			//视频vps帧内容
	unsigned char	 sps[255];			//视频sps帧内容
	unsigned char	 pps[128];			//视频sps帧内容
	unsigned char	 sei[128];			//视频sei帧内容
}EASY_MEDIA_INFO_T;




#endif
