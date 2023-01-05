using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ShootingRange
{
    public partial class SettingsFrm : Form
    {
        #region Variables
        private clsDevice mDevice = null;
        #endregion

        #region Constructor / Destructor
        public SettingsFrm()
        {
            InitializeComponent();
            //---------------------------------------
            timer.Start();
        }
        #endregion
        
        #region Methods
        public void SetDevice(clsDevice device)
        {
            mDevice = device;
            caCalibration.SetDevice(mDevice);
            UpdateControls();
        }
        private void UpdateControls()
        {
            caCalibration.UpdateControls();
            if (mDevice != null)
            {
                tbAverageBufferSizeInFrames.Value = mDevice.VMD_AverageBufferSizeInFrames;
                tbResizeSquare.Value = mDevice.VMD_ResizeSquare;
                tbTolerance.Value = mDevice.VMD_Tolerance;
                tbMedianSquare.Value = mDevice.VMD_MedianSquare;
                tbMinimalDistanceBetweenTwoObjects.Value = mDevice.VMD_MinimalDistanceBetweenTwoObjects;
                tbMaximalAmountOfObjectsToFind.Value = mDevice.VMD_MaximalAmountOfObjectsToFind;
                //----------------------------------------------------------------------------------------------
                txtAverageBufferSizeInFrames.Text = tbAverageBufferSizeInFrames.Value.ToString();
                txtResizeSquare.Text = tbResizeSquare.Value.ToString();
                txtTolerance.Text = tbTolerance.Value.ToString();
                txtMedianSquare.Text = tbMedianSquare.Value.ToString();
                txtMinimalDistanceBetweenTwoObjects.Text = tbMinimalDistanceBetweenTwoObjects.Value.ToString();
                txtMaximalAmountOfObjectsToFind.Text = tbMaximalAmountOfObjectsToFind.Value.ToString();
                //----------------------------------------------------------------------------------------------
                cbWhatToShowType.SelectedIndex = (int)mDevice.VMD_WhatToShowType - 1;
                cbCameraPosition.SelectedIndex = mDevice.VMD_HorizontalFlip ? 1 : 0;
            }
        }
        private void ApplyDefaults()
        {
            caCalibration.ApplyDefaults();
            if (mDevice != null)
            {
                mDevice.VMD_AverageBufferSizeInFrames = 10;
                mDevice.VMD_ResizeSquare = Settings.VMD_ResizeSquare = 2;
                mDevice.VMD_Tolerance = Settings.VMD_Tolerance = 30;
                mDevice.VMD_MedianSquare = Settings.VMD_MedianSquare = 2;
                mDevice.VMD_MinimalDistanceBetweenTwoObjects = 10;
                mDevice.VMD_MaximalAmountOfObjectsToFind = 2;
                mDevice.VMD_WhatToShowType = clsDeviceWrap.eWhatToShowTypes.ShowRGBFrame;
                mDevice.VMD_IdleTimeAfterShotInMS = 200;
                mDevice.VMD_HorizontalFlip = false;
            }
        }
        #endregion

        #region Main Form Events
        private void tbAverageBufferSizeInFrames_Scroll(object sender, EventArgs e)
        {
            txtAverageBufferSizeInFrames.Text = tbAverageBufferSizeInFrames.Value.ToString();
            if (mDevice != null)
            {
                mDevice.VMD_AverageBufferSizeInFrames = tbAverageBufferSizeInFrames.Value;
            }
        }
        private void tbResizeSquare_Scroll(object sender, EventArgs e)
        {
            txtResizeSquare.Text = tbResizeSquare.Value.ToString();
            if (mDevice != null)
            {
                mDevice.VMD_ResizeSquare = tbResizeSquare.Value;
            }
        }
        private void tbTolerance_Scroll(object sender, EventArgs e)
        {
            txtTolerance.Text = tbTolerance.Value.ToString();
            if (mDevice != null)
            {
                mDevice.VMD_Tolerance = tbTolerance.Value;
            }
        }
        private void tbMedianSquare_Scroll(object sender, EventArgs e)
        {
            txtMedianSquare.Text = tbMedianSquare.Value.ToString();
            if (mDevice != null)
            {
                mDevice.VMD_MedianSquare = tbMedianSquare.Value;
            }
        }
        private void tbMinimalDistanceBetweenTwoObjects_Scroll(object sender, EventArgs e)
        {
            txtMinimalDistanceBetweenTwoObjects.Text = tbMinimalDistanceBetweenTwoObjects.Value.ToString();
            if (mDevice != null)
            {
                mDevice.VMD_MinimalDistanceBetweenTwoObjects = tbMinimalDistanceBetweenTwoObjects.Value;
            }
        }
        private void tbMaximalAmountOfObjectsToFind_Scroll(object sender, EventArgs e)
        {
            txtMaximalAmountOfObjectsToFind.Text = tbMaximalAmountOfObjectsToFind.Value.ToString();
            if (mDevice != null)
            {
                mDevice.VMD_MaximalAmountOfObjectsToFind = tbMaximalAmountOfObjectsToFind.Value;
            }
        }
        private void cbWhatToShowType_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (mDevice != null)
            {
                mDevice.VMD_WhatToShowType = (clsDeviceWrap.eWhatToShowTypes)cbWhatToShowType.SelectedIndex + 1;
            }
        }
        private void cbCameraPosition_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (mDevice != null)
            {
                mDevice.VMD_HorizontalFlip = (cbCameraPosition.SelectedIndex == 0) ? false : true;
            }
        }
        private void bDefaults_Click(object sender, EventArgs e)
        {
            ApplyDefaults();
            UpdateControls();
        }
        private void timer_Tick(object sender, EventArgs e)
        {
            caCalibration.OnTimer();
        }
        #endregion
    }
}
