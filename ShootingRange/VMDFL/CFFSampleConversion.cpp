#include "stdafx.h"
#include "CFFSampleConversion.h"
#include <tchar.h>

CFFSampleConversion::CFFSampleConversion()
{
	mCriticalSectionPool = new CCriticalSectionPool(eCriticalSections::CSSize);
	//-------------------------------------------------------
	mSwrContext = NULL;
	mOutputBuffer = NULL;
	mOutputBufferLineSize = 0;
	memset(&mLastSource, 0, sizeof(mLastSource));
}

CFFSampleConversion::~CFFSampleConversion()
{
	mCriticalSectionPool->Enter(eCriticalSections::SampleConversion);
	DeallocateResources();
	mCriticalSectionPool->Leave(eCriticalSections::SampleConversion);
	//--------------------------------------------------------------
	if (mCriticalSectionPool != NULL)
	{
		delete mCriticalSectionPool;
		mCriticalSectionPool = NULL;
	}
}

BOOL CFFSampleConversion::AllocateResources(CFFCommon::SampleConversionParams sampleConversionParams)
{
	try
	{
		av_register_all();
		//-------------------------------------------------------
		// Set up SWR context once you've got codec information
		mSwrContext = swr_alloc();
		if (!mSwrContext) return (FALSE);
		av_opt_set_int(mSwrContext, "in_channel_layout", sampleConversionParams.Source.ChannelLayout, 0);
		av_opt_set_int(mSwrContext, "out_channel_layout", sampleConversionParams.Target.ChannelLayout, 0);
		av_opt_set_int(mSwrContext, "in_sample_rate", sampleConversionParams.Source.SampleRate, 0);
		av_opt_set_int(mSwrContext, "out_sample_rate", sampleConversionParams.Target.SampleRate, 0);
		av_opt_set_sample_fmt(mSwrContext, "in_sample_fmt", sampleConversionParams.Source.SampleFormatsID, 0);
		av_opt_set_sample_fmt(mSwrContext, "out_sample_fmt", sampleConversionParams.Target.SampleFormatsID, 0);
		if (swr_init(mSwrContext) < 0) return(FALSE);
		//-------------------------------------------------------
		if (av_samples_alloc(&mOutputBuffer, 
			                 &mOutputBufferLineSize, 
						     sampleConversionParams.DecodedAudio.Channels, 
						     sampleConversionParams.DecodedAudio.NbSamples, 
						     sampleConversionParams.Target.SampleFormatsID, 
						     0) < 0) return(FALSE);
	}
	catch (...)
	{
		return (FALSE);
	}
	return (TRUE);
}

BOOL CFFSampleConversion::DeallocateResources()
{
	try
	{
		if (mSwrContext != NULL)
		{
			swr_free(&mSwrContext);
			mSwrContext = NULL;
		}
		if (mOutputBuffer != NULL)
		{
			av_freep(&mOutputBuffer[0]);
			mOutputBuffer = NULL;
		}
	}
	catch (...)
	{
		return(FALSE);
	}
	return (TRUE);
}

BOOL CFFSampleConversion::AreParamsChanged(CFFCommon::SampleConversionParams sampleConversionParams)
{
	BOOL retVal = FALSE;
	try
	{
		if (mResourcesMustBeReallocated)
		{
			mResourcesMustBeReallocated = FALSE;
			retVal = TRUE;
		}
		if ((mLastSource.ChannelLayout != sampleConversionParams.Source.ChannelLayout) ||
			(mLastSource.SampleRate != sampleConversionParams.Source.SampleRate) ||
			(mLastSource.SampleFormatsID != sampleConversionParams.Source.SampleFormatsID))
		{
			mLastSource.ChannelLayout = sampleConversionParams.Source.ChannelLayout;
			mLastSource.SampleRate = sampleConversionParams.Source.SampleRate;
			mLastSource.SampleFormatsID = sampleConversionParams.Source.SampleFormatsID;
			retVal = TRUE;
		}
	}
	catch (...)
	{
	}
	return (retVal);
}

int CFFSampleConversion::PerformSampleConversion(CFFCommon::SampleConversionParams sampleConversionParams, CFFCommon::SampleConversionData* sampleConversionData)
{
	int retVal = -1;
	mCriticalSectionPool->Enter(eCriticalSections::SampleConversion);
	try
	{
		BOOL paramsChanged = FALSE;
		if (AreParamsChanged(sampleConversionParams))
		{
			DeallocateResources();
			AllocateResources(sampleConversionParams);
			paramsChanged = TRUE;
		}
		//-------------------------------------------------------
		retVal = swr_convert(mSwrContext,
			                 &mOutputBuffer, 
							 sampleConversionParams.DecodedAudio.NbSamples,
							 (const uint8_t**)sampleConversionParams.DecodedAudio.Data,
							 sampleConversionParams.DecodedAudio.NbSamples);
		//-------------------------------------------------------
		if (retVal > 0)
		{
			sampleConversionData->Data = &mOutputBuffer;
			sampleConversionData->LineSize = &mOutputBufferLineSize;
			sampleConversionData->SamplesPerSec = sampleConversionParams.DecodedAudio.SampleRate;
			switch (sampleConversionParams.Target.SampleFormatsID)
			{
			case AV_SAMPLE_FMT_U8:
				sampleConversionData->BitsPerSample = 8;
				break;
			case AV_SAMPLE_FMT_S16:
				sampleConversionData->BitsPerSample = 16;
				break;
			case AV_SAMPLE_FMT_S32:
				sampleConversionData->BitsPerSample = 32;
				break;
			case AV_SAMPLE_FMT_FLT:
				sampleConversionData->BitsPerSample = 32;
				break;
			case AV_SAMPLE_FMT_DBL:
				sampleConversionData->BitsPerSample = 64;
				break;
			case AV_SAMPLE_FMT_U8P:
				sampleConversionData->BitsPerSample = 8;
				break;
			case AV_SAMPLE_FMT_S16P:
				sampleConversionData->BitsPerSample = 16;
				break;
			case AV_SAMPLE_FMT_S32P:
				sampleConversionData->BitsPerSample = 32;
				break;
			case AV_SAMPLE_FMT_FLTP:
				sampleConversionData->BitsPerSample = 32;
				break;
			case AV_SAMPLE_FMT_DBLP:
				sampleConversionData->BitsPerSample = 64;
				break;
			}
			sampleConversionData->Channels = sampleConversionParams.DecodedAudio.Channels;
			sampleConversionData->ParamsChanged = paramsChanged;
		}
	}
	catch (...)
	{
		retVal = -1;
	}
	mCriticalSectionPool->Leave(eCriticalSections::SampleConversion);
	return (retVal);
}

void CFFSampleConversion::ReallocateResources()
{
	try
	{
		mResourcesMustBeReallocated = TRUE;
	}
	catch (...)
	{
	}
}