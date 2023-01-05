#ifndef __CFFCOMMON_H__
#define __CFFCOMMON_H__

#include <windows.h>
#include "MMSystem.h"
#include "../Common/CCriticalSectionPool.h"
#include "../Common/CThreadPool.h"
#include "CVMDStructures.h"

extern "C"
{
	#include "libavcodec/avcodec.h"
	#include "libavdevice/avdevice.h"
	#include "libavformat/avformat.h"
	#include "libavformat/avio.h"
	#include "libswscale/swscale.h"
	#include "libswresample/swresample.h"
	#include "libavutil/imgutils.h"
	#include "libavutil/opt.h"
	#include "libavutil/log.h"
}

class CFFCommon
{
public:
	enum eConnectionStates : int
	{
		NotSetYet = 0,
		NotConnected = 1,
		Connecting = 2,
		Connected = 3,
		UnableToConnect = 4
	};
public:
	typedef struct
	{
		BYTE*       Data;
		DWORD       Size;
		AVMediaType MediaType;
		AVCodecID   CodecID;
		void*       FormatContext;
	} RTSPFrameData;
	//---------------------------------------------
	typedef struct
	{
		AVCodecID     CodecID;
		RTSPFrameData RTSPFrame;
	} DecodeVideoParams;
	typedef struct
	{
		BYTE**        Data;
		int*          LineSize;
		int           Width;
		int           Height;
		AVPixelFormat TargetPixelFormatsID;
		BOOL          ParamsChanged;
	} DecodeVideoData;
	//---------------------------------------------
	typedef struct
	{
		AVCodecID     CodecID;
		RTSPFrameData RTSPFrame;
	} DecodeAudioParams;
	typedef struct
	{
		BYTE**         Data;
		int*           LineSize;
		WORD           BitsPerSample;
		WORD           Channels;
		int            NbSamples;
		uint64_t       ChannelLayout;
		int            SampleRate;
		AVSampleFormat SampleFmt;
		BOOL           ParamsChanged;
	} DecodeAudioData;
	//---------------------------------------------
	typedef struct
	{
		AVCodecID     CodecID;
		AVPixelFormat SourcePixelFormatsID;
		int	          Width;
		int	          Height;
		int	          BitRate;
		int           TimeBaseNum; ///< numerator
		int           TimeBaseDen; ///< denominator
		int           GopSize;
		int           MaxBFrames;
	} EncodeVideoParams;
	typedef struct
	{
		BYTE** Data;
		int    Size;
		BOOL   ParamsChanged;
	} EncodeVideoData;
	//---------------------------------------------
	typedef struct
	{
		AVPixelFormat   SourcePixelFormatsID;
		AVPixelFormat   TargetPixelFormatsID;
		DecodeVideoData DecodedVideo;
		int             ResizedWidth;
		int             ResizedHeight;
	} ColorConversionParams;
	typedef struct
	{
		BYTE** Data;
		int*   LineSize;
		int    Width;
		int    Height;
		BOOL   ParamsChanged;
	} ColorConversionData;
	//---------------------------------------------
	typedef struct
	{
		AVSampleFormat  SampleFormatsID;
		uint64_t        ChannelLayout;
		int             SampleRate;
	} SampleSTParams; //ST is Source / Target.
	typedef struct
	{
		SampleSTParams  Source;
		SampleSTParams  Target;
		DecodeAudioData DecodedAudio;
	} SampleConversionParams;
	typedef struct
	{
		BYTE** Data;
		int*   LineSize;
		DWORD  SamplesPerSec;
		WORD   BitsPerSample;
		WORD   Channels;
		BOOL   ParamsChanged;
	} SampleConversionData;
	//---------------------------------------------
	typedef struct
	{
		ColorConversionData ColorConversion;
	} FilterParams;
	//---------------------------------------------
	typedef struct
	{
		unsigned char* Frame;
		int            TypeOfFrame;
		int            OriginalWidth;
		int            OriginalHeight;
		int            ResizedSquareSize;
		unsigned char* RGBFrame;
		int*           ResizedWidth;
		int*           ResizedHeight;
		int            MedianSquareSize;
		unsigned char* MedianFrame;
		int*           MedianWidth;
		int*           MedianHeight;
		int            BufferSize;
		int            Tolerance;
		int            MinimalDistanceBetweenTwoObjects;
		int            MaximalAmountOfObjectsToFind;
		float          MinWidth;
		float          MinHeight;
		float          MaxWidth;
		float          MaxHeight;
		RECTANGLEF*    InstantMotionAccepted;
		int*           InstantMotionAcceptedCount;
		POINTF*        ZonesPolygons;
		int*           ZonesPointsNumbers;
		float*         ZonesFilters;
		int            ZonesNumber;
		ALARM*         Alarms;
		int*           AlarmsCount;
		int            OneOfNFramesGoesToBuffer;
	} VMDParams;
	typedef struct
	{
		BYTE** Data;
		int*   LineSize;
		int    Width;
		int    Height;
		BOOL   ParamsChanged;
	} VMDData;
	//---------------------------------------------
	enum eWhatToShowTypes : int
	{
		ShowNotSetYet,
		ShowRGBFrame,
		ShowVMDFrame,
		ShowAverageFrame,
		ShowDifferenceFrame,
		ShowToleranceFrame,
		ShowMedianFrame,
		ShowBorderFrame,
		ShowFilterFrame
	};
	//---------------------------------------------
	enum eFilterTypes : int
	{
		Filter_Blur_01, 
		Filter_Blur_02, 
		Filter_Blur_03, 
		Filter_Motion_Blur,
		Filter_Edges_01,
		Filter_Edges_02,
		Filter_Edges_03,
		Filter_Edges_04,
		Filter_Edges_Prewitt,
		Filter_Edges_Sobel,
		Filter_Edges_Laplacian,
		Filter_Shapen_01,
		Filter_Shapen_02,
		Filter_Shapen_03,
		Filter_Emboss_01,
		Filter_Emboss_02,
		Filter_Mean
	};
	//=============================================
	enum eShootingRangeInfoDataTypes : int
	{
		//-------------------------------------
		VMD_AverageBufferSizeInFrames,
		VMD_ResizeSquare,
		VMD_Tolerance,
		VMD_MedianSquare,
		VMD_MinimalDistanceBetweenTwoObjects,
		VMD_MaximalAmountOfObjectsToFind,
		VMD_WhatToShowType,
		//-------------------
		VMD_AcceptedObjectLimit_MinWidth,
		VMD_AcceptedObjectLimit_MinHeight,
		VMD_AcceptedObjectLimit_MaxWidth,
		VMD_AcceptedObjectLimit_MaxHeight,
		//-------------------
		VMD_IdleTimeAfterShotInMS,
		//-------------------------------------
		Draw_Windows_TargetWindow,
		//-------------------------------------
		Connection_URL,
		Connection_IsWebCam
		//-------------------------------------
	};
	//=============================================
	typedef struct
	{
		float MinWidth;
		float MinHeight;
		float MaxWidth;
		float MaxHeight;
	} LimitInfo;
	//---------------------------------------------
	typedef struct
	{
		int AverageBufferSizeInFrames;
		int ResizeSquare;
		int Tolerance;
		int MedianSquare;
		int MinimalDistanceBetweenTwoObjects;
		int MaximalAmountOfObjectsToFind;
		CFFCommon::eWhatToShowTypes WhatToShowType;
		LimitInfo AcceptedObjectLimit;
		int IdleTimeAfterShotInMS;
	} VMDInfo;
	//---------------------------------------------
	typedef struct
	{
		HWND TargetWindow;
	} WindowInfo;
	//---------------------------------------------
	typedef struct
	{
		WindowInfo Windows;
	} DrawInfo;
	//---------------------------------------------
	typedef struct
	{
		TCHAR URL[256];
		BOOL  IsWebCam;
	} ConnectionInfo;
	//---------------------------------------------
	typedef struct
	{
		VMDInfo VMD;
		DrawInfo Draw;
		ConnectionInfo Connection;
	} ShootingRangeInfo;
	//=============================================
	typedef struct
	{
		HWND                Window;
		BYTE*               Data;
		RECTANGLEF*         InstantMotionAccepted;
		POINTF*             InstantMotionAcceptedAveragePoint;
		int                 InstantMotionAcceptedCount;
		int                 ColorConversionWidth;
		int                 ColorConversionHeight;
		ShootingRangeInfo*  ShootingRangeInfo;
	} GDIDrawParams;
};

typedef void(*dOnFrameReceived)(void* user, CFFCommon::RTSPFrameData rtspFrame);

#endif //__CFFCOMMON_H__