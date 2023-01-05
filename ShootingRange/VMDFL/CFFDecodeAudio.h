#ifndef __CFFDECODEAUDIO_H__
#define __CFFDECODEAUDIO_H__

//http://stackoverflow.com/questions/10715170/receiving-rtsp-stream-using-ffmpeg-library
//https://ffmpeg.org/doxygen/trunk/decoding__encoding_8c-source.html

#include "CFFCommon.h"

class CFFDecodeAudio
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
	AVFormatContext*        mFormatContext;
	AVCodecContext*         mCodecContext;
	AVFrame*                mDecodedFrame;
	int                     mLastSampleRate;
	BOOL                    mResourcesMustBeReallocated;
private:
	BOOL AllocateResources(CFFCommon::DecodeAudioParams decodeAudioParams);
	BOOL DeallocateResources();
	BOOL AreParamsChanged(CFFCommon::DecodeAudioParams decodeAudioParams);
public:
	CFFDecodeAudio();
	~CFFDecodeAudio();
	int DecodeAudio(CFFCommon::DecodeAudioParams decodeAudioParams, CFFCommon::DecodeAudioData* decodeAudioData);
	void ReallocateResources();
};

#endif //__CFFDECODEAUDIO_H__