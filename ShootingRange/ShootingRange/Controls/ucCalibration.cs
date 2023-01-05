using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ShootingRange
{
    public partial class ucCalibration : UserControl
    {
        #region Inner Classes
        #endregion

        #region Delegates
        #endregion

        #region Events
        #endregion

        #region Enums
        #endregion

        #region Variables
        private clsGraphicsDoubleSquare mDoubleSquare = null;
        private clsGraphicsTargetBorder mTargetBorder = null;
        private clsGraphicsMotionBorder mMotionBorder = null;
        private Cursor mTargetBorderCursor = null;
        private Cursor mDoubleSquareCursor = null;
        private clsDevice mDevice = null;
        #endregion

        #region Properties
        public IntPtr CalibrationWindowHandle
        {
            get
            {
                return (pbCalibrationWindow.Handle);
            }
        }
        #endregion

        #region Constructor / Destructor
        public ucCalibration()
        {
            InitializeComponent();
            //-----------------------------------------------------
            mDoubleSquare = new clsGraphicsDoubleSquare();
            mDoubleSquare.OnCursorChanged += mDoubleSquare_OnCursorChanged;
            mDoubleSquare.OnRefreshNeeded += mDoubleSquare_OnRefreshNeeded;
            mDoubleSquare.OnDoubleSquareDataChanged += mDoubleSquare_OnDoubleSquareDataChanged;
            //-----------------------------------------------------
            mTargetBorder = new clsGraphicsTargetBorder();
            mTargetBorder.OnCursorChanged += mTargetBorder_OnCursorChanged;
            mTargetBorder.OnRefreshNeeded += mTargetBorder_OnRefreshNeeded;
            mTargetBorder.OnTargetBorderDataChanged += mTargetBorder_OnTargetBorderDataChanged;
            //-----------------------------------------------------
            mMotionBorder = new clsGraphicsMotionBorder();
        }
        #endregion

        #region Methods
        public void SetDevice(clsDevice device)
        {
            mDevice = device;
            mDevice.OnShot += device_OnShot;
        }
        public void UpdateControls()
        {
            if (mDevice != null)
            {
                clsGraphicsDoubleSquare.DoubleSquareData doubleSquareData = new clsGraphicsDoubleSquare.DoubleSquareData();
                doubleSquareData.Center = mDevice.Draw_DoubleSquare_CenterLocation;
                doubleSquareData.InnerDiagonal = mDevice.Draw_DoubleSquare_InnerSquareDiagonal;
                doubleSquareData.OuterDiagonal = mDevice.Draw_DoubleSquare_OuterSquareDiagonal;
                //----------------------------------------------------------------------------------------
                clsGraphicsDoubleSquare.DoubleSquareConfig doubleSquareConfig = new clsGraphicsDoubleSquare.DoubleSquareConfig();
                doubleSquareConfig.CenterDiagonal = 0.01f;
                doubleSquareConfig.LineThickness = 1;
                doubleSquareConfig.LineColor = Color.Yellow;
                doubleSquareConfig.CornerCircleRadiusDiagonal = 0.01f;
                doubleSquareConfig.CornerCircleBorderThickness = 1;
                doubleSquareConfig.CornerCircleBorderColor = Color.Yellow;
                doubleSquareConfig.CornerCircleFillColor = Color.Black;
                //----------------------------------------------------------------------------------------
                if (mDoubleSquare != null)
                {
                    mDoubleSquare.SetDoubleSquareData(doubleSquareData);
                }
                //----------------------------------------------------------------------------------------
                clsGraphicsTargetBorder.TargetBorderData targetBorderData = new clsGraphicsTargetBorder.TargetBorderData();
                targetBorderData.TargetBorder = mDevice.Draw_TargetBorder_Border;
                //----------------------------------------------------------------------------------------
                clsGraphicsTargetBorder.TargetBorderConfig targetBorderConfig = new clsGraphicsTargetBorder.TargetBorderConfig();
                targetBorderConfig.LineThickness = 1;
                targetBorderConfig.LineColor = Color.Yellow;
                targetBorderConfig.CornerCircleRadiusDiagonal = 0.01f;
                targetBorderConfig.CornerCircleBorderThickness = 1;
                targetBorderConfig.CornerFirstCircleFillColor = Color.Red;
                targetBorderConfig.CornerOtherCircleFillColor = Color.Black;
                targetBorderConfig.CornerCircleBorderColor = Color.Yellow;
                //----------------------------------------------------------------------------------------
                if (mTargetBorder != null)
                {
                    mTargetBorder.SetTargetBorderData(targetBorderData);
                }
            }
        }
        public void ApplyDefaults()
        {
            mDevice.Draw_DoubleSquare_CenterLocation = new PointF(0.5f, 0.5f);
            //----------------------------------------------------------------------------------------
            mDevice.Draw_DoubleSquare_InnerSquareDiagonal = 0.02f;
            mDevice.Draw_DoubleSquare_OuterSquareDiagonal = 0.05f;
            //========================================================================================
            mDevice.Draw_TargetBorder_Border = new PointF[] { new PointF(0.2f, 0.2f), new PointF(0.8f, 0.2f), new PointF(0.8f, 0.8f), new PointF(0.2f, 0.8f) };
        }
        public void OnTimer()
        {
            //------------------------------------------------------------
            pbCalibrationWindow.Invalidate();
            pbCalibrationWindow.Refresh();
        }
        public void device_OnShot(PointF shotCoordinates, PointF[] shotBorder, DateTime time)
        {
            clsGraphicsMotionBorder.MotionBorderData tempMotionBorderData = new clsGraphicsMotionBorder.MotionBorderData();
            tempMotionBorderData.ShotCoordinates = shotCoordinates;
            tempMotionBorderData.ShotBorder = shotBorder;
            tempMotionBorderData.Time = time;
            mMotionBorder.SetMotionBorderData(tempMotionBorderData);
        }
        private void mDoubleSquare_OnRefreshNeeded()
        {
            pbCalibrationWindow.Invalidate();
            pbCalibrationWindow.Refresh();
        }
        private void mDoubleSquare_OnCursorChanged(Cursor cursor)
        {
            mDoubleSquareCursor = cursor;
            if ((mTargetBorderCursor == Cursors.Default) && (mDoubleSquareCursor == Cursors.Default))
            {
                Cursor = Cursors.Default;
            }
            else if (mTargetBorderCursor == Cursors.Default)
            {
                Cursor = mDoubleSquareCursor;
            }
            else if (mDoubleSquareCursor == Cursors.Default)
            {
                Cursor = mTargetBorderCursor;
            }
        }
        void mDoubleSquare_OnDoubleSquareDataChanged(clsGraphicsDoubleSquare.DoubleSquareData doubleSquareData)
        {
            if (mDevice != null)
            {
                mDevice.Draw_DoubleSquare_CenterLocation = doubleSquareData.Center;
                mDevice.Draw_DoubleSquare_InnerSquareDiagonal = doubleSquareData.InnerDiagonal;
                mDevice.Draw_DoubleSquare_OuterSquareDiagonal = doubleSquareData.OuterDiagonal;
            }
        }
        private void mTargetBorder_OnRefreshNeeded()
        {
            pbCalibrationWindow.Invalidate();
            pbCalibrationWindow.Refresh();
        }
        private void mTargetBorder_OnCursorChanged(Cursor cursor)
        {
            mTargetBorderCursor = cursor;
            if ((mTargetBorderCursor == Cursors.Default) && (mDoubleSquareCursor == Cursors.Default))
            {
                Cursor = Cursors.Default;
            }
            else if (mTargetBorderCursor == Cursors.Default)
            {
                Cursor = mDoubleSquareCursor;
            }
            else if (mDoubleSquareCursor == Cursors.Default)
            {
                Cursor = mTargetBorderCursor;
            }
        }
        void mTargetBorder_OnTargetBorderDataChanged(clsGraphicsTargetBorder.TargetBorderData targetBorderData)
        {
            mDevice.Draw_TargetBorder_Border = targetBorderData.TargetBorder;
        }
        private void pbCalibrationWindow_MouseDown(object sender, MouseEventArgs e)
        {
            mDoubleSquare.OnMouseDown((float)e.X / (float)pbCalibrationWindow.Width, (float)e.Y / (float)pbCalibrationWindow.Height);
            mTargetBorder.OnMouseDown((float)e.X / (float)pbCalibrationWindow.Width, (float)e.Y / (float)pbCalibrationWindow.Height);
        }
        private void pbCalibrationWindow_MouseUp(object sender, MouseEventArgs e)
        {
            mDoubleSquare.OnMouseUp((float)e.X / (float)pbCalibrationWindow.Width, (float)e.Y / (float)pbCalibrationWindow.Height);
            mTargetBorder.OnMouseUp((float)e.X / (float)pbCalibrationWindow.Width, (float)e.Y / (float)pbCalibrationWindow.Height);
        }
        private void pbCalibrationWindow_MouseMove(object sender, MouseEventArgs e)
        {
            mDoubleSquare.OnMouseMove((float)e.X / (float)pbCalibrationWindow.Width, (float)e.Y / (float)pbCalibrationWindow.Height);
            mTargetBorder.OnMouseMove((float)e.X / (float)pbCalibrationWindow.Width, (float)e.Y / (float)pbCalibrationWindow.Height);
        }
        private void pbCalibrationWindow_Paint(object sender, PaintEventArgs e)
        {
            if (mDevice != null)
            {
                Bitmap bmp = mDevice.GetRGBPicture(true);
                if (bmp != null)
                {
                    e.Graphics.DrawImage(bmp, new Rectangle(0, 0, this.Width, this.Height), new Rectangle(0, 0, bmp.Width, bmp.Height), GraphicsUnit.Pixel);
                    bmp.Dispose();
                    bmp = null;
                }
            }
            mDoubleSquare.OnPaint(e.Graphics, this.Width, this.Height);
            mTargetBorder.OnPaint(e.Graphics, this.Width, this.Height);
            mMotionBorder.OnPaint(e.Graphics, this.Width, this.Height);
        }
        private void ucCalibration_Resize(object sender, EventArgs e)
        {
            pbCalibrationWindow.Invalidate();
            pbCalibrationWindow.Refresh();
        }
        #endregion
    }
}
