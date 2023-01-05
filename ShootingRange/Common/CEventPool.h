#ifndef __CEVENTSPOOL_H__
#define __CEVENTSPOOL_H__

#include <windows.h>

class CEventPool
{
private:
	HANDLE* mEventArray;
	INT     mEventArrayLength;
public:
	CEventPool(INT numOfEvents, BOOL manual);
	~CEventPool();
	BOOL Set(INT eventNumber);
	BOOL Reset(INT eventNumber);
	BOOL Wait(INT eventNumber, DWORD dwMilliseconds);
};
#endif // __CEVENTSPOOL_H__