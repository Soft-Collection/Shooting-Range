#ifndef __CFFCOLORCONVERSION_H__
#define __CFFCOLORCONVERSION_H__

//http://stackoverflow.com/questions/10715170/receiving-rtsp-stream-using-ffmpeg-library

#include "CFFCommon.h"

class CFFColorConversion
{
private:
	enum eCriticalSections : int
	{
		ColorConversion = 0,
		CSSize = 1 //Must be last.
	};
private:
	uint8_t*                mPicture_Buf_RGB;
	AVFrame*                mPicRGB;
	SwsContext*             mImg_Convert_Context;
	CCriticalSectionPool*   mCriticalSectionPool;
	int                     mLastWidth;
	int                     mLastHeight;
	int                  	mLastResizedWidth;
	int                  	mLastResizedHeight;
	BOOL                    mResourcesMustBeReallocated;
private:
	BOOL AllocateResources(CFFCommon::ColorConversionParams colorConversionParams);
	BOOL DeallocateResources();
	BOOL AreParamsChanged(CFFCommon::ColorConversionParams colorConversionParams);
public:
	CFFColorConversion();
	~CFFColorConversion();
	int PerformColorConversion(CFFCommon::ColorConversionParams colorConversionParams, CFFCommon::ColorConversionData* colorConversionData);
	void ReallocateResources();
};

#endif //__CFFCOLORCONVERSION_H__