#ifndef __CCRITICALSECTIONPOOL_H__
#define __CCRITICALSECTIONPOOL_H__

#include <windows.h>

class CCriticalSectionPool
{
private:
	CRITICAL_SECTION* mCriticalSectionArray;
	INT               mCriticalSectionArrayLength;
public:
	CCriticalSectionPool(INT numOfCriticalSections);
	~CCriticalSectionPool();
	void Enter(INT criticalSectionNumber);
	BOOL TryEnter(INT criticalSectionNumber);
	void Leave(INT criticalSectionNumber);
};
#endif // __CCRITICALSECTIONPOOL_H__