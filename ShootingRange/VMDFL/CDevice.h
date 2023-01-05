#ifndef __CDEVICE_H__
#define __CDEVICE_H__

#include "CFFCommon.h"
#include "CFFMpegRtsp.h"
#include "CBuffer.h"
#include "CFFDecodeVideo.h"
#include "CFFDecodeAudio.h"
#include "CFFSampleConversion.h"
#include "CWavePlaying.h"
#include "CFFColorConversion.h"
#include "CFilter.h"
#include "CGDIDraw.h"
#include "CVMDAverageBuffer.h"
#include "CVMDFrame.h"
#include <tchar.h>

class CDevice
{
private:
	HANDLE	                         mEventShot;
	//----------------------------------------
	LPCRITICAL_SECTION               mCriticalSection;
	//----------------------------------------
	CFFCommon::ShootingRangeInfo*    mShootingRangeInfo;
	//----------------------------------------
	BYTE*                            mRGBPicture;
	int                              mRGBPictureWidth;
	int                              mRGBPictureHeight;
	//----------------------------------------
	class  CFFMpegRtsp*              mFFMpegRtsp;
	class  CBuffer*                  mBuffer;
	class  CFFDecodeVideo*           mFFDecodeVideo;
	class  CFFDecodeAudio*           mFFDecodeAudio;
	class  CFFSampleConversion*      mFFSampleConversion;
	class  CWavePlaying*             mWavePlaying;
	class  CFFColorConversion*       mFFColorConversion;
	class  CFilter*                  mFilter;
	class  CGDIDraw*                 mGDIDrawTarget;
	//----------------------------------------
	RECTANGLEF*                      mInstantMotionAccepted;
	POINTF*                          mInstantMotionAcceptedAveragePoint;
	CVMDAverageBuffer*               mAverageBuffer;
	//----------------------------------------
	int                              mLastResizeSquare;
	//----------------------------------------
	DWORD                            mLastShotTimeInMS;
	POINTF                           mLastShotCoordinates;
	POINTF                           mLastShotBorder[4];
	//----------------------------------------
	BOOL                             mAverageBufferMustBeReallocated;
public:
	CDevice();
	~CDevice();
	void Connect();
	void Disconnect();
	CFFCommon::eConnectionStates GetConnectionState();
	void GetShotData(POINTF* shotCoordinates, POINTF* shotBorder);
	void RGBPictureLock();
	void RGBPictureUnlock();
	void GetRGBPictureSize(int* width, int* height);
	void GetRGBPictureData(BYTE* data);
	//--------------------------------------------------------------------------------------------------
	void SetShootingRangeInfo(CFFCommon::eShootingRangeInfoDataTypes shootingRangeInfoDataType, int val);
	void SetShootingRangeInfo(CFFCommon::eShootingRangeInfoDataTypes shootingRangeInfoDataType, float val);
	void SetShootingRangeInfo(CFFCommon::eShootingRangeInfoDataTypes shootingRangeInfoDataType, POINTF val);
	void SetShootingRangeInfo(CFFCommon::eShootingRangeInfoDataTypes shootingRangeInfoDataType, POINTF* val);
	void SetShootingRangeInfo(CFFCommon::eShootingRangeInfoDataTypes shootingRangeInfoDataType, LPTSTR val);
	void SetShootingRangeInfo(CFFCommon::eShootingRangeInfoDataTypes shootingRangeInfoDataType, HWND val);
	void SetShootingRangeInfo(CFFCommon::eShootingRangeInfoDataTypes shootingRangeInfoDataType, COLORREF val);
	//--------------------------------------------------------------------------------------------------
private:
	static void OnFrameReceivedFromRTSPStatic(void* user, CFFCommon::RTSPFrameData rtspFrame);
	void        OnFrameReceivedFromRTSP(CFFCommon::RTSPFrameData rtspFrame);
	static void OnFrameReceivedFromBufferStatic(void* user, CFFCommon::RTSPFrameData rtspFrame);
	void        OnFrameReceivedFromBuffer(CFFCommon::RTSPFrameData rtspFrame);
};

#endif //__CDEVICE_H__