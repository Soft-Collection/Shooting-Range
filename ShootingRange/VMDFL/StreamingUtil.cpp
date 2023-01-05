#include "stdafx.h"
#include "StreamingUtil.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Audio Video In///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void* DeviceCreateNew()
{
	CDevice* cDevice = new CDevice();
	return ((void*)cDevice);
}
void DeviceDispose(void* handle)
{
	if (handle == NULL) return;
	CDevice* cDevice = (CDevice*)handle;
	delete cDevice;
}
void DeviceConnect(void* handle)
{
	if (handle == NULL) return;
	CDevice* cDevice = (CDevice*)handle;
	cDevice->Connect();
}
void DeviceDisconnect(void* handle)
{
	if (handle == NULL) return;
	CDevice* cDevice = (CDevice*)handle;
	cDevice->Disconnect();
}
CFFCommon::eConnectionStates DeviceGetConnectionState(void* handle)
{
	if (handle == NULL) return(CFFCommon::eConnectionStates::NotSetYet);
	CDevice* cDevice = (CDevice*)handle;
	return (cDevice->GetConnectionState());
}
void DeviceGetShotData(void* handle, POINTF* shotCoordinates, POINTF* shotBorder1, POINTF* shotBorder2, POINTF* shotBorder3, POINTF* shotBorder4)
{
	if (handle == NULL) return;
	CDevice* cDevice = (CDevice*)handle;
	POINTF tempShotBorder[4];
	cDevice->GetShotData(shotCoordinates, tempShotBorder);
	*shotBorder1 = tempShotBorder[0];
	*shotBorder2 = tempShotBorder[1];
	*shotBorder3 = tempShotBorder[2];
	*shotBorder4 = tempShotBorder[3];
}
void DeviceRGBPictureLock(void* handle)
{
	if (handle == NULL) return;
	CDevice* cDevice = (CDevice*)handle;
	cDevice->RGBPictureLock();
}
void DeviceRGBPictureUnlock(void* handle)
{
	if (handle == NULL) return;
	CDevice* cDevice = (CDevice*)handle;
	cDevice->RGBPictureUnlock();
}
void DeviceGetRGBPictureSize(void* handle, int* width, int* height)
{
	if (handle == NULL) return;
	CDevice* cDevice = (CDevice*)handle;
	cDevice->GetRGBPictureSize(width, height);
}
void DeviceGetRGBPictureData(void* handle, BYTE* data)
{
	if (handle == NULL) return;
	CDevice* cDevice = (CDevice*)handle;
	cDevice->GetRGBPictureData(data);
}
//------------------------------------------------------------------------------------------------------------------------------
void DeviceSetShootingRangeInfo1(void* handle, CFFCommon::eShootingRangeInfoDataTypes shootingRangeInfoDataType, int val)
{
	if (handle == NULL) return;
	CDevice* cDevice = (CDevice*)handle;
	cDevice->SetShootingRangeInfo(shootingRangeInfoDataType, val);
}
void DeviceSetShootingRangeInfo2(void* handle, CFFCommon::eShootingRangeInfoDataTypes shootingRangeInfoDataType, float val)
{
	if (handle == NULL) return;
	CDevice* cDevice = (CDevice*)handle;
	cDevice->SetShootingRangeInfo(shootingRangeInfoDataType, val);
}
void DeviceSetShootingRangeInfo3(void* handle, CFFCommon::eShootingRangeInfoDataTypes shootingRangeInfoDataType, POINTF val)
{
	if (handle == NULL) return;
	CDevice* cDevice = (CDevice*)handle;
	cDevice->SetShootingRangeInfo(shootingRangeInfoDataType, val);
}
void DeviceSetShootingRangeInfo4(void* handle, CFFCommon::eShootingRangeInfoDataTypes shootingRangeInfoDataType, POINTF* val)
{
	if (handle == NULL) return;
	CDevice* cDevice = (CDevice*)handle;
	cDevice->SetShootingRangeInfo(shootingRangeInfoDataType, val);
}
void DeviceSetShootingRangeInfo5(void* handle, CFFCommon::eShootingRangeInfoDataTypes shootingRangeInfoDataType, LPTSTR val)
{
	if (handle == NULL) return;
	CDevice* cDevice = (CDevice*)handle;
	cDevice->SetShootingRangeInfo(shootingRangeInfoDataType, val);
}
void DeviceSetShootingRangeInfo6(void* handle, CFFCommon::eShootingRangeInfoDataTypes shootingRangeInfoDataType, HWND val)
{
	if (handle == NULL) return;
	CDevice* cDevice = (CDevice*)handle;
	cDevice->SetShootingRangeInfo(shootingRangeInfoDataType, val);
}
void DeviceSetShootingRangeInfo7(void* handle, CFFCommon::eShootingRangeInfoDataTypes shootingRangeInfoDataType, COLORREF val)
{
	if (handle == NULL) return;
	CDevice* cDevice = (CDevice*)handle;
	cDevice->SetShootingRangeInfo(shootingRangeInfoDataType, val);
}