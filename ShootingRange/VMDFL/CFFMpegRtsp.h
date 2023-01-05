#ifndef __CFFMPEGRTSP_H__
#define __CFFMPEGRTSP_H__

//http://stackoverflow.com/questions/10715170/receiving-rtsp-stream-using-ffmpeg-library

#include "CFFCommon.h"

class CFFMpegRtsp
{
private:
	enum eCriticalSections : int
	{
		CSRTSP = 0,
		CSStatus = 1,
		CSSize = 2 //Must be last.
	};
	enum eThreads : int
	{
		TRRTSP = 0,
		TRStatus = 1,
		TRSize = 2 //Must be last.
	};
private:
	CHAR*	                     mRTSPString;
	BOOL                         mIsWebCam;
	dOnFrameReceived             mOnFrameReceived;
	void*                        mUser;
	AVFormatContext*             mFormatContext;
	CCriticalSectionPool*        mCriticalSectionPool;
	BOOL                         mIsConnected;
	CFFCommon::eConnectionStates mConnectionState;
	DWORD                        mLastFrameTime;
	DWORD                        mBeginConnectingTime;
	CThreadPool*                 mThreadPool;
	static BOOL                  mStaticInitialized;
public:
	CFFMpegRtsp(dOnFrameReceived onFrameReceived, void* user);
	~CFFMpegRtsp();
	void Connect(LPTSTR rtspString, BOOL isWebCam);
	void Disconnect();
	void Reconnect();
	BOOL IsConnected();
	void Play();
	void Pause();
	CFFCommon::eConnectionStates GetConnectionState();
public:
	static DWORD WINAPI MyThreadRTSPFunctionStatic(LPVOID lpParam);
	void MyThreadRTSPFunction();
	static DWORD WINAPI MyThreadStatusFunctionStatic(LPVOID lpParam);
	void MyThreadStatusFunction();
	static void MyLogCallbackFunctionStatic(void *ptr, int level, const char *fmt, va_list vl);
private:
	static void InitStatic();
	void SetConnectionState(CFFCommon::eConnectionStates connectionState);
};

#endif //__CFFMPEGRTSP_H__