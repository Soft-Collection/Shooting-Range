#ifndef __CGDIDRAW_H__
#define __CGDIDRAW_H__

//http://stackoverflow.com/questions/10715170/receiving-rtsp-stream-using-ffmpeg-library

#include "CFFCommon.h"

class CGDIDraw
{
private:
	enum eCriticalSections : int
	{
		GDIDraw = 0,
		CSSize = 1 //Must be last.
	};
private:
	HDC                     m_HDC;
	HDC                     m_MemoryDC;
	void*                   m_MemoryDC_Data_Ptr;
	BITMAPINFO*             m_bi;
	HBITMAP                 m_MemoryBitmap;
	CCriticalSectionPool*   mCriticalSectionPool;
	HWND                    mLastTargetWindow;
	int                     mLastWidth;
	int                     mLastHeight;
	BOOL                    mResourcesMustBeReallocated;
private:
	BOOL AllocateResources(CFFCommon::GDIDrawParams gdiDrawParams);
	BOOL DeallocateResources();
	BOOL AreParamsChanged(CFFCommon::GDIDrawParams gdiDrawParams);
public:
	CGDIDraw();
	~CGDIDraw();
	VOID DrawImage(HWND hwnd, BYTE* data, int width, int height);
	BOOL DrawImageOnly(CFFCommon::GDIDrawParams gdiDrawParams);
	void ReallocateResources();
};

#endif //__CGDIDRAW_H__