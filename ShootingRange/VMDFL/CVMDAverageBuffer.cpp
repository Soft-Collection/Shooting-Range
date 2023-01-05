#include "stdafx.h"
#include "CVMDAverageBuffer.h"

CVMDAverageBuffer::CVMDAverageBuffer()
{ 
	mCriticalSectionPool = new CCriticalSectionPool(eCriticalSections::CSSize);
	//-------------------------------------------------------
	mCriticalSectionPool->Enter(eCriticalSections::Queue);
	mQueue = new std::queue<CVMDFrame*>();
	mCriticalSectionPool->Leave(eCriticalSections::Queue);
	//-------------------------------------------------------
	mDesiredCount = 10; 
	mSum = NULL; 
}
CVMDAverageBuffer::~CVMDAverageBuffer()
{ 
	mCriticalSectionPool->Enter(eCriticalSections::Queue);
	if (mQueue != NULL)
	{
		while (!mQueue->empty())
		{
			delete (mQueue->front());
			mQueue->pop();
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
int CVMDAverageBuffer::GetDesiredCount()
{ 
	return mDesiredCount; 
}
void CVMDAverageBuffer::SetDesiredCount(int desiredCount)
{
	mCriticalSectionPool->Enter(eCriticalSections::Queue);
	if (desiredCount < 1) mDesiredCount = 1; else mDesiredCount = desiredCount;
	mCriticalSectionPool->Leave(eCriticalSections::Queue);
}
CVMDFrame* CVMDAverageBuffer::Add(CVMDFrame* vmdFrame) //Parameter vmdFrame is a constant pointer to a Clonned VMDFrame-Class instance.
{
	CVMDFrame* retVal = NULL;
	if (mSum == NULL) mSum = vmdFrame->DeepClone(); else mSum->AddFrame(vmdFrame);
	mCriticalSectionPool->Enter(eCriticalSections::Queue);
	if (mQueue != NULL)
	{
		mQueue->push(vmdFrame->DeepClone());
		if (mQueue->size() > mDesiredCount)
		{
			while (mQueue->size() > mDesiredCount)
			{
				CVMDFrame* const temp = mQueue->front();
				mQueue->pop();
				mSum->SubFrame(temp);
				delete temp;
			}
		}
		retVal = mSum->DivFrame(mQueue->size());
	}
	mCriticalSectionPool->Leave(eCriticalSections::Queue);
	return retVal;
}