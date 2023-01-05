#ifndef __CFFDECODEVIDEO_H__
#define __CFFDECODEVIDEO_H__

//http://stackoverflow.com/questions/10715170/receiving-rtsp-stream-using-ffmpeg-library

#include "CFFCommon.h"

class CFFDecodeVideo
{
private:
	enum eCriticalSections : int
	{
		Decode = 0,
		CSSize = 1 //Must be last.
	};
private:
	CCriticalSectionPool*   mCriticalSectionPool;
	AVCodec*                mCodec;
	AVDictionary*           mOpts;
	uint8_t*                mPicture_Buf_YUV;
	AVFormatContext*        mFormatContext;
	AVCodecContext*         mCodecContext;
	AVFrame*                mPicYUV;
	int                     mLastWidth;
	int                     mLastHeight;
	BOOL                    mResourcesMustBeReallocated;
private:
	BOOL AllocateResources(CFFCommon::DecodeVideoParams decodeVideoParams);
	BOOL DeallocateResources();
	BOOL AreParamsChanged(CFFCommon::DecodeVideoParams decodeVideoParams);
public:
	CFFDecodeVideo();
	~CFFDecodeVideo();
	int DecodeVideo(CFFCommon::DecodeVideoParams decodeVideoParams, CFFCommon::DecodeVideoData* decodeVideoData);
	void ReallocateResources();
};

#endif //__CFFDECODEVIDEO_H__