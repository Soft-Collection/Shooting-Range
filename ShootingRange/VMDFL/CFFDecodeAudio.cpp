#include "stdafx.h"
#include "CFFDecodeAudio.h"
#include <tchar.h>

CFFDecodeAudio::CFFDecodeAudio()
{
	mCriticalSectionPool = new CCriticalSectionPool(eCriticalSections::CSSize);
	//-------------------------------------------------------
	mCodec = NULL;
	mDecodedFrame = NULL;
	mFormatContext = NULL;
	mCodecContext = NULL;
	mLastSampleRate = 0;
	mResourcesMustBeReallocated = FALSE;
}

CFFDecodeAudio::~CFFDecodeAudio()
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

BOOL CFFDecodeAudio::AllocateResources(CFFCommon::DecodeAudioParams decodeAudioParams)
{
	try
	{
		av_register_all();
		//-------------------------------------------------------
		mCodec = avcodec_find_decoder(decodeAudioParams.CodecID);
		if (!mCodec) return(FALSE);
		//-------------------------------------------------------
		mCodecContext = avcodec_alloc_context3(mCodec);
		if (!mCodecContext) return(FALSE);
		//-------------------------------------------------------
		mFormatContext = (AVFormatContext*)decodeAudioParams.RTSPFrame.FormatContext;
		if (!mFormatContext) return(FALSE);
		//-------------------------------------------------------
		if (avcodec_open2(mCodecContext, mCodec, NULL) < 0) return(FALSE);
		//-------------------------------------------------------
		mDecodedFrame = av_frame_alloc();
	}
	catch (...)
	{
		return(FALSE);
	}
	return (TRUE);
}

BOOL CFFDecodeAudio::DeallocateResources()
{
	try
	{
		if (mDecodedFrame != NULL)
		{
			av_free(mDecodedFrame);
			mDecodedFrame = NULL;
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

BOOL CFFDecodeAudio::AreParamsChanged(CFFCommon::DecodeAudioParams decodeAudioParams)
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
			if (mLastSampleRate != mCodecContext->sample_rate)
			{
				mLastSampleRate = mCodecContext->sample_rate;
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

int CFFDecodeAudio::DecodeAudio(CFFCommon::DecodeAudioParams decodeAudioParams, CFFCommon::DecodeAudioData* decodeAudioData)
{
	int retVal = (-1);
	mCriticalSectionPool->Enter(eCriticalSections::Decode);
	try
	{
		BOOL paramsChanged = FALSE;
		if (AreParamsChanged(decodeAudioParams))
		{
			DeallocateResources();
			AllocateResources(decodeAudioParams);
			paramsChanged = TRUE;
		}
		//-------------------------------------------------------
		AVPacket packet;
		av_init_packet(&packet);
		//-------------------------------------------------------
		packet.data = decodeAudioParams.RTSPFrame.Data;
		packet.size = decodeAudioParams.RTSPFrame.Size;
		//-------------------------------------------------------
		int check = 0;
		if (mCodecContext != NULL)
		{
			if (mDecodedFrame != NULL)
			{
				retVal = avcodec_decode_audio4(mCodecContext, mDecodedFrame, &check, &packet);
				if (retVal > 0)
				{
					if (check)
					{
						decodeAudioData->Data = mDecodedFrame->extended_data;           //Pointers to the data planes/channels.
						decodeAudioData->LineSize = mDecodedFrame->linesize;            //For Audio, size in bytes of each plane.
						decodeAudioData->Channels = mCodecContext->channels;            //Number of audio channels, only used for audio.
						decodeAudioData->NbSamples = mDecodedFrame->nb_samples;         //Number of audio samples (per channel) described by this frame.
						decodeAudioData->ChannelLayout = mDecodedFrame->channel_layout; //Channel layout of the audio data. 
						decodeAudioData->SampleRate = mDecodedFrame->sample_rate;       //Sample rate of the audio data.
						decodeAudioData->SampleFmt = mCodecContext->sample_fmt;
						switch (mCodecContext->sample_fmt)
						{
							case AV_SAMPLE_FMT_U8:
								decodeAudioData->BitsPerSample = 8;
								break;
							case AV_SAMPLE_FMT_S16:
								decodeAudioData->BitsPerSample = 16;
								break;
							case AV_SAMPLE_FMT_S32:
								decodeAudioData->BitsPerSample = 32;
								break;
							case AV_SAMPLE_FMT_FLT:
								decodeAudioData->BitsPerSample = 32;
								break;
							case AV_SAMPLE_FMT_DBL:
								decodeAudioData->BitsPerSample = 64;
								break;
							case AV_SAMPLE_FMT_U8P:
								decodeAudioData->BitsPerSample = 8;
								break;
							case AV_SAMPLE_FMT_S16P:
								decodeAudioData->BitsPerSample = 16;
								break;
							case AV_SAMPLE_FMT_S32P:
								decodeAudioData->BitsPerSample = 32;
								break;
							case AV_SAMPLE_FMT_FLTP:
								decodeAudioData->BitsPerSample = 32;
								break;
							case AV_SAMPLE_FMT_DBLP:
								decodeAudioData->BitsPerSample = 64;
								break;
						}
						decodeAudioData->ParamsChanged = paramsChanged;
					}
					else
					{
						retVal = (-1);
					}
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

void CFFDecodeAudio::ReallocateResources()
{
	try
	{
		mResourcesMustBeReallocated = TRUE;
	}
	catch (...)
	{
	}
}