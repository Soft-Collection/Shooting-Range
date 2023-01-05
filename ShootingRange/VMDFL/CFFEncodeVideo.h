#ifndef __CFFENCODEVIDEO_H__
#define __CFFENCODEVIDEO_H__

//http://stackoverflow.com/questions/10715170/receiving-rtsp-stream-using-ffmpeg-library

#include "CFFCommon.h"

class CFFEncodeVideo
{
private:
	enum eCriticalSections : int
	{
		Encode = 0,
		CSSize = 1 //Must be last.
	};
private:
	CCriticalSectionPool*   mCriticalSectionPool;
	AVCodec*                mCodec;
	AVCodecContext*         mCodecContext;
	AVFrame*                mPicYUV;
	AVPacket*               mPacket;
	int                     mFrameCounter;
	int                     mLastWidth;
	int                     mLastHeight;
	BOOL                    mResourcesMustBeReallocated;
private:
	BOOL AllocateResources(CFFCommon::EncodeVideoParams encodeVideoParams);
	BOOL DeallocateResources();
	BOOL AreParamsChanged(CFFCommon::EncodeVideoParams encodeVideoParams);
public:
	CFFEncodeVideo();
	~CFFEncodeVideo();
	int EncodeVideo(CFFCommon::EncodeVideoParams encodeVideoParams, CFFCommon::EncodeVideoData* encodeVideoData);
	void ReallocateResources();
};

#endif //__CFFENCODEVIDEO_H__