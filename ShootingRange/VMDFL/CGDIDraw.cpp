#include "stdafx.h"
#include "CGDIDraw.h"
#include <tchar.h>

CGDIDraw::CGDIDraw()
{
	mCriticalSectionPool = new CCriticalSectionPool(eCriticalSections::CSSize);
	//-------------------------------------------------------
	m_HDC = NULL;
	m_MemoryDC = NULL;
	m_MemoryDC_Data_Ptr = NULL;
	m_bi = NULL;
	m_MemoryBitmap = NULL;
	mLastTargetWindow = NULL;
	mLastWidth = 0;
	mLastHeight = 0;
	mResourcesMustBeReallocated = FALSE;
}

CGDIDraw::~CGDIDraw()
{
	mCriticalSectionPool->Enter(eCriticalSections::GDIDraw);
	DeallocateResources();
	mCriticalSectionPool->Leave(eCriticalSections::GDIDraw);
	//--------------------------------------------------------------
	if (mCriticalSectionPool != NULL)
	{
		delete mCriticalSectionPool;
		mCriticalSectionPool = NULL;
	}
}

BOOL CGDIDraw::AllocateResources(CFFCommon::GDIDrawParams gdiDrawParams)
{
	try
	{
		if (gdiDrawParams.Window == NULL) return (FALSE);
		m_HDC = GetDC(gdiDrawParams.Window);
		if (m_HDC == NULL) return (FALSE);
		m_MemoryDC = CreateCompatibleDC(m_HDC);
		if (m_MemoryDC == NULL) return (FALSE);
		//--------------------------------------------------------
		m_bi = new BITMAPINFO;
		ZeroMemory(m_bi, sizeof(BITMAPINFO));
		m_bi->bmiHeader.biBitCount = 24;
		m_bi->bmiHeader.biCompression = BI_RGB;
		m_bi->bmiHeader.biPlanes = 1;
		m_bi->bmiHeader.biSize = 40;
		m_bi->bmiHeader.biWidth = gdiDrawParams.ColorConversionWidth;
		m_bi->bmiHeader.biHeight = -gdiDrawParams.ColorConversionHeight;
		m_bi->bmiHeader.biSizeImage = gdiDrawParams.ColorConversionWidth * gdiDrawParams.ColorConversionHeight * (m_bi->bmiHeader.biBitCount / 8);
		//--------------------------------------------------------
		m_MemoryBitmap = CreateDIBSection(m_MemoryDC, m_bi, DIB_RGB_COLORS, &m_MemoryDC_Data_Ptr, NULL, 0);
		SelectObject(m_MemoryDC, m_MemoryBitmap);
	}
	catch (...)
	{
		return (FALSE);
	}
	return (TRUE);
}

BOOL CGDIDraw::DeallocateResources()
{
	try
	{
		DeleteObject(m_MemoryBitmap);
		//--------------------------------------------------------
		if (m_bi != NULL)
		{
			delete m_bi;
			m_bi = NULL;
		}
		//--------------------------------------------------------
		ReleaseDC(mLastTargetWindow, m_MemoryDC);
		ReleaseDC(mLastTargetWindow, m_HDC);
	}
	catch (...)
	{
		return(FALSE);
	}
	return (TRUE);
}

BOOL CGDIDraw::AreParamsChanged(CFFCommon::GDIDrawParams gdiDrawParams)
{
	BOOL retVal = FALSE;
	try
	{
		if (mResourcesMustBeReallocated)
		{
			mResourcesMustBeReallocated = FALSE;
			retVal = TRUE;
		}
		if ((mLastWidth != gdiDrawParams.ColorConversionWidth) ||
			(mLastHeight != gdiDrawParams.ColorConversionHeight) ||
			(mLastTargetWindow != gdiDrawParams.Window))
		{
			mLastWidth = gdiDrawParams.ColorConversionWidth;
			mLastHeight = gdiDrawParams.ColorConversionHeight;
			mLastTargetWindow = gdiDrawParams.Window;
			retVal = TRUE;
		}
	}
	catch (...)
	{
	}
	return (retVal);
}

VOID CGDIDraw::DrawImage(HWND hwnd, BYTE* data, int width, int height)
{
	try
	{
		RECT rect;
		GetWindowRect(hwnd, &rect);
		SetStretchBltMode(m_HDC, COLORONCOLOR);
		if (m_MemoryDC_Data_Ptr != 0)
		{
			memcpy(m_MemoryDC_Data_Ptr, data, m_bi->bmiHeader.biSizeImage);
		}
		StretchBlt(m_HDC, 0, 0, rect.right - rect.left, rect.bottom - rect.top, m_MemoryDC, 0, 0, width, height, SRCCOPY);
	}
	catch (...)
	{
	}
}

BOOL CGDIDraw::DrawImageOnly(CFFCommon::GDIDrawParams gdiDrawParams)
{
	BOOL retVal = TRUE;
	mCriticalSectionPool->Enter(eCriticalSections::GDIDraw);
	try
	{
		if (AreParamsChanged(gdiDrawParams))
		{
			DeallocateResources();
			AllocateResources(gdiDrawParams);
		}
		//-------------------------------------------------------
		DrawImage(gdiDrawParams.Window, gdiDrawParams.Data, gdiDrawParams.ColorConversionWidth, gdiDrawParams.ColorConversionHeight);
		//-------------------------------------------------------
	}
	catch (...)
	{
		retVal = FALSE;
	}
	mCriticalSectionPool->Leave(eCriticalSections::GDIDraw);
	return (retVal);
}

void CGDIDraw::ReallocateResources()
{
	try
	{
		mResourcesMustBeReallocated = TRUE;
	}
	catch (...)
	{
	}
}