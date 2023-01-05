using System;
using System.Drawing;
using System.Runtime.InteropServices;

namespace ShootingRange
{
    public class clsDeviceWrap
    {
        #region Constants
        private const string DllFileName = "VMDFL.dll";
        #endregion

        #region "Enums"
        public enum eWhatToShowTypes : int
        {
            ShowNotSetYet,
            ShowRGBFrame,
            ShowVMDFrame,
            ShowAverageFrame,
            ShowDifferenceFrame,
            ShowToleranceFrame,
            ShowMedianFrame,
            ShowBorderFrame,
            ShowFilterFrame
        };
        public enum eFilterTypes : int
        {
            Filter_Blur_01,
            Filter_Blur_02,
            Filter_Blur_03,
            Filter_Motion_Blur,
            Filter_Edges_01,
            Filter_Edges_02,
            Filter_Edges_03,
            Filter_Edges_04,
            Filter_Edges_Prewitt,
            Filter_Edges_Sobel,
            Filter_Edges_Laplacian,
            Filter_Shapen_01,
            Filter_Shapen_02,
            Filter_Shapen_03,
            Filter_Emboss_01,
            Filter_Emboss_02,
            Filter_Mean
        };
        public enum eShootingRangeInfoDataTypes : int
        {
            //-------------------------------------
            VMD_AverageBufferSizeInFrames,
            VMD_ResizeSquare,
            VMD_Tolerance,
            VMD_MedianSquare,
            VMD_MinimalDistanceBetweenTwoObjects,
            VMD_MaximalAmountOfObjectsToFind,
            VMD_WhatToShowType,
            //-------------------
            VMD_AcceptedObjectLimit_MinWidth,
            VMD_AcceptedObjectLimit_MinHeight,
            VMD_AcceptedObjectLimit_MaxWidth,
            VMD_AcceptedObjectLimit_MaxHeight,
            //-------------------
            VMD_IdleTimeAfterShotInMS,
            //-------------------------------------
            Draw_Windows_TargetWindow,
            //-------------------------------------
            Connection_URL,
            Connection_IsWebCam
            //-------------------------------------
        };
        #endregion

        #region "Structures"
        #endregion

        #region "External Functions"
        //__declspec(dllexport) void* DeviceCreateNew();
        //__declspec(dllexport) void DeviceDispose(void* handle);
        //__declspec(dllexport) void DeviceConnect(void* handle);
        //__declspec(dllexport) void DeviceDisconnect(void* handle);
        //__declspec(dllexport) CFFCommon::eConnectionStates DeviceGetConnectionState(void* handle);
        //__declspec(dllexport) void DeviceGetShotData(void* handle, POINTF* shotCoordinates, POINTF* shotBorder);
        //__declspec(dllexport) void DeviceRGBPictureLock(void* handle);
        //__declspec(dllexport) void DeviceRGBPictureUnlock(void* handle);
        //__declspec(dllexport) void DeviceGetRGBPictureSize(void* handle, int* width, int* height);
        //__declspec(dllexport) void DeviceGetRGBPictureData(void* handle, BYTE* data);
        //__declspec(dllexport) void DeviceSetShootingRangeInfo1(void* handle, CFFCommon::eShootingRangeInfoDataTypes shootingRangeInfoDataType, int val);
        //__declspec(dllexport) void DeviceSetShootingRangeInfo2(void* handle, CFFCommon::eShootingRangeInfoDataTypes shootingRangeInfoDataType, float val);
        //__declspec(dllexport) void DeviceSetShootingRangeInfo3(void* handle, CFFCommon::eShootingRangeInfoDataTypes shootingRangeInfoDataType, POINTF val);
        //__declspec(dllexport) void DeviceSetShootingRangeInfo4(void* handle, CFFCommon::eShootingRangeInfoDataTypes shootingRangeInfoDataType, POINTF* val);
        //__declspec(dllexport) void DeviceSetShootingRangeInfo5(void* handle, CFFCommon::eShootingRangeInfoDataTypes shootingRangeInfoDataType, LPTSTR val);
        //__declspec(dllexport) void DeviceSetShootingRangeInfo6(void* handle, CFFCommon::eShootingRangeInfoDataTypes shootingRangeInfoDataType, HWND val);
        //__declspec(dllexport) void DeviceSetShootingRangeInfo7(void* handle, CFFCommon::eShootingRangeInfoDataTypes shootingRangeInfoDataType, COLORREF val);
        //================================================================================================
        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr DeviceCreateNew();
        //------------------------------------------------------------------------------------------------
        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void DeviceDispose(IntPtr handle);
        //------------------------------------------------------------------------------------------------
        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void DeviceConnect(IntPtr handle);
        //------------------------------------------------------------------------------------------------
        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void DeviceDisconnect(IntPtr handle);
        //------------------------------------------------------------------------------------------------
        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern Common.eConnectionStates DeviceGetConnectionState(IntPtr handle);
        //------------------------------------------------------------------------------------------------
        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void DeviceGetShotData(IntPtr handle, ref PointF shotCoordinates, ref PointF shotBorder1, ref PointF shotBorder2, ref PointF shotBorder3, ref PointF shotBorder4);
        //------------------------------------------------------------------------------------------------
        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void DeviceRGBPictureLock(IntPtr handle);
        //------------------------------------------------------------------------------------------------
        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void DeviceRGBPictureUnlock(IntPtr handle);
        //------------------------------------------------------------------------------------------------
        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void DeviceGetRGBPictureSize(IntPtr handle, ref int width, ref int height);
        //------------------------------------------------------------------------------------------------
        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void DeviceGetRGBPictureData(IntPtr handle, byte[] data);
        //------------------------------------------------------------------------------------------------
        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void DeviceGetRGBPictureData(IntPtr handle, IntPtr data);
        //------------------------------------------------------------------------------------------------
        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void DeviceSetShootingRangeInfo1(IntPtr handle, eShootingRangeInfoDataTypes shootingRangeInfoDataType, int val);
        //------------------------------------------------------------------------------------------------
        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void DeviceSetShootingRangeInfo2(IntPtr handle, eShootingRangeInfoDataTypes shootingRangeInfoDataType, float val);
        //------------------------------------------------------------------------------------------------
        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void DeviceSetShootingRangeInfo3(IntPtr handle, eShootingRangeInfoDataTypes shootingRangeInfoDataType, PointF val);
        //------------------------------------------------------------------------------------------------
        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void DeviceSetShootingRangeInfo4(IntPtr handle, eShootingRangeInfoDataTypes shootingRangeInfoDataType, PointF[] val);
        //------------------------------------------------------------------------------------------------
        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void DeviceSetShootingRangeInfo5(IntPtr handle, eShootingRangeInfoDataTypes shootingRangeInfoDataType, string val);
        //------------------------------------------------------------------------------------------------
        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void DeviceSetShootingRangeInfo6(IntPtr handle, eShootingRangeInfoDataTypes shootingRangeInfoDataType, IntPtr val);
        //------------------------------------------------------------------------------------------------
        [DllImport(DllFileName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void DeviceSetShootingRangeInfo7(IntPtr handle, eShootingRangeInfoDataTypes shootingRangeInfoDataType, uint val);
        #endregion
    }
}