#include "CCriticalSectionPool.h"
#include "../Common/CExceptionReport.h"

CCriticalSectionPool::CCriticalSectionPool(INT numOfCriticalSections)
{
	try
	{
		mCriticalSectionArray = NULL;
		mCriticalSectionArrayLength = numOfCriticalSections;
		mCriticalSectionArray = new CRITICAL_SECTION[mCriticalSectionArrayLength];
		//------------------------------------------------------------------------
		for (INT i = 0; i < mCriticalSectionArrayLength; i++)
		{
			InitializeCriticalSection(&mCriticalSectionArray[i]);
		}
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CCriticalSectionPool::CCriticalSectionPool", "Exception in CCriticalSectionPool Constructor");
	}
}
CCriticalSectionPool::~CCriticalSectionPool()
{
	try
	{
		for (INT i = 0; i < mCriticalSectionArrayLength; i++)
		{
			DeleteCriticalSection(&mCriticalSectionArray[i]);
		}
		//------------------------------------------------------------------------
		if (mCriticalSectionArray != NULL)
		{
			delete[] mCriticalSectionArray;
			mCriticalSectionArray = NULL;
		}
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CCriticalSectionPool::~CCriticalSectionPool", "Exception in CCriticalSectionPool Destructor");
	}
}
void CCriticalSectionPool::Enter(INT criticalSectionNumber)
{
	EnterCriticalSection(&mCriticalSectionArray[criticalSectionNumber]);
}
BOOL CCriticalSectionPool::TryEnter(INT criticalSectionNumber)
{
	return(TryEnterCriticalSection(&mCriticalSectionArray[criticalSectionNumber]));
}
void CCriticalSectionPool::Leave(INT criticalSectionNumber)
{
	LeaveCriticalSection(&mCriticalSectionArray[criticalSectionNumber]);
}