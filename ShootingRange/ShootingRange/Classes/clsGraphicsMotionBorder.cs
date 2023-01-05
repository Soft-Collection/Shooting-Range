using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ShootingRange
{
    public class clsGraphicsMotionBorder
    {
        #region Inner Classes
        public class MotionBorderConfig
        {
            public float AveragePointDiagonal = 0.01f;
            //----------------------------------------
            public int LineThickness = 1;
            public Color LineColor = Color.Yellow;
        }
        public class MotionBorderData
        {
            public PointF[] ShotBorder = new PointF[4];
            public PointF ShotCoordinates;
            public DateTime Time = DateTime.Now;
        }
        #endregion

        #region Delegates
        #endregion

        #region Events
        #endregion

        #region Enums
        public enum eStates
        {
            Normal,
            Changing
        }
        #endregion

        #region Constants
        #endregion

        #region Variables
        private MotionBorderConfig mMotionBorderConfig = new MotionBorderConfig();
        private MotionBorderData mMotionBorderData = new MotionBorderData();
        private object mMotionBorderDataLock = new object();
        #endregion

        #region Properties
        #endregion

        #region Constructor / Destructor
        #endregion

        #region Methods

        #region Static Methods
        #endregion

        #region Non-static Methods
        public void SetMotionBorderData(MotionBorderData motionBorderData)
        {
            lock (mMotionBorderDataLock)
            {
                mMotionBorderData = motionBorderData;
            }
        }
        #endregion

        #region Draw
        private void Draw(Graphics g, int width, int height)
        {
            MotionBorderData tempMotionBorderData = null;
            //-----------------------------------------------------------------
            lock (mMotionBorderDataLock)
            {
                tempMotionBorderData = mMotionBorderData;
            }
            //-----------------------------------------------------------------
            if (DateTime.Now.Subtract(tempMotionBorderData.Time).TotalMilliseconds > 200) return;
            //-----------------------------------------------------------------
            g.SmoothingMode = SmoothingMode.AntiAlias;
            //-----------------------------------------------------------------
            List<PointF> polygon = new List<PointF>(tempMotionBorderData.ShotBorder);
            clsGraphicsCommon.Translate_0P1_ToRealDimensions(polygon, width, height);
            //-----------------------------------------------------------------
            List<PointF[]> linesCenter = clsGraphicsCommon.GetCenterLines(mMotionBorderData.ShotCoordinates, mMotionBorderConfig.AveragePointDiagonal);
            clsGraphicsCommon.Translate_0P1_ToRealDimensions(linesCenter, width, height);
            //-----------------------------------------------------------------
            g.DrawPolygon(new Pen(mMotionBorderConfig.LineColor, mMotionBorderConfig.LineThickness), polygon.ToArray());
            //-----------------------------------------------------------------
            for (int i = 0; i < linesCenter.Count; i++)
            {
                g.DrawLine(new Pen(mMotionBorderConfig.LineColor, mMotionBorderConfig.LineThickness), linesCenter[i][0], linesCenter[i][1]);
            }
        }
        #endregion

        #region OnEvents
        public void OnPaint(Graphics g, int width, int height)
        {
            Draw(g, width, height);
        }
        #endregion

        #endregion
    }
}