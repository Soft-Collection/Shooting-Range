#ifndef __CBUFFER_H__
#define __CBUFFER_H__

#include "CFFCommon.h"
#include <queue>

class CBuffer
{
private:
	enum eCriticalSections : int
	{
		Queue = 0,
		CSSize = 1 //Must be last.
	};
private:
	CCriticalSectionPool*                 mCriticalSectionPool;
	HANDLE                                mThread;
	BOOL                                  mThreadQuit;
	dOnFrameReceived                      mOnFrameReceived;
	void*                                 mUser;
	std::queue<CFFCommon::RTSPFrameData>* mQueue;
	int                                   mDelayInMilliSeconds;
public:
	CBuffer(dOnFrameReceived onFrameReceived, void* user);
	~CBuffer();
	void Enqueue(CFFCommon::RTSPFrameData rtspFrameData);
	BOOL Dequeue(CFFCommon::RTSPFrameData* rtspFrameData);
	static void Free(CFFCommon::RTSPFrameData rtspFrameData);
public:
	static DWORD WINAPI MyThreadFunctionStatic(LPVOID lpParam);
	void MyThreadFunction();
};

#endif //__CBUFFER_H__