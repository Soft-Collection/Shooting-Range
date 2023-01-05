//https://wiki.openttd.org/Compiling_on_Windows_using_Microsoft_Visual_C%2B%2B_2012
//Fix for Error C1189: "The C++ Standard Library forbids macroizing keywords. Enable warning C4005 to find the forbidden macro[...]"
#include "stdafx.h"
#include "CBuffer.h"
#include <tchar.h>

CBuffer::CBuffer(dOnFrameReceived onFrameReceived, void* user)
{
	mCriticalSectionPool = new CCriticalSectionPool(eCriticalSections::CSSize);
	//-------------------------------------------------------
	mOnFrameReceived = onFrameReceived;
	mUser = user;	
	//-------------------------------------------------------
	mCriticalSectionPool->Enter(eCriticalSections::Queue);
	mQueue = new std::queue<CFFCommon::RTSPFrameData>();
	mCriticalSectionPool->Leave(eCriticalSections::Queue);
	//-------------------------------------------------------
	DWORD dwThread;
	mThreadQuit = FALSE;
	mThread = CreateThread(NULL, 0, MyThreadFunctionStatic, this, 0, &dwThread);
	//-------------------------------------------------------
	mDelayInMilliSeconds = 0;
}

CBuffer::~CBuffer()
{
	mThreadQuit = TRUE;
	if (mThread != NULL)
	{
		WaitForSingleObject(mThread, INFINITE);
		mThread = NULL;
	}
	mCriticalSectionPool->Enter(eCriticalSections::Queue);
	if (mQueue != NULL)
	{
		while (!mQueue->empty())
		{
			CFFCommon::RTSPFrameData tempRTSPFrameData;
			if (Dequeue(&tempRTSPFrameData)) Free(tempRTSPFrameData);
		}
		delete mQueue;
		mQueue = NULL;
	}
	mCriticalSectionPool->Leave(eCriticalSections::Queue);
	if (mCriticalSectionPool != NULL)
	{
		delete mCriticalSectionPool;
		mCriticalSectionPool = NULL;
	}
}

void CBuffer::Enqueue(CFFCommon::RTSPFrameData rtspFrameData)
{
	CFFCommon::RTSPFrameData tempRTSPFrameData = { 0, };
	memcpy(&tempRTSPFrameData, &rtspFrameData, sizeof(CFFCommon::RTSPFrameData));
	tempRTSPFrameData.Data = new BYTE[tempRTSPFrameData.Size];
	memcpy(tempRTSPFrameData.Data, rtspFrameData.Data, tempRTSPFrameData.Size);
	mCriticalSectionPool->Enter(eCriticalSections::Queue);
	if (mQueue != NULL) mQueue->push(tempRTSPFrameData);
	mCriticalSectionPool->Leave(eCriticalSections::Queue);
}

BOOL CBuffer::Dequeue(CFFCommon::RTSPFrameData* rtspFrameData)
{
	BOOL retVal = FALSE;
	mCriticalSectionPool->Enter(eCriticalSections::Queue);
	if (mQueue != NULL)
	{
		if (!mQueue->empty())
		{
			*rtspFrameData = mQueue->front();
			mQueue->pop();
			retVal = TRUE;
		}
	}
	mCriticalSectionPool->Leave(eCriticalSections::Queue);
	return retVal;
}

void CBuffer::Free(CFFCommon::RTSPFrameData rtspFrameData)
{
	delete[] rtspFrameData.Data;
}

DWORD WINAPI CBuffer::MyThreadFunctionStatic(LPVOID lpParam)
{
	try
	{
		CBuffer* buf = (CBuffer*)lpParam;
		if (buf != NULL)
		{
			buf->MyThreadFunction();
		}
	}
	catch (...)
	{
	}
	return 0;
}

void CBuffer::MyThreadFunction()
{
	while (!mThreadQuit)
	{
		try
		{
			int bufferCount = mQueue->size();
			//-------------------------------------------------------
			mCriticalSectionPool->Enter(eCriticalSections::Queue);
			//-------------------------------------------------------
			if (mOnFrameReceived != NULL)
			{
				CFFCommon::RTSPFrameData tempRTSPFrameData;
				if (Dequeue(&tempRTSPFrameData))
				{
					(*(mOnFrameReceived))(mUser, tempRTSPFrameData);
					Free(tempRTSPFrameData);
				}
			}
			//-------------------------------------------------------
			mCriticalSectionPool->Leave(eCriticalSections::Queue);
			//-------------------------------------------------------
			Sleep(mDelayInMilliSeconds);
		}
		catch (...)
		{
		}
	}
}