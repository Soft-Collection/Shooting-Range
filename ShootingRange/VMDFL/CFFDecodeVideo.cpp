#include "stdafx.h"
#include "CFFDecodeVideo.h"
#include <tchar.h>

CFFDecodeVideo::CFFDecodeVideo()
{
	mCriticalSectionPool = new CCriticalSectionPool(eCriticalSections::CSSize);
	//-------------------------------------------------------
	mCodec = NULL;
	mOpts = NULL;
	mPicture_Buf_YUV = NULL;
	mPicYUV = NULL;
	mFormatContext = NULL;
	mCodecContext = NULL;
	mLastWidth = 0;
	mLastHeight = 0;
	mResourcesMustBeReallocated = FALSE;
}

CFFDecodeVideo::~CFFDecodeVideo()
{
	mCriticalSectionPool->Enter(eCriticalSections::Decode);
	DeallocateResources();
	mCriticalSectionPool->Leave(eCriticalSections::Decode);
	//-------------------------------------------------------
	if (mCriticalSectionPool != NULL)
	{
		delete mCriticalSectionPool;
		mCriticalSectionPool = NULL;
	}
}

BOOL CFFDecodeVideo::AllocateResources(CFFCommon::DecodeVideoParams decodeVideoParams)
{
	try
	{
		av_register_all();
		//-------------------------------------------------------
		mCodec = avcodec_find_decoder(decodeVideoParams.CodecID);
		if (!mCodec) return(FALSE);
		//-------------------------------------------------------
		mCodecContext = avcodec_alloc_context3(mCodec);
		if (!mCodecContext) return(FALSE);
		if (avcodec_get_context_defaults3(mCodecContext, mCodec) != 0) return(FALSE);
		//-------------------------------------------------------
		mFormatContext = (AVFormatContext*)decodeVideoParams.RTSPFrame.FormatContext;
		if (!mFormatContext) return(FALSE);
		//-------------------------------------------------------
		for (int i = 0; i < mFormatContext->nb_streams; i++)
		{
			if (mFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
			{
				avcodec_copy_context(mCodecContext, mFormatContext->streams[i]->codec);
				break;
			}
		}
		//-------------------------------------------------------
		mOpts = NULL; // "create" an empty dictionary
		av_dict_set(&mOpts, "b", "2.5M", 0);
		if (avcodec_open2(mCodecContext, mCodec, &mOpts) < 0) return(FALSE);
		//-------------------------------------------------------
		int sizeYUV = avpicture_get_size(mCodecContext->pix_fmt, mCodecContext->width, mCodecContext->height);
		mPicture_Buf_YUV = (uint8_t*)(av_malloc(sizeYUV));
		mPicYUV = av_frame_alloc();
		avpicture_fill((AVPicture*)mPicYUV, mPicture_Buf_YUV, mCodecContext->pix_fmt, mCodecContext->width, mCodecContext->height);
	}
	catch (...)
	{
		return(FALSE);
	}
	return (TRUE);
}

BOOL CFFDecodeVideo::DeallocateResources()
{
	try
	{
		if (mPicYUV != NULL)
		{
			av_free(mPicYUV);
			mPicYUV = NULL;
		}
		if (mPicture_Buf_YUV != NULL)
		{
			av_free(mPicture_Buf_YUV);
			mPicture_Buf_YUV = NULL;
		}
		if (mCodecContext != NULL)
		{
			avcodec_close(mCodecContext);
			av_free(mCodecContext);
			mCodecContext = NULL;
		}
		if (mCodec != NULL)
		{
			mCodec = NULL;
		}
	}
	catch (...)
	{
		return(FALSE);
	}
	return (TRUE);
}

BOOL CFFDecodeVideo::AreParamsChanged(CFFCommon::DecodeVideoParams decodeVideoParams)
{
	BOOL retVal = FALSE;
	try
	{
		if (mResourcesMustBeReallocated)
		{
			mResourcesMustBeReallocated = FALSE;
			retVal = TRUE;
		}
		if (mCodecContext != NULL)
		{
			if ((mLastWidth != mCodecContext->width) || (mLastHeight != mCodecContext->height))
			{
				mLastWidth = mCodecContext->width;
				mLastHeight = mCodecContext->height;
				retVal = TRUE;
			}
		}
		else //Has never been initialized.
		{
			retVal = TRUE;
		}
	}
	catch (...)
	{
	}
	return (retVal);
}

int CFFDecodeVideo::DecodeVideo(CFFCommon::DecodeVideoParams decodeVideoParams, CFFCommon::DecodeVideoData* decodeVideoData)
{
	int retVal = (-1);
	mCriticalSectionPool->Enter(eCriticalSections::Decode);
	try
	{
		BOOL paramsChanged = FALSE;
		if (AreParamsChanged(decodeVideoParams))
		{
			DeallocateResources();
			AllocateResources(decodeVideoParams);
			paramsChanged = TRUE;
		}
		//-------------------------------------------------------
		AVPacket packet;
		av_init_packet(&packet);
		//-------------------------------------------------------
		packet.data = decodeVideoParams.RTSPFrame.Data;
		packet.size = decodeVideoParams.RTSPFrame.Size;
		//-------------------------------------------------------
		int check = 0;
		if (mCodecContext != NULL)
		{
			if (mPicYUV != NULL)
			{
				retVal = avcodec_decode_video2(mCodecContext, mPicYUV, &check, &packet);
				if (retVal > 0)
				{
					decodeVideoData->Data = mPicYUV->data;
					decodeVideoData->LineSize = mPicYUV->linesize;
					decodeVideoData->Width = mCodecContext->width;
					decodeVideoData->Height = mCodecContext->height;
					decodeVideoData->TargetPixelFormatsID = mCodecContext->pix_fmt;
					decodeVideoData->ParamsChanged = paramsChanged;
				}
			}
		}
		//-------------------------------------------------------
		av_free_packet(&packet);
	}
	catch (...)
	{
		retVal = -1;
	}
	mCriticalSectionPool->Leave(eCriticalSections::Decode);
	return (retVal);
}

void CFFDecodeVideo::ReallocateResources()
{
	try
	{
		mResourcesMustBeReallocated = TRUE;
	}
	catch (...)
	{
	}
}