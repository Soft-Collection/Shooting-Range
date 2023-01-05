#include "CExceptionReport.h"

FILE* CExceptionReport::ms_fp = NULL;
CRITICAL_SECTION* CExceptionReport::ms_crit = NULL;

BOOL CExceptionReport::SetFileName(char* fileName)
{
	BOOL retVal = TRUE;
	try
	{
		if (!ms_crit)
		{
			ms_crit = new CRITICAL_SECTION;
			InitializeCriticalSection(ms_crit);
		}
		EnterCriticalSection(ms_crit);
		do
		{
			int res;
			if (ms_fp)
			{
				res = fclose(ms_fp);
				if (!res) {retVal = FALSE; break;}
			}
			ms_fp = fopen(fileName, "at");
			if (!ms_fp) {retVal = FALSE; break;}
		} while(0);
		LeaveCriticalSection(ms_crit);
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CExceptionReport::SetFileName", "Exception in CExceptionReport::SetFileName");
	}
	return retVal;
}

BOOL CExceptionReport::WriteExceptionReportToFile(char* functionLocation, char* exceptionDescription)
{
	return TRUE;
	//--------------------------------------
	BOOL retVal = TRUE;
	try
	{
		if (!ms_crit)
		{
			ms_crit = new CRITICAL_SECTION;
			InitializeCriticalSection(ms_crit);
		}
		EnterCriticalSection(ms_crit);
		do
		{
			if (!ms_fp) ms_fp = fopen("ExceptionReport.txt", "at");
			if (!ms_fp) {retVal = FALSE; break;}
			int res;
			//--------------------------------------
			time_t t = time(0);
			struct tm* lt = localtime(&t);
			res = fprintf(ms_fp, "%s%02d/%02d/%04d %02d:%02d:%02d%s\n", "--------------------------", lt->tm_mday, lt->tm_mon + 1, lt->tm_year + 1900, lt->tm_hour, lt->tm_min, lt->tm_sec, "--------------------------");
			//--------------------------------------
			res = fprintf(ms_fp, "%s%s\n", "Function Location: ", functionLocation);
			res = fprintf(ms_fp, "%s%s\n", "Exception Description: ", exceptionDescription);
			//--------------------------------------
			res = fflush(ms_fp);
		} while(0);
		LeaveCriticalSection(ms_crit);
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CExceptionReport::WriteExceptionReportToFile", "Exception in CExceptionReport::WriteExceptionReportToFile");
	}
	return retVal;
}

BOOL CExceptionReport::CloseFile()
{
	BOOL retVal = TRUE;
	try
	{
		if (!ms_crit)
		{
			ms_crit = new CRITICAL_SECTION;
			InitializeCriticalSection(ms_crit);
		}
		EnterCriticalSection(ms_crit);
		do
		{
			int res;
			if (ms_fp)
			{
				res = fclose(ms_fp);
				if (!res) {retVal = FALSE; break;}
				ms_fp = NULL;
			}
		} while(0);
		LeaveCriticalSection(ms_crit);
		DeleteCriticalSection(ms_crit);
		ms_crit = NULL;
	}
	catch(...)
	{ 
		CExceptionReport::WriteExceptionReportToFile("CExceptionReport::CloseFile", "Exception in CExceptionReport::CloseFile");
	}
	return retVal;
}