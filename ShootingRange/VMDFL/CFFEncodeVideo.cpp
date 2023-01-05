#include "stdafx.h"
#include "CFFEncodeVideo.h"
#include <tchar.h>

CFFEncodeVideo::CFFEncodeVideo()
{
	mCriticalSectionPool = new CCriticalSectionPool(eCriticalSections::CSSize);
	//-------------------------------------------------------
	mCodec = NULL;
	mPicYUV = NULL;
	mPacket = NULL;
	mFrameCounter = 0;
	mCodecContext = NULL;
	mLastWidth = 0;
	mLastHeight = 0;
	mResourcesMustBeReallocated = FALSE;
}

CFFEncodeVideo::~CFFEncodeVideo()
{
	mCriticalSectionPool->Enter(eCriticalSections::Encode);
	DeallocateResources();
	mCriticalSectionPool->Leave(eCriticalSections::Encode);
	//-------------------------------------------------------
	if (mCriticalSectionPool != NULL)
	{
		delete mCriticalSectionPool;
		mCriticalSectionPool = NULL;
	}
}

BOOL CFFEncodeVideo::AllocateResources(CFFCommon::EncodeVideoParams encodeVideoParams)
{
	try
	{
		av_register_all();
		//-------------------------------------------------------
		mCodec = avcodec_find_encoder(encodeVideoParams.CodecID);
		if (!mCodec) return(FALSE);
		//-------------------------------------------------------
		mCodecContext = avcodec_alloc_context3(mCodec);
		if (!mCodecContext) return(FALSE);
		//-------------------------------------------------------
		/* put sample parameters */
		mCodecContext->bit_rate = encodeVideoParams.BitRate;
		/* resolution must be a multiple of two */
		mCodecContext->width = encodeVideoParams.Width;
		mCodecContext->height = encodeVideoParams.Height;
		/* frames per second */
		mCodecContext->time_base.num = encodeVideoParams.TimeBaseNum;
		mCodecContext->time_base.den = encodeVideoParams.TimeBaseDen;
		/* emit one intra frame every ten frames
		* check frame pict_type before passing frame
		* to encoder, if frame->pict_type is AV_PICTURE_TYPE_I
		* then gop_size is ignored and the output of encoder
		* will always be I frame irrespective to gop_size
		*/
		mCodecContext->gop_size = encodeVideoParams.GopSize;
		mCodecContext->max_b_frames = encodeVideoParams.MaxBFrames;
		mCodecContext->pix_fmt = encodeVideoParams.SourcePixelFormatsID;
		//-------------------------------------------------------
		if (encodeVideoParams.CodecID == AV_CODEC_ID_H264) av_opt_set(mCodecContext->priv_data, "preset", "slow", 0);
		//-------------------------------------------------------
		/* open it */
		if (avcodec_open2(mCodecContext, mCodec, NULL) < 0) return(FALSE);
		//-------------------------------------------------------
		mPicYUV = av_frame_alloc();
		if (!mPicYUV) return(FALSE);
		//-------------------------------------------------------
		mPicYUV->format = mCodecContext->pix_fmt;
		mPicYUV->width = mCodecContext->width;
		mPicYUV->height = mCodecContext->height;
		//-------------------------------------------------------
		/* the image can be allocated by any means and av_image_alloc() is
		* just the most convenient way if av_malloc() is to be used */
		if (av_image_alloc(mPicYUV->data, mPicYUV->linesize, mCodecContext->width, mCodecContext->height, mCodecContext->pix_fmt, 32) < 0) return(FALSE);
		//-------------------------------------------------------
		mPacket = new AVPacket();
		av_init_packet(mPacket);
	}
	catch (...)
	{
		return(FALSE);
	}
	return (TRUE);
}

BOOL CFFEncodeVideo::DeallocateResources()
{
	try
	{
		if (mPacket != NULL)
		{
			av_packet_unref(mPacket);
			av_free_packet(mPacket);
			mPacket = NULL;
		}
		if (mPicYUV != NULL)
		{
			av_freep(&mPicYUV->data[0]);
			av_free(mPicYUV);
			mPicYUV = NULL;
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

BOOL CFFEncodeVideo::AreParamsChanged(CFFCommon::EncodeVideoParams encodeVideoParams)
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

int CFFEncodeVideo::EncodeVideo(CFFCommon::EncodeVideoParams encodeVideoParams, CFFCommon::EncodeVideoData* encodeVideoData)
{
	int retVal = (-1);
	mCriticalSectionPool->Enter(eCriticalSections::Encode);
	try
	{
		BOOL paramsChanged = FALSE;
		if (AreParamsChanged(encodeVideoParams))
		{
			DeallocateResources();
			AllocateResources(encodeVideoParams);
			paramsChanged = TRUE;
		}
		//-------------------------------------------------------
		int got_output;
		if (mPacket != NULL)
		{
			av_packet_unref(mPacket);
			mPacket->data = NULL; // packet data will be allocated by the encoder
			mPacket->size = 0;
			if (mCodecContext != NULL)
			{
				if (mPicYUV != NULL)
				{
					mPicYUV->pts = mFrameCounter;
					retVal = avcodec_encode_video2(mCodecContext, mPacket, mPicYUV, &got_output);
					if (got_output)
					{
						encodeVideoData->Data = &mPacket->data;
						encodeVideoData->Size = mPacket->size;
						encodeVideoData->ParamsChanged = paramsChanged;
					}
					mFrameCounter++;
				}
			}
		}
	}
	catch (...)
	{
		retVal = -1;
	}
	mCriticalSectionPool->Leave(eCriticalSections::Encode);
	return (retVal);
}

void CFFEncodeVideo::ReallocateResources()
{
	try
	{
		mResourcesMustBeReallocated = TRUE;
	}
	catch (...)
	{
	}
}