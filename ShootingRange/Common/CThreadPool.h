#ifndef __CTHREADPOOL_H__
#define __CTHREADPOOL_H__

#include <windows.h>

class CThreadPool
{
private:
	HANDLE* mThreadArray;
	BOOL*   mThreadQuitArray;
	INT     mThreadArrayLength;
public:
	CThreadPool(INT numOfThreads, LPTHREAD_START_ROUTINE* callbacks, void* user);
	~CThreadPool();
	void Wait(INT threadNumber);
	BOOL IsQuit(INT threadNumber);
	void Quit(INT threadNumber);
};
#endif // __CTHREADPOOL_H__