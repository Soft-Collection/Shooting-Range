#include "CThreadPool.h"
#include "../Common/CExceptionReport.h"

CThreadPool::CThreadPool(INT numOfCriticalSections, LPTHREAD_START_ROUTINE* callbacks, void* user)
{
	try
	{
		mThreadArray = NULL;
		mThreadQuitArray = NULL;
		mThreadArrayLength = numOfCriticalSections;
		mThreadArray = new HANDLE[mThreadArrayLength];
		mThreadQuitArray = new BOOL[mThreadArrayLength];
		//------------------------------------------------------------------------
		for (INT i = 0; i < mThreadArrayLength; i++)
		{
			DWORD dwThread;
			mThreadQuitArray[i] = FALSE;
			mThreadArray[i] = CreateThread(NULL, 0, callbacks[i], user, 0, &dwThread);
		}
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CCriticalSectionPool::CCriticalSectionPool", "Exception in CCriticalSectionPool Constructor");
	}
}
CThreadPool::~CThreadPool()
{
	try
	{
		for (INT i = 0; i < mThreadArrayLength; i++)
		{
			if (mThreadQuitArray != NULL)
			{
				if (mThreadQuitArray[i] != TRUE)
				{
					mThreadQuitArray[i] = TRUE;
				}
			}
		}
		for (INT i = 0; i < mThreadArrayLength; i++)
		{
			if (mThreadArray != NULL)
			{
				if (mThreadArray[i] != NULL)
				{
					WaitForSingleObject(mThreadArray[i], INFINITE);
					mThreadArray[i] = NULL;
				}
			}
		}
		//------------------------------------------------------------------------
		if (mThreadQuitArray != NULL)
		{
			delete[] mThreadQuitArray;
			mThreadQuitArray = NULL;
		}
		if (mThreadArray != NULL)
		{
			delete[] mThreadArray;
			mThreadArray = NULL;
		}
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CCriticalSectionPool::~CCriticalSectionPool", "Exception in CCriticalSectionPool Destructor");
	}
}
void CThreadPool::Wait(INT threadNumber)
{
	WaitForSingleObject(mThreadArray[threadNumber], INFINITE);
}
BOOL CThreadPool::IsQuit(INT threadNumber)
{
	return mThreadQuitArray[threadNumber];
}
void CThreadPool::Quit(INT threadNumber)
{
	mThreadQuitArray[threadNumber] = TRUE;
}