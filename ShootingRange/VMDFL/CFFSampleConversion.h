#ifndef __CFFSAMPLECONVERSION_H__
#define __CFFSAMPLECONVERSION_H__

//http://stackoverflow.com/questions/10715170/receiving-rtsp-stream-using-ffmpeg-library

#include "CFFCommon.h"

class CFFSampleConversion
{
private:
	enum eCriticalSections : int
	{
		SampleConversion = 0,
		CSSize = 1 //Must be last.
	};
private:
	SwrContext*               mSwrContext;
	uint8_t*                  mOutputBuffer;
	int                       mOutputBufferLineSize;
	CCriticalSectionPool*     mCriticalSectionPool;
	CFFCommon::SampleSTParams mLastSource;
	BOOL                      mResourcesMustBeReallocated;
private:
	BOOL AllocateResources(CFFCommon::SampleConversionParams sampleConversionParams);
	BOOL DeallocateResources();
	BOOL AreParamsChanged(CFFCommon::SampleConversionParams sampleConversionParams);
public:
	CFFSampleConversion();
	~CFFSampleConversion();
	int PerformSampleConversion(CFFCommon::SampleConversionParams sampleConversionParams, CFFCommon::SampleConversionData* sampleConversionData);
	void ReallocateResources();
};

#endif //__CFFSAMPLECONVERSION_H__