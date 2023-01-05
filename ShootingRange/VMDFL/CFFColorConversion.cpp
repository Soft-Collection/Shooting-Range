#include "stdafx.h"
#include "CFFColorConversion.h"
#include <tchar.h>

CFFColorConversion::CFFColorConversion()
{
	mCriticalSectionPool = new CCriticalSectionPool(eCriticalSections::CSSize);
	//-------------------------------------------------------
	mPicture_Buf_RGB = NULL;
	mPicRGB = NULL;
	mImg_Convert_Context = NULL;
	mLastWidth = 0;
	mLastHeight = 0;
	mLastResizedWidth = 0;
	mLastResizedHeight = 0;
	mResourcesMustBeReallocated = FALSE;
}

CFFColorConversion::~CFFColorConversion()
{
	mCriticalSectionPool->Enter(eCriticalSections::ColorConversion);
	DeallocateResources();
	mCriticalSectionPool->Leave(eCriticalSections::ColorConversion);
	//--------------------------------------------------------------
	if (mCriticalSectionPool != NULL)
	{
		delete mCriticalSectionPool;
		mCriticalSectionPool = NULL;
	}
}

BOOL CFFColorConversion::AllocateResources(CFFCommon::ColorConversionParams colorConversionParams)
{
	try
	{
		av_register_all();
		//-------------------------------------------------------
		int sizeRGB = avpicture_get_size(colorConversionParams.TargetPixelFormatsID, 
			                             colorConversionParams.DecodedVideo.Width, 
										 colorConversionParams.DecodedVideo.Height);
		//-------------------------------------------------------
		mPicture_Buf_RGB = (uint8_t*)(av_malloc(sizeRGB));
		mPicRGB = av_frame_alloc();
		//-------------------------------------------------------
		avpicture_fill((AVPicture*)mPicRGB, 
			           mPicture_Buf_RGB, 
					   colorConversionParams.TargetPixelFormatsID, 
					   colorConversionParams.ResizedWidth, 
					   colorConversionParams.ResizedHeight);
		//-------------------------------------------------------
		mImg_Convert_Context = sws_getContext(colorConversionParams.DecodedVideo.Width, 
			                                  colorConversionParams.DecodedVideo.Height, 
											  colorConversionParams.SourcePixelFormatsID, 
											  colorConversionParams.ResizedWidth, 
											  colorConversionParams.ResizedHeight, 
											  colorConversionParams.TargetPixelFormatsID, 
											  SWS_BICUBIC, NULL, NULL, NULL);
		//-------------------------------------------------------
		if (!mImg_Convert_Context) return (FALSE);
	}
	catch (...)
	{
		return (FALSE);
	}
	return (TRUE);
}

BOOL CFFColorConversion::DeallocateResources()
{
	try
	{
		if (mImg_Convert_Context != NULL)
		{
			av_free(mImg_Convert_Context);
			mImg_Convert_Context = NULL;
		}
		if (mPicRGB != NULL)
		{
			av_free(mPicRGB);
			mPicRGB = NULL;
		}
		if (mPicture_Buf_RGB != NULL)
		{
			av_free(mPicture_Buf_RGB);
			mPicture_Buf_RGB = NULL;
		}
	}
	catch (...)
	{
		return(FALSE);
	}
	return (TRUE);
}

BOOL CFFColorConversion::AreParamsChanged(CFFCommon::ColorConversionParams colorConversionParams)
{
	BOOL retVal = FALSE;
	try
	{
		if (mResourcesMustBeReallocated)
		{
			mResourcesMustBeReallocated = FALSE;
			retVal = TRUE;
		}
		if ((mLastWidth != colorConversionParams.DecodedVideo.Width)    || 
			(mLastHeight != colorConversionParams.DecodedVideo.Height)  ||
			(mLastResizedWidth != colorConversionParams.ResizedWidth)   ||
			(mLastResizedHeight != colorConversionParams.ResizedHeight))
		{
			mLastWidth = colorConversionParams.DecodedVideo.Width;
			mLastHeight = colorConversionParams.DecodedVideo.Height;
			mLastResizedWidth = colorConversionParams.ResizedWidth;
			mLastResizedHeight = colorConversionParams.ResizedHeight;
			retVal = TRUE;
		}
	}
	catch (...)
	{
	}
	return (retVal);
}

int CFFColorConversion::PerformColorConversion(CFFCommon::ColorConversionParams colorConversionParams, CFFCommon::ColorConversionData* colorConversionData)
{
	int retVal = -1;
	//For GDI Drawing, Width and Height must be Dividable by 4. 
	colorConversionParams.ResizedWidth &= 0xFFFFFFFC;  //Needed for GDI Drawing.
	colorConversionParams.ResizedHeight &= 0xFFFFFFFC; //Needed for GDI Drawing.
	mCriticalSectionPool->Enter(eCriticalSections::ColorConversion);
	try
	{
		BOOL paramsChanged = FALSE;
		if (AreParamsChanged(colorConversionParams))
		{
			DeallocateResources();
			AllocateResources(colorConversionParams);
			paramsChanged = TRUE;
		}
		//-------------------------------------------------------
		retVal = sws_scale(mImg_Convert_Context, 
			               colorConversionParams.DecodedVideo.Data, 
						   colorConversionParams.DecodedVideo.LineSize, 
						   0, 
						   colorConversionParams.DecodedVideo.Height,
						   mPicRGB->data,
						   mPicRGB->linesize);
		//-------------------------------------------------------
		if (retVal > 0)
		{
			colorConversionData->Data = mPicRGB->data;
			colorConversionData->LineSize = mPicRGB->linesize;
			colorConversionData->Width = colorConversionParams.ResizedWidth;
			colorConversionData->Height = colorConversionParams.ResizedHeight;
			colorConversionData->ParamsChanged = paramsChanged;
		}
	}
	catch (...)
	{
		retVal = -1;
	}
	mCriticalSectionPool->Leave(eCriticalSections::ColorConversion);
	return (retVal);
}

void CFFColorConversion::ReallocateResources()
{
	try
	{
		mResourcesMustBeReallocated = TRUE;
	}
	catch (...)
	{
	}
}