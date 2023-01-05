#include "stdafx.h"
#include "CFFMpegRtsp.h"
#include <tchar.h>

BOOL CFFMpegRtsp::mStaticInitialized = FALSE;

CFFMpegRtsp::CFFMpegRtsp(dOnFrameReceived onFrameReceived, void* user)
{
	mCriticalSectionPool = new CCriticalSectionPool(eCriticalSections::CSSize);
	//-------------------------------------------------------
	mOnFrameReceived = onFrameReceived;
	mUser = user;
	//-------------------------------------------------------
	mRTSPString = NULL;
	mIsWebCam = FALSE;
	mFormatContext = NULL;
	mIsConnected = FALSE;
	SetConnectionState(CFFCommon::eConnectionStates::NotSetYet);
	mLastFrameTime = 0;
	mBeginConnectingTime = 0;
	//-------------------------------------------------------
	InitStatic();
	//-------------------------------------------------------
	av_register_all();
	avformat_network_init();
	avdevice_register_all();
	//-------------------------------------------------------
	PTHREAD_START_ROUTINE callbacks[] = { MyThreadRTSPFunctionStatic, MyThreadStatusFunctionStatic };
	mThreadPool = new CThreadPool(eThreads::TRSize, callbacks, this);
}

CFFMpegRtsp::~CFFMpegRtsp()
{
	Disconnect();
	if (mThreadPool != NULL)
	{
		delete mThreadPool;
		mThreadPool = NULL;
	}
	if (mCriticalSectionPool != NULL)
	{
		delete mCriticalSectionPool;
		mCriticalSectionPool = NULL;
	}
}

void CFFMpegRtsp::InitStatic()
{
	if (!mStaticInitialized)
	{
		//---------------------------------
		av_log_set_callback(MyLogCallbackFunctionStatic);
		//---------------------------------
		mStaticInitialized = TRUE;
	}
}

void CFFMpegRtsp::Connect(LPTSTR rtspString, BOOL isWebCam)
{
	mCriticalSectionPool->Enter(eCriticalSections::CSRTSP);
	//--------------------------------------------------------------
	SetConnectionState(CFFCommon::eConnectionStates::Connecting);
	mBeginConnectingTime = GetTickCount();
	mRTSPString = new CHAR[256];
	memset(mRTSPString, 0, 256 * sizeof(CHAR));
	_tcscpy(mRTSPString, rtspString);
	mIsWebCam = isWebCam;
	mFormatContext = avformat_alloc_context();
	int res = (-1);
	if (isWebCam)
	{
		res = avformat_open_input(&mFormatContext, rtspString, av_find_input_format("vfwcap"), NULL);
	}
	else
	{
		res = avformat_open_input(&mFormatContext, rtspString, NULL, NULL);
	}
	if (res == 0)
	{
		if (avformat_find_stream_info(mFormatContext, NULL) >= 0)
		{
			mIsConnected = TRUE;
		}
		else
		{
			SetConnectionState(CFFCommon::eConnectionStates::UnableToConnect);
		}
	}
	else
	{
		SetConnectionState(CFFCommon::eConnectionStates::UnableToConnect);
	}
	//--------------------------------------------------------------
	mCriticalSectionPool->Leave(eCriticalSections::CSRTSP);
}

void CFFMpegRtsp::Disconnect()
{
	mIsConnected = FALSE;
	//--------------------------------------------------------------
	mCriticalSectionPool->Enter(eCriticalSections::CSRTSP);
	if (mFormatContext != NULL)
	{
		av_read_pause(mFormatContext);
		avformat_close_input(&mFormatContext);
		avformat_free_context(mFormatContext);
		mFormatContext = NULL;
		SetConnectionState(CFFCommon::eConnectionStates::NotConnected);
	}
	if (mRTSPString != NULL)
	{
		delete[] mRTSPString;
		mRTSPString = NULL;
	}
	mCriticalSectionPool->Leave(eCriticalSections::CSRTSP);
}

void CFFMpegRtsp::Reconnect()
{
	if (mIsConnected) Disconnect();
	Connect(mRTSPString, mIsWebCam);
}

BOOL CFFMpegRtsp::IsConnected()
{
	return mIsConnected;
}

void CFFMpegRtsp::Play()
{
	mCriticalSectionPool->Enter(eCriticalSections::CSRTSP);
	if (mIsConnected)
	{
		if (mFormatContext != NULL)
		{
			//start reading packets from stream.
			av_read_play(mFormatContext); //play RTSP
		}
	}
	mCriticalSectionPool->Leave(eCriticalSections::CSRTSP);
}

void CFFMpegRtsp::Pause()
{
	mCriticalSectionPool->Enter(eCriticalSections::CSRTSP);
	if (mIsConnected)
	{
		if (mFormatContext != NULL)
		{
			//pause reading packets from stream.
			av_read_pause(mFormatContext); //pause RTSP
		}
	}
	mCriticalSectionPool->Leave(eCriticalSections::CSRTSP);
}

CFFCommon::eConnectionStates CFFMpegRtsp::GetConnectionState()
{
	CFFCommon::eConnectionStates retVal = CFFCommon::eConnectionStates::NotSetYet;
	mCriticalSectionPool->Enter(eCriticalSections::CSStatus);
	retVal = mConnectionState;
	mCriticalSectionPool->Leave(eCriticalSections::CSStatus);
	return retVal;
}

void CFFMpegRtsp::SetConnectionState(CFFCommon::eConnectionStates connectionState)
{
	mCriticalSectionPool->Enter(eCriticalSections::CSStatus);
	mConnectionState = connectionState;
	mCriticalSectionPool->Leave(eCriticalSections::CSStatus);
}

DWORD WINAPI CFFMpegRtsp::MyThreadRTSPFunctionStatic(LPVOID lpParam)
{
	try
	{
		CFFMpegRtsp* ffMpegRtsp = (CFFMpegRtsp*)lpParam;
		if (ffMpegRtsp != NULL)
		{
			ffMpegRtsp->MyThreadRTSPFunction();
		}
	}
	catch (...)
	{
	}
	return 0;
}

void CFFMpegRtsp::MyThreadRTSPFunction()
{
	while (mThreadPool == NULL);
	while (!mThreadPool->IsQuit(eThreads::TRRTSP))
	{
		try
		{
			mCriticalSectionPool->Enter(eCriticalSections::CSRTSP);
			//----------------------------------------------------------------
			if (mIsConnected)
			{
				AVPacket packet;
				av_init_packet(&packet);
				//------------------------------------------------------------
				while ((av_read_frame(mFormatContext, &packet) >= 0) && (mIsConnected))
				{
					SetConnectionState(CFFCommon::eConnectionStates::Connected);
					mLastFrameTime = GetTickCount();
					if (mOnFrameReceived != NULL)
					{
						AVMediaType mediaType = (AVMediaType)mFormatContext->streams[packet.stream_index]->codec->codec_type;
						AVCodecID codecID = (AVCodecID)mFormatContext->streams[packet.stream_index]->codec->codec_id;
						CFFCommon::RTSPFrameData rtspFrameData;
						rtspFrameData.Data = packet.data;
						rtspFrameData.Size = packet.size;
						rtspFrameData.MediaType = mediaType;
						rtspFrameData.CodecID = codecID;
						rtspFrameData.FormatContext = mFormatContext;
						(*(mOnFrameReceived))(mUser, rtspFrameData);
					}
					av_free_packet(&packet);
					av_init_packet(&packet);
				}
				//------------------------------------------------------------
				av_free_packet(&packet);
			}
			//----------------------------------------------------------------
			mCriticalSectionPool->Leave(eCriticalSections::CSRTSP);
			Sleep(20);
		}
		catch (...)
		{
		}
	}
}

DWORD WINAPI CFFMpegRtsp::MyThreadStatusFunctionStatic(LPVOID lpParam)
{
	try
	{
		CFFMpegRtsp* ffMpegRtsp = (CFFMpegRtsp*)lpParam;
		if (ffMpegRtsp != NULL)
		{
			ffMpegRtsp->MyThreadStatusFunction();
		}
	}
	catch (...)
	{
	}
	return 0;
}

void CFFMpegRtsp::MyThreadStatusFunction()
{
	while (mThreadPool == NULL);
	while (!mThreadPool->IsQuit(eThreads::TRStatus))
	{
		try
		{
			mCriticalSectionPool->Enter(eCriticalSections::CSStatus);
			if (mConnectionState == CFFCommon::eConnectionStates::Connected)
			{
				if (GetTickCount() - mLastFrameTime > 10000)
				{
					SetConnectionState(CFFCommon::eConnectionStates::NotConnected);
				}
			}
			if (mConnectionState == CFFCommon::eConnectionStates::Connecting)
			{
				if (GetTickCount() - mBeginConnectingTime > 20000)
				{
					SetConnectionState(CFFCommon::eConnectionStates::NotConnected);
				}
			}
			mCriticalSectionPool->Leave(eCriticalSections::CSStatus);
			Sleep(100);
		}
		catch (...)
		{
		}
	}
}

void CFFMpegRtsp::MyLogCallbackFunctionStatic(void *ptr, int level, const char *fmt, va_list vl)
{
    va_list vl2;
    char line[1024];
    int print_prefix = 1;
	int report_file_level = AV_LOG_DEBUG;
	try
	{
		va_copy(vl2, vl);
		av_log_format_line(ptr, level, fmt, vl2, line, sizeof(line), &print_prefix);
		va_end(vl2);
		if (report_file_level >= level) 
		{
			OutputDebugString(line);
		}
	}
	catch (...)
	{
	}
}