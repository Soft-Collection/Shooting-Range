#include "CFFCommon.h"
#include "CFFMpegRtsp.h"
#include "CDevice.h"

extern "C"
{
	__declspec(dllexport) void* DeviceCreateNew();
	__declspec(dllexport) void DeviceDispose(void* handle);
	__declspec(dllexport) void DeviceConnect(void* handle);
	__declspec(dllexport) void DeviceDisconnect(void* handle);
	__declspec(dllexport) CFFCommon::eConnectionStates DeviceGetConnectionState(void* handle);
	__declspec(dllexport) void DeviceGetShotData(void* handle, POINTF* shotCoordinates, POINTF* shotBorder1, POINTF* shotBorder2, POINTF* shotBorder3, POINTF* shotBorder4);
	__declspec(dllexport) void DeviceRGBPictureLock(void* handle);
	__declspec(dllexport) void DeviceRGBPictureUnlock(void* handle);
	__declspec(dllexport) void DeviceGetRGBPictureSize(void* handle, int* width, int* height);
	__declspec(dllexport) void DeviceGetRGBPictureData(void* handle, BYTE* data);
	//--------------------------------------------------------------------------------------------------------------------------------------------------
	__declspec(dllexport) void DeviceSetShootingRangeInfo1(void* handle, CFFCommon::eShootingRangeInfoDataTypes shootingRangeInfoDataType, int val);
	__declspec(dllexport) void DeviceSetShootingRangeInfo2(void* handle, CFFCommon::eShootingRangeInfoDataTypes shootingRangeInfoDataType, float val);
	__declspec(dllexport) void DeviceSetShootingRangeInfo3(void* handle, CFFCommon::eShootingRangeInfoDataTypes shootingRangeInfoDataType, POINTF val);
	__declspec(dllexport) void DeviceSetShootingRangeInfo4(void* handle, CFFCommon::eShootingRangeInfoDataTypes shootingRangeInfoDataType, POINTF* val);
	__declspec(dllexport) void DeviceSetShootingRangeInfo5(void* handle, CFFCommon::eShootingRangeInfoDataTypes shootingRangeInfoDataType, LPTSTR val);
	__declspec(dllexport) void DeviceSetShootingRangeInfo6(void* handle, CFFCommon::eShootingRangeInfoDataTypes shootingRangeInfoDataType, HWND val);
	__declspec(dllexport) void DeviceSetShootingRangeInfo7(void* handle, CFFCommon::eShootingRangeInfoDataTypes shootingRangeInfoDataType, COLORREF val);
}