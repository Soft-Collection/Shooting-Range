#include "stdafx.h"
#include "CDevice.h"
#include "../Common/CExceptionReport.h"

CDevice::CDevice()
{
	try
	{
		mEventShot = NULL;
		//----------------------------------------
		mCriticalSection = NULL;
		//----------------------------------------
		mShootingRangeInfo = NULL;
		//----------------------------------------
		mRGBPicture = NULL;
		mRGBPictureWidth = 0;
		mRGBPictureHeight = 0;
		//----------------------------------------
		mFFMpegRtsp = NULL;
		mBuffer = NULL;
		mFFDecodeVideo = NULL;
		mFFDecodeAudio = NULL;
		mFFSampleConversion = NULL;
		mWavePlaying = NULL;
		mFFColorConversion = NULL;
		mFilter = NULL;
		mGDIDrawTarget = NULL;
		mInstantMotionAccepted = NULL;
		mInstantMotionAcceptedAveragePoint = NULL;
		mAverageBuffer = NULL;
		mLastResizeSquare = 0;
		mLastShotTimeInMS = 0;
		memset(&mLastShotCoordinates, 0, sizeof(POINTF));
		memset(mLastShotBorder, 0, 4 * sizeof(POINTF));
		mEventShot = NULL;
		mAverageBufferMustBeReallocated = FALSE;
		//========================================
		mEventShot = CreateEvent(NULL, FALSE, FALSE, 0);
		//----------------------------------------
		mCriticalSection = new CRITICAL_SECTION;
		InitializeCriticalSection(mCriticalSection);
		//----------------------------------------
		mShootingRangeInfo = new CFFCommon::ShootingRangeInfo;
		memset(mShootingRangeInfo, 0, sizeof(CFFCommon::ShootingRangeInfo));
		//----------------------------------------
		mRGBPicture = new BYTE[1920 * 1080 * 3];
		//----------------------------------------
		mGDIDrawTarget = new CGDIDraw();
		mFilter = new CFilter();
		mInstantMotionAccepted = new RECTANGLEF[5000];
		mInstantMotionAcceptedAveragePoint = new POINTF[5000];
		mAverageBuffer = new CVMDAverageBuffer;
		mFFColorConversion = new CFFColorConversion();
		mFFDecodeVideo = new CFFDecodeVideo();
		mFFDecodeAudio = new CFFDecodeAudio();
		mFFSampleConversion = new CFFSampleConversion();
		mBuffer = new CBuffer(OnFrameReceivedFromBufferStatic, this);
		mFFMpegRtsp = new CFFMpegRtsp(OnFrameReceivedFromRTSPStatic, this);
	}
	catch (...)
	{
		CExceptionReport::WriteExceptionReportToFile("CDeviceManager::CDeviceManager", "Exception in CDeviceManager Constructor");
	}
}
CDevice::~CDevice()
{
	try
	{
		if (mFFMpegRtsp != NULL)
		{
			delete mFFMpegRtsp;
			mFFMpegRtsp = NULL;
		}
		if (mBuffer != NULL)
		{
			delete mBuffer;
			mBuffer = NULL;
		}
		if (mWavePlaying != NULL)
		{
			delete mWavePlaying;
			mWavePlaying = NULL;
		}
		if (mFFSampleConversion != NULL)
		{
			delete mFFSampleConversion;
			mFFSampleConversion = NULL;
		}
		if (mFFDecodeAudio != NULL)
		{
			delete mFFDecodeAudio;
			mFFDecodeAudio = NULL;
		}
		if (mFFDecodeVideo != NULL)
		{
			delete mFFDecodeVideo;
			mFFDecodeVideo = NULL;
		}
		if (mFFColorConversion != NULL)
		{
			delete mFFColorConversion;
			mFFColorConversion = NULL;
		}
		if (mInstantMotionAccepted != NULL)
		{
			delete[] mInstantMotionAccepted;
			mInstantMotionAccepted = NULL;
		}
		if (mInstantMotionAcceptedAveragePoint != NULL)
		{
			delete[] mInstantMotionAcceptedAveragePoint;
			mInstantMotionAcceptedAveragePoint = NULL;
		}
		if (mAverageBuffer != NULL)
		{
			delete mAverageBuffer;
			mAverageBuffer = NULL;
		}
		if (mFilter != NULL)
		{
			delete mFilter;
			mFilter = NULL;
		}
		if (mGDIDrawTarget != NULL)
		{
			delete mGDIDrawTarget;
			mGDIDrawTarget = NULL;
		}
		if (mRGBPicture != NULL)
		{
			delete[] mRGBPicture;
			mRGBPicture = NULL;
		}
		if (mShootingRangeInfo != NULL)
		{
			delete mShootingRangeInfo;
			mShootingRangeInfo = NULL;
		}
		//----------------------------------------
		if (mCriticalSection != NULL)
		{
		  	DeleteCriticalSection(mCriticalSection);
			mCriticalSection = NULL;
		}
		//----------------------------------------
		if (mEventShot != NULL)
		{
			CloseHandle(mEventShot);
			mEventShot = NULL;
		}
	}
	catch (...)
	{
		CExceptionReport::WriteExceptionReportToFile("CDeviceManager::~CDeviceManager", "Exception in CDeviceManager Destructor");
	}
}
void CDevice::Connect()
{
	try
	{
		mAverageBufferMustBeReallocated = TRUE;
		if (mFFColorConversion != NULL) mFFColorConversion->ReallocateResources();
		if (mFFDecodeVideo != NULL) mFFDecodeVideo->ReallocateResources();
		if (mFFDecodeAudio != NULL) mFFDecodeAudio->ReallocateResources();
		if (mFFSampleConversion != NULL) mFFSampleConversion->ReallocateResources();
		if (mFFMpegRtsp != NULL) mFFMpegRtsp->Connect(mShootingRangeInfo->Connection.URL, mShootingRangeInfo->Connection.IsWebCam);
	}
	catch (...)
	{

	}
}
void CDevice::Disconnect()
{
	try
	{
		if (mFFMpegRtsp != NULL) mFFMpegRtsp->Disconnect();
	}
	catch (...)
	{

	}
}
CFFCommon::eConnectionStates CDevice::GetConnectionState()
{
	CFFCommon::eConnectionStates retVal;
	try
	{
		if (mFFMpegRtsp != NULL) retVal = mFFMpegRtsp->GetConnectionState();
	}
	catch (...)
	{
		CExceptionReport::WriteExceptionReportToFile("CDeviceManager::OnFrameReceived", "Exception in CDeviceManager::GetConnectionState");
	}
	return retVal;
}
void CDevice::GetShotData(POINTF* shotCoordinates, POINTF* shotBorder)
{
	try
	{
		DWORD dwEventObject = WaitForSingleObject(mEventShot, INFINITE);
		memcpy(shotCoordinates, &mLastShotCoordinates, sizeof(POINTF));
		memcpy(shotBorder, mLastShotBorder, 4 * sizeof(POINTF));
	}
	catch (...)
	{
		CExceptionReport::WriteExceptionReportToFile("CDeviceManager::OnFrameReceived", "Exception in CDeviceManager::GetConnectionState");
	}
}
void CDevice::RGBPictureLock()
{
	try
	{
		EnterCriticalSection(mCriticalSection);
	}
	catch (...)
	{
		CExceptionReport::WriteExceptionReportToFile("CDeviceManager::OnFrameReceived", "Exception in CDeviceManager::GetConnectionState");
	}
}
void CDevice::RGBPictureUnlock()
{
	try
	{
		LeaveCriticalSection(mCriticalSection);
	}
	catch (...)
	{
		CExceptionReport::WriteExceptionReportToFile("CDeviceManager::OnFrameReceived", "Exception in CDeviceManager::GetConnectionState");
	}
}
void CDevice::GetRGBPictureSize(int* width, int* height)
{
	try
	{
		*width =  mRGBPictureWidth;
		*height = mRGBPictureHeight;
	}
	catch (...)
	{
		CExceptionReport::WriteExceptionReportToFile("CDeviceManager::OnFrameReceived", "Exception in CDeviceManager::GetConnectionState");
	}
}
void CDevice::GetRGBPictureData(BYTE* data)
{
	try
	{
		memcpy(data, mRGBPicture, mRGBPictureWidth * mRGBPictureHeight * 3);
	}
	catch (...)
	{
		CExceptionReport::WriteExceptionReportToFile("CDeviceManager::OnFrameReceived", "Exception in CDeviceManager::GetConnectionState");
	}
}
void CDevice::SetShootingRangeInfo(CFFCommon::eShootingRangeInfoDataTypes shootingRangeInfoDataType, int val)
{
	switch (shootingRangeInfoDataType)
	{
		case CFFCommon::eShootingRangeInfoDataTypes::VMD_AverageBufferSizeInFrames:
			mShootingRangeInfo->VMD.AverageBufferSizeInFrames = val;
			break;
		case CFFCommon::eShootingRangeInfoDataTypes::VMD_ResizeSquare:
			mShootingRangeInfo->VMD.ResizeSquare = val;
			break;
		case CFFCommon::eShootingRangeInfoDataTypes::VMD_Tolerance:
			mShootingRangeInfo->VMD.Tolerance = val;
			break;
		case CFFCommon::eShootingRangeInfoDataTypes::VMD_MedianSquare:
			mShootingRangeInfo->VMD.MedianSquare = val;
			break;
		case CFFCommon::eShootingRangeInfoDataTypes::VMD_MinimalDistanceBetweenTwoObjects:
			mShootingRangeInfo->VMD.MinimalDistanceBetweenTwoObjects = val;
			break;
		case CFFCommon::eShootingRangeInfoDataTypes::VMD_MaximalAmountOfObjectsToFind:
			mShootingRangeInfo->VMD.MaximalAmountOfObjectsToFind = val;
			break;
		case CFFCommon::eShootingRangeInfoDataTypes::VMD_WhatToShowType:
			mShootingRangeInfo->VMD.WhatToShowType = (CFFCommon::eWhatToShowTypes)val;
			break;
		case CFFCommon::eShootingRangeInfoDataTypes::VMD_IdleTimeAfterShotInMS:
			mShootingRangeInfo->VMD.IdleTimeAfterShotInMS = val;
			break;
		case CFFCommon::eShootingRangeInfoDataTypes::Connection_IsWebCam:
			mShootingRangeInfo->Connection.IsWebCam = val;
			break;
	}
}
void CDevice::SetShootingRangeInfo(CFFCommon::eShootingRangeInfoDataTypes shootingRangeInfoDataType, float val)
{
	switch (shootingRangeInfoDataType)
	{
		case CFFCommon::eShootingRangeInfoDataTypes::VMD_AcceptedObjectLimit_MinWidth:
			mShootingRangeInfo->VMD.AcceptedObjectLimit.MinWidth = val;
			break;
		case CFFCommon::eShootingRangeInfoDataTypes::VMD_AcceptedObjectLimit_MinHeight:
			mShootingRangeInfo->VMD.AcceptedObjectLimit.MinHeight = val;
			break;
		case CFFCommon::eShootingRangeInfoDataTypes::VMD_AcceptedObjectLimit_MaxWidth:
			mShootingRangeInfo->VMD.AcceptedObjectLimit.MaxWidth = val;
			break;
		case CFFCommon::eShootingRangeInfoDataTypes::VMD_AcceptedObjectLimit_MaxHeight:
			mShootingRangeInfo->VMD.AcceptedObjectLimit.MaxHeight = val;
			break;
	}
}
void CDevice::SetShootingRangeInfo(CFFCommon::eShootingRangeInfoDataTypes shootingRangeInfoDataType, POINTF val)
{
	switch (shootingRangeInfoDataType)
	{
		default:
			break;
	}
}
void CDevice::SetShootingRangeInfo(CFFCommon::eShootingRangeInfoDataTypes shootingRangeInfoDataType, POINTF* val)
{
	switch (shootingRangeInfoDataType)
	{
		default:
			break;
	}
}
void CDevice::SetShootingRangeInfo(CFFCommon::eShootingRangeInfoDataTypes shootingRangeInfoDataType, LPTSTR val)
{
	switch (shootingRangeInfoDataType)
	{
		case CFFCommon::eShootingRangeInfoDataTypes::Connection_URL:
			_tcscpy(mShootingRangeInfo->Connection.URL, val);
			break;
	}
}
void CDevice::SetShootingRangeInfo(CFFCommon::eShootingRangeInfoDataTypes shootingRangeInfoDataType, HWND val)
{
	switch (shootingRangeInfoDataType)
	{
		case CFFCommon::eShootingRangeInfoDataTypes::Draw_Windows_TargetWindow:
			mShootingRangeInfo->Draw.Windows.TargetWindow = val;
			break;
	}
}
void CDevice::SetShootingRangeInfo(CFFCommon::eShootingRangeInfoDataTypes shootingRangeInfoDataType, COLORREF val)
{
	switch (shootingRangeInfoDataType)
	{
		default:
			break;
	}
}
void CDevice::OnFrameReceivedFromRTSPStatic(void* user, CFFCommon::RTSPFrameData rtspFrame)
{
	CDevice* device = (CDevice*)user;
	if (device != NULL)
	{
		device->OnFrameReceivedFromRTSP(rtspFrame);
	}
}
void CDevice::OnFrameReceivedFromRTSP(CFFCommon::RTSPFrameData rtspFrame)
{
	try
	{
		if (mBuffer != NULL)
		{
			mBuffer->Enqueue(rtspFrame);
		}
	}
	catch (...)
	{
		CExceptionReport::WriteExceptionReportToFile("CDeviceManager::OnFrameReceived", "Exception in CDeviceManager::OnFrameReceived");
	}
}
void CDevice::OnFrameReceivedFromBufferStatic(void* user, CFFCommon::RTSPFrameData rtspFrame)
{
	CDevice* device = (CDevice*)user;
	if (device != NULL)
	{
		device->OnFrameReceivedFromBuffer(rtspFrame);
	}
}
void CDevice::OnFrameReceivedFromBuffer(CFFCommon::RTSPFrameData rtspFrame)
{
	try
	{
		if (rtspFrame.MediaType == AVMEDIA_TYPE_AUDIO)
		{
			CFFCommon::DecodeAudioData decodeAudioData;
			CFFCommon::SampleConversionData sampleConversionData;
			//------------------------
			int res = 0;
			int res1 = 0;
			if (mFFMpegRtsp != NULL)
			{
				if (mFFDecodeAudio != NULL)
				{
					CFFCommon::DecodeAudioParams dap;
					dap.CodecID = rtspFrame.CodecID;
					dap.RTSPFrame = rtspFrame;
					dap.RTSPFrame.MediaType = AVMEDIA_TYPE_AUDIO;
					res = mFFDecodeAudio->DecodeAudio(dap, &decodeAudioData);
					if (res > 0)
					{
						if (mFFSampleConversion != NULL)
						{
							CFFCommon::SampleConversionParams scp;
							scp.Source.ChannelLayout = decodeAudioData.ChannelLayout;
							scp.Source.SampleRate = decodeAudioData.SampleRate;
							scp.Source.SampleFormatsID = decodeAudioData.SampleFmt;
							scp.Target.ChannelLayout = decodeAudioData.ChannelLayout;
							scp.Target.SampleRate = decodeAudioData.SampleRate;
							scp.Target.SampleFormatsID = AV_SAMPLE_FMT_S16;
							scp.DecodedAudio = decodeAudioData;
							res1 = mFFSampleConversion->PerformSampleConversion(scp, &sampleConversionData);
							if (res1 > 0)
							{
								if (sampleConversionData.ParamsChanged)
								{
									mWavePlaying = new CWavePlaying(10, 10000, sampleConversionData.SamplesPerSec, sampleConversionData.BitsPerSample, sampleConversionData.Channels);
									mWavePlaying->Open();
								}
								mWavePlaying->Play((char*)*sampleConversionData.Data, 0, (ULONG)sampleConversionData.LineSize[0] / (sampleConversionData.BitsPerSample / 8));
							}
						}
					}
				}
			}
		}
		if (rtspFrame.MediaType == AVMEDIA_TYPE_VIDEO)
		{
			CFFCommon::DecodeVideoData decodeVideoData;
			CFFCommon::ColorConversionData colorConversionData;
			//------------------------
			int res = 0;
			int res1 = 0;
			if (mFFMpegRtsp != NULL)
			{
				if (mFFDecodeVideo != NULL)
				{
					CFFCommon::DecodeVideoParams dp;
					dp.CodecID = rtspFrame.CodecID;
					dp.RTSPFrame = rtspFrame;
					dp.RTSPFrame.MediaType = AVMEDIA_TYPE_VIDEO;
					res = mFFDecodeVideo->DecodeVideo(dp, &decodeVideoData);
					if (res > 0)
					{
						if (mFFColorConversion != NULL)
						{
							CFFCommon::ColorConversionParams ccp;
							ccp.SourcePixelFormatsID = decodeVideoData.TargetPixelFormatsID;
							ccp.TargetPixelFormatsID = AV_PIX_FMT_BGR24;
							ccp.DecodedVideo = decodeVideoData;
							double widthHeightRatio = (double)decodeVideoData.Width / (double)decodeVideoData.Height;
							ccp.ResizedHeight = min(decodeVideoData.Height, 480);
							ccp.ResizedWidth = (int)((double)ccp.ResizedHeight * widthHeightRatio);
							res1 = mFFColorConversion->PerformColorConversion(ccp, &colorConversionData);
							if (res1 > 0)
							{
								CVMDFrame* mVMDFrame = NULL;
								CVMDFrame* mAverageFrame = NULL;
								CVMDFrame* mDifferenceFrame = NULL;
								CVMDFrame* mToleranceFrame = NULL;
								CVMDFrame* mMedianFrame = NULL;
								CVMDFrame* mBorderFrame = NULL;
								CVMDFrame* mFilteredFrame = NULL;
								mAverageBuffer->SetDesiredCount(mShootingRangeInfo->VMD.AverageBufferSizeInFrames);
								mVMDFrame = new CVMDFrame(VMD_RGB_COLOR_TYPE, colorConversionData.Width, colorConversionData.Height, *colorConversionData.Data, mShootingRangeInfo->VMD.ResizeSquare);
								if (mVMDFrame != NULL)
								{
									if (colorConversionData.ParamsChanged) mAverageBufferMustBeReallocated = TRUE;
									if (mLastResizeSquare != mShootingRangeInfo->VMD.ResizeSquare)
									{
										mAverageBufferMustBeReallocated = TRUE;
										mLastResizeSquare = mShootingRangeInfo->VMD.ResizeSquare;
									}
									if (mAverageBufferMustBeReallocated)
									{
										mAverageBufferMustBeReallocated = FALSE;
										if (mAverageBuffer != NULL)
										{
											delete mAverageBuffer;
											mAverageBuffer = NULL;
										}
										mAverageBuffer = new CVMDAverageBuffer;
										mAverageBuffer->SetDesiredCount(mShootingRangeInfo->VMD.AverageBufferSizeInFrames);
									}
									mAverageFrame = mAverageBuffer->Add(mVMDFrame);
									if (mAverageFrame != NULL)
									{
										mDifferenceFrame = mVMDFrame->DifferenceFrame(mAverageFrame);
										if (mDifferenceFrame != NULL)
										{
											mToleranceFrame = mDifferenceFrame->ToleranceFrame(mShootingRangeInfo->VMD.Tolerance);
											if (mToleranceFrame != NULL)
											{
												mMedianFrame = mToleranceFrame->MedianFrame(mShootingRangeInfo->VMD.MedianSquare);
												if (mMedianFrame != NULL)
												{
													mBorderFrame = mMedianFrame->BorderFrame();
													if (mBorderFrame != NULL)
													{
														//--------------------------------------------------------------------------------------------------
														FILTERINFO* fi = (FILTERINFO*)&CVMDFrame::FILTER_BLUR_01;
														mFilteredFrame = mVMDFrame->FilteredFrame(fi);
														if (mFilteredFrame != NULL)
														{
															//--------------------------------------------------------------------------------------------------
															int instantMotionAcceptedCount;
															mMedianFrame->EnumerateMovingObjects(mShootingRangeInfo->VMD.MinimalDistanceBetweenTwoObjects, mShootingRangeInfo->VMD.AcceptedObjectLimit.MinWidth, mShootingRangeInfo->VMD.AcceptedObjectLimit.MinHeight, mShootingRangeInfo->VMD.AcceptedObjectLimit.MaxWidth, mShootingRangeInfo->VMD.AcceptedObjectLimit.MaxHeight, mShootingRangeInfo->VMD.MaximalAmountOfObjectsToFind, mInstantMotionAccepted, mInstantMotionAcceptedAveragePoint, &instantMotionAcceptedCount);
															if (instantMotionAcceptedCount == 1)
															{
																DWORD timeNow = GetTickCount();
																if (timeNow - mLastShotTimeInMS > mShootingRangeInfo->VMD.IdleTimeAfterShotInMS)
																{
																	memcpy(&mLastShotCoordinates, &(mInstantMotionAcceptedAveragePoint[0]), sizeof(POINTF));
																	RECTANGLEF tempShotBorder = mInstantMotionAccepted[0];
																	//------------------------------------------------------
																	mLastShotBorder[0].X = mInstantMotionAccepted[0].left;
																	mLastShotBorder[0].Y = mInstantMotionAccepted[0].top;
																	mLastShotBorder[1].X = mInstantMotionAccepted[0].right;
																	mLastShotBorder[1].Y = mInstantMotionAccepted[0].top;
																	mLastShotBorder[2].X = mInstantMotionAccepted[0].right;
																	mLastShotBorder[2].Y = mInstantMotionAccepted[0].bottom;
																	mLastShotBorder[3].X = mInstantMotionAccepted[0].left;
																	mLastShotBorder[3].Y = mInstantMotionAccepted[0].bottom;
																	//------------------------------------------------------
																	mLastShotTimeInMS = timeNow;
																	if (mEventShot != NULL)	SetEvent(mEventShot);
																}
															}
															//--------------------------------------------------------------------------------------------------
															EnterCriticalSection(mCriticalSection);
															//--------------------------------------
															switch (mShootingRangeInfo->VMD.WhatToShowType)
															{
															case CFFCommon::eWhatToShowTypes::ShowNotSetYet:
																break;
															case CFFCommon::eWhatToShowTypes::ShowRGBFrame:
																mRGBPictureWidth = colorConversionData.Width;
																mRGBPictureHeight = colorConversionData.Height;
																memcpy(mRGBPicture, *colorConversionData.Data, colorConversionData.Width * colorConversionData.Height * 3);
																break;
															case CFFCommon::eWhatToShowTypes::ShowVMDFrame:
																mVMDFrame->GetDimensions(&mRGBPictureWidth, &mRGBPictureHeight);
																mVMDFrame->GetData(mRGBPicture);
																break;
															case CFFCommon::eWhatToShowTypes::ShowAverageFrame:
																mAverageFrame->GetDimensions(&mRGBPictureWidth, &mRGBPictureHeight);
																mAverageFrame->GetData(mRGBPicture);
																break;
															case CFFCommon::eWhatToShowTypes::ShowDifferenceFrame:
																mDifferenceFrame->GetDimensions(&mRGBPictureWidth, &mRGBPictureHeight);
																mDifferenceFrame->GetData(mRGBPicture);
																break;
															case CFFCommon::eWhatToShowTypes::ShowToleranceFrame:
																mToleranceFrame->GetDimensions(&mRGBPictureWidth, &mRGBPictureHeight);
																mToleranceFrame->GetData(mRGBPicture);
																break;
															case CFFCommon::eWhatToShowTypes::ShowMedianFrame:
																mMedianFrame->GetDimensions(&mRGBPictureWidth, &mRGBPictureHeight);
																mMedianFrame->GetData(mRGBPicture);
																break;
															case CFFCommon::eWhatToShowTypes::ShowBorderFrame:
																mBorderFrame->GetDimensions(&mRGBPictureWidth, &mRGBPictureHeight);
																mBorderFrame->GetData(mRGBPicture);
																break;
															case CFFCommon::eWhatToShowTypes::ShowFilterFrame:
																mFilteredFrame->GetDimensions(&mRGBPictureWidth, &mRGBPictureHeight);
																mFilteredFrame->GetData(mRGBPicture);
																break;
															}
															//--------------------------------------
															LeaveCriticalSection(mCriticalSection);
															//--------------------------------------------------------------------------------------------------
															if (mGDIDrawTarget != NULL)
															{
																if (mShootingRangeInfo->Draw.Windows.TargetWindow != NULL)
																{
																	CFFCommon::GDIDrawParams gdp;
																	BYTE* tempData = NULL;
																	//--------------------------------------------------------------------------------------------------
																	gdp.ColorConversionWidth = colorConversionData.Width;
																	gdp.ColorConversionHeight = colorConversionData.Height;
																	tempData = new BYTE[gdp.ColorConversionWidth * gdp.ColorConversionHeight * 3];
																	memcpy(tempData, *colorConversionData.Data, gdp.ColorConversionWidth * gdp.ColorConversionHeight * 3);
																	//--------------------------------------------------------------------------------------------------
																	gdp.Data = tempData;
																	gdp.Window = mShootingRangeInfo->Draw.Windows.TargetWindow;
																	gdp.ShootingRangeInfo = mShootingRangeInfo;
																	//--------------------------------------------------------------------------------------------------
																	gdp.InstantMotionAccepted = mInstantMotionAccepted;
																	gdp.InstantMotionAcceptedAveragePoint = mInstantMotionAcceptedAveragePoint;
																	gdp.InstantMotionAcceptedCount = instantMotionAcceptedCount;
																	//--------------------------------------------------------------------------------------------------
																	if (mShootingRangeInfo->VMD.WhatToShowType != CFFCommon::eWhatToShowTypes::ShowNotSetYet) mGDIDrawTarget->DrawImageOnly(gdp);
																	if (tempData != NULL) delete[] tempData;
																}
															}
															//--------------------------------------------------------------------------------------------------
															delete mFilteredFrame;
															mFilteredFrame = NULL;
														}
														delete mBorderFrame;
														mBorderFrame = NULL;
													}
													delete mMedianFrame;
													mMedianFrame = NULL;
												}
												delete mToleranceFrame;
												mToleranceFrame = NULL;
											}
											delete mDifferenceFrame;
											mDifferenceFrame = NULL;
										}
										delete mAverageFrame;
										mAverageFrame = NULL;
									}
									delete mVMDFrame;
									mVMDFrame = NULL;
								}
							}
						}
					}
				}
			}
		}
	}
	catch (...)
	{
		CExceptionReport::WriteExceptionReportToFile("CDeviceManager::OnFrameReceived", "Exception in CDeviceManager::OnFrameReceived");
	}
}
