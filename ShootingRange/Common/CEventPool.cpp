#include "CExceptionReport.h"
#include "../Common/CEventPool.h"

CEventPool::CEventPool(INT numOfEvents, BOOL manual)
{
	try
	{
		mEventArray = NULL;
		mEventArrayLength = numOfEvents;
		mEventArray = new HANDLE[mEventArrayLength];
		//------------------------------------------------------------------------
		for (INT i = 0; i < mEventArrayLength; i++)
		{
			mEventArray[i] = CreateEvent(NULL, manual, FALSE, NULL); //manual reset event
		}
	}
	catch (...)
	{
		CExceptionReport::WriteExceptionReportToFile("CManualResetEventPool::CManualResetEventPool", "Exception in CManualResetEventPool::CManualResetEventPool");
	}
}
CEventPool::~CEventPool()
{
	try
	{
		for (INT i = 0; i < mEventArrayLength; i++)
		{
			if (mEventArray[i] != NULL)
			{
				CloseHandle(mEventArray[i]);
				mEventArray[i] = NULL;
			}
		}
		//------------------------------------------------------------------------
		if (mEventArray != NULL)
		{
			delete[] mEventArray;
			mEventArray = NULL;
		}
	}
	catch (...)
	{
		CExceptionReport::WriteExceptionReportToFile("CManualResetEventPool::~CManualResetEventPool", "Exception in CManualResetEventPool::~CManualResetEventPool");
	}
}
BOOL CEventPool::Set(INT eventNumber)
{
	BOOL retVal = FALSE;
	try
	{
		if (mEventArray[eventNumber] != NULL)
		{
			retVal = SetEvent(mEventArray[eventNumber]);
		}
	}
	catch (...)
	{
		CExceptionReport::WriteExceptionReportToFile("CManualResetEventPool::Set", "Exception in CManualResetEventPool::Set");
	}
	return(retVal);
}
BOOL CEventPool::Reset(INT eventNumber)
{
	BOOL retVal = FALSE;
	try
	{
		if (mEventArray[eventNumber] != NULL)
		{
			retVal = ResetEvent(mEventArray[eventNumber]);
		}
	}
	catch (...)
	{
		CExceptionReport::WriteExceptionReportToFile("CManualResetEventPool::Reset", "Exception in CManualResetEventPool::Reset");
	}
	return(retVal);
}
BOOL CEventPool::Wait(INT eventNumber, DWORD dwMilliseconds)
{
	BOOL retVal = FALSE;
	try
	{
		DWORD dwObject = WaitForSingleObject(mEventArray[eventNumber], dwMilliseconds);
		retVal = (dwObject != WAIT_TIMEOUT);
	}
	catch (...)
	{
		CExceptionReport::WriteExceptionReportToFile("CManualResetEventPool::Wait", "Exception in CManualResetEventPool::Wait");
	}
	return(retVal);
}