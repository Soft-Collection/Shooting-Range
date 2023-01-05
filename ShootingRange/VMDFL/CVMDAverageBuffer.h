#ifndef CVMDAVERAGEBUFFER_H
#define CVMDAVERAGEBUFFER_H

#include "CFFCommon.h"
#include "CVMDFrame.h"
#include <queue>

class CVMDAverageBuffer
{
private:
	enum eCriticalSections : int
	{
		Queue = 0,
		CSSize = 1 //Must be last.
	};
private:
	CCriticalSectionPool*   mCriticalSectionPool;
	std::queue<CVMDFrame*>* mQueue;
	CVMDFrame*              mSum;         //Pointer to Sum.
	int                     mDesiredCount; //Number of items that the buffer will tend to contain.
public:
	CVMDAverageBuffer();
	virtual ~CVMDAverageBuffer();
	void SetDesiredCount(int desiredCount);
	int GetDesiredCount();
	CVMDFrame* Add(CVMDFrame* vmdFrame);
};

#endif //CVMDAVERAGEBUFFER_H