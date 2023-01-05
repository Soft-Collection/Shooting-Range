using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ShootingRange
{
    class Settings
    {
        #region Properties
        #region Original Settings
        //Location Property.
        public static Point Location
        {
            get
            {
                System.Drawing.Point retVal = new System.Drawing.Point();
                retVal.X = GetInt("Left", 0);
                retVal.Y = GetInt("Top", 0);
                return retVal;
            }
            set
            {
                SetInt("Left", value.X);
                SetInt("Top", value.Y);
            }
        }
        //Size Property.
        public static System.Drawing.Size Size
        {
            get
            {
                System.Drawing.Size retVal = new System.Drawing.Size();
                retVal.Width = GetInt("Width", 500);
                retVal.Height = GetInt("Height", 400);
                return retVal;
            }
            set
            {
                SetInt("Width", value.Width);
                SetInt("Height", value.Height);
            }
        }
        //Visible Property.
        public static bool Visible
        {
            get
            {
                return (GetBool("Visible", true));
            }
            set
            {
                SetBool("Visible", value);
            }
        }
        //Always On Top Property.
        public static bool AlwaysOnTop
        {
            get
            {
                return (GetBool("AlwaysOnTop", true));
            }
            set
            {
                SetBool("AlwaysOnTop", value);
            }
        }
        //Run At Startup Property.
        public static bool RunAtStartup
        {
            get
            {
                bool retVal = GetBool("RunAtStartup", false);
                //If application executable file location has been changed.
                RegistryKey runKey = Registry.CurrentUser.OpenSubKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", true);
                if (retVal) runKey.SetValue(GetAssemblyInfo.AssemblyProduct, Application.ExecutablePath.ToString());
                else runKey.DeleteValue(GetAssemblyInfo.AssemblyProduct, false);
                return (retVal);
            }
            set
            {
                SetBool("RunAtStartup", value);
                RegistryKey runKey = Registry.CurrentUser.OpenSubKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", true);
                if (value) runKey.SetValue(GetAssemblyInfo.AssemblyProduct, Application.ExecutablePath.ToString());
                else runKey.DeleteValue(GetAssemblyInfo.AssemblyProduct, false);
            }
        }
        #endregion

        #region Shooting Range Properties
        public static int VMD_AverageBufferSizeInFrames
        {
            get
            {
                return (GetInt("VMD_AverageBufferSizeInFrames", 10));
            }
            set
            {
                SetInt("VMD_AverageBufferSizeInFrames", value);
            }
        }
        public static int VMD_ResizeSquare
        {
            get
            {
                return (GetInt("VMD_ResizeSquare", 2));
            }
            set
            {
                SetInt("VMD_ResizeSquare", value);
            }
        }
        public static int VMD_Tolerance
        {
            get
            {
                return (GetInt("VMD_Tolerance", 30));
            }
            set
            {
                SetInt("VMD_Tolerance", value);
            }
        }
        public static int VMD_MedianSquare
        {
            get
            {
                return (GetInt("VMD_MedianSquare", 2));
            }
            set
            {
                SetInt("VMD_MedianSquare", value);
            }
        }
        public static int VMD_MinimalDistanceBetweenTwoObjects
        {
            get
            {
                return (GetInt("VMD_MinimalDistanceBetweenTwoObjects", 10));
            }
            set
            {
                SetInt("VMD_MinimalDistanceBetweenTwoObjects", value);
            }
        }
        public static int VMD_MaximalAmountOfObjectsToFind
        {
            get
            {
                return (GetInt("VMD_MaximalAmountOfObjectsToFind", 2));
            }
            set
            {
                SetInt("VMD_MaximalAmountOfObjectsToFind", value);
            }
        }
        public static clsDeviceWrap.eWhatToShowTypes VMD_WhatToShowType
        {
            get
            {
                return ((clsDeviceWrap.eWhatToShowTypes)GetInt("VMD_WhatToShowType", (int)clsDeviceWrap.eWhatToShowTypes.ShowRGBFrame));
            }
            set
            {
                SetInt("VMD_WhatToShowType", (int)value);
            }
        }
        //-----------------------------------------------------------------------
        public static bool VMD_HorizontalFlip
        {
            get
            {
                return (GetBool("VMD_HorizontalFlip", false));
            }
            set
            {
                SetBool("VMD_HorizontalFlip", value);
            }
        }
        public static int VMD_IdleTimeAfterShotInMS
        {
            get
            {
                return (GetInt("VMD_IdleTimeAfterShotInMS", 200));
            }
            set
            {
                SetInt("VMD_IdleTimeAfterShotInMS", value);
            }
        }
        //-----------------------------------------------------------------------
        public static PointF Draw_DoubleSquare_CenterLocation
        {
            get
            {
                System.Drawing.PointF retVal = new System.Drawing.Point();
                retVal.X = GetFloat("Draw_DoubleSquare_CenterLocation_X", 0.5f);
                retVal.Y = GetFloat("Draw_DoubleSquare_CenterLocation_Y", 0.5f);
                return retVal;
            }
            set
            {
                SetFloat("Draw_DoubleSquare_CenterLocation_X", value.X);
                SetFloat("Draw_DoubleSquare_CenterLocation_Y", value.Y);
            }
        }
        public static float Draw_DoubleSquare_InnerSquareDiagonal
        {
            get
            {
                return (GetFloat("Draw_DoubleSquare_InnerSquareDiagonal", 0.02f));
            }
            set
            {
                SetFloat("Draw_DoubleSquare_InnerSquareDiagonal", value);
            }
        }
        public static float Draw_DoubleSquare_OuterSquareDiagonal
        {
            get
            {
                return (GetFloat("Draw_DoubleSquare_OuterSquareDiagonal", 0.05f));
            }
            set
            {
                SetFloat("Draw_DoubleSquare_OuterSquareDiagonal", value);
            }
        }
        //-----------------------------------------------------------------------
        public static PointF[] Draw_TargetBorder_Border
        {
            get
            {
                System.Drawing.PointF[] retVal = new System.Drawing.PointF[4];
                //----------------------------------------------------------------
                retVal[0].X = GetFloat("Draw_TargetBorder_BorderValue_0_X", 0.2f);
                retVal[0].Y = GetFloat("Draw_TargetBorder_BorderValue_0_Y", 0.2f);
                //----------------------------------------------------------------
                retVal[1].X = GetFloat("Draw_TargetBorder_BorderValue_1_X", 0.8f);
                retVal[1].Y = GetFloat("Draw_TargetBorder_BorderValue_1_Y", 0.2f);
                //----------------------------------------------------------------
                retVal[2].X = GetFloat("Draw_TargetBorder_BorderValue_2_X", 0.8f);
                retVal[2].Y = GetFloat("Draw_TargetBorder_BorderValue_2_Y", 0.8f);
                //----------------------------------------------------------------
                retVal[3].X = GetFloat("Draw_TargetBorder_BorderValue_3_X", 0.2f);
                retVal[3].Y = GetFloat("Draw_TargetBorder_BorderValue_3_Y", 0.8f);
                //----------------------------------------------------------------
                return retVal;
            }
            set
            {
                SetFloat("Draw_TargetBorder_BorderValue_0_X", value[0].X);
                SetFloat("Draw_TargetBorder_BorderValue_0_Y", value[0].Y);
                //------------------------------------------------------
                SetFloat("Draw_TargetBorder_BorderValue_1_X", value[1].X);
                SetFloat("Draw_TargetBorder_BorderValue_1_Y", value[1].Y);
                //------------------------------------------------------
                SetFloat("Draw_TargetBorder_BorderValue_2_X", value[2].X);
                SetFloat("Draw_TargetBorder_BorderValue_2_Y", value[2].Y);
                //------------------------------------------------------
                SetFloat("Draw_TargetBorder_BorderValue_3_X", value[3].X);
                SetFloat("Draw_TargetBorder_BorderValue_3_Y", value[3].Y);
            }
        }
        //-----------------------------------------------------------------------
        #endregion
        #endregion

        #region Registry Access Functions
        private static bool GetBool(string name, bool defaultValue)
        {
            return bool.Parse(GetString(name, defaultValue.ToString()));
        }
        private static void SetBool(string name, bool value)
        {
            SetString(name, value.ToString());
        }
        private static int GetInt(string name, int defaultValue)
        {
            return int.Parse(GetString(name, defaultValue.ToString()));
        }
        private static void SetInt(string name, int value)
        {
            SetString(name, value.ToString());
        }
        private static uint GetUInt(string name, uint defaultValue)
        {
            return uint.Parse(GetString(name, defaultValue.ToString()));
        }
        private static void SetUInt(string name, uint value)
        {
            SetString(name, value.ToString());
        }
        private static float GetFloat(string name, float defaultValue)
        {
            return float.Parse(GetString(name, defaultValue.ToString()));
        }
        private static void SetFloat(string name, float value)
        {
            SetString(name, value.ToString());
        }
        private static double GetDouble(string name, double defaultValue)
        {
            return double.Parse(GetString(name, defaultValue.ToString()));
        }
        private static void SetDouble(string name, double value)
        {
            SetString(name, value.ToString());
        }
        private static string GetString(string name, string defaultValue)
        {
            string retVal = null;
            RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
            if (key == null) key = Registry.CurrentUser.CreateSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct);
            retVal = (string)key.GetValue(name, defaultValue);
            return retVal;
        }
        private static void SetString(string name, string value)
        {
            RegistryKey key = Registry.CurrentUser.OpenSubKey("Software\\" + GetAssemblyInfo.AssemblyCompany + "\\" + GetAssemblyInfo.AssemblyProduct, true);
            key.SetValue(name, value.ToString());
        }
        #endregion
    }
}
