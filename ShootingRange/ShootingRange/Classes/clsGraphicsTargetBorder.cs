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
    public class clsGraphicsTargetBorder
    {
        #region Inner Classes
        public class TargetBorderConfig
        {
            public int LineThickness = 1;
            public Color LineColor = Color.Yellow;
            //----------------------------------------
            public float CornerCircleRadiusDiagonal = 0.01f;
            public int CornerCircleBorderThickness = 1;
            public Color CornerFirstCircleFillColor = Color.Red;
            public Color CornerOtherCircleFillColor = Color.Yellow;
            public Color CornerCircleBorderColor = Color.Yellow;
        }
        public class TargetBorderData
        {
            public PointF[] TargetBorder = new PointF[4];
        }
        #endregion

        #region Delegates
        public delegate void dOnCursorChanged(Cursor cursor);
        public delegate void dOnRefreshNeeded();
        public delegate void dOnTargetBorderDataChanged(TargetBorderData targetBorderData);
        #endregion

        #region Events
        public event dOnCursorChanged OnCursorChanged = null;
        public event dOnRefreshNeeded OnRefreshNeeded = null;
        public event dOnTargetBorderDataChanged OnTargetBorderDataChanged = null;
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
        private TargetBorderConfig mTargetBorderConfig = new TargetBorderConfig();
        private TargetBorderData mTargetBorderData = new TargetBorderData();
        private eStates mState = eStates.Normal;
        private int mLastIndex = -1;
        #endregion

        #region Properties
        #endregion

        #region Constructor / Destructor
        #endregion

        #region Methods

        #region Static Methods
        #endregion

        #region Non-static Methods
        private void IsPointOnSignificantPoints(PointF point, out int index)
        {
            index = -1;
            //-----------------------------------------------------------------
            for (int i = 0; i < mTargetBorderData.TargetBorder.Length; i++)
            {
                if (clsGraphicsCommon.IsPointOnThePoint(point, mTargetBorderData.TargetBorder[i], mTargetBorderConfig.CornerCircleRadiusDiagonal))
                {
                    index = i;
                }
            }
        }
        public void SetTargetBorderData(TargetBorderData targetBorderData)
        {
            mTargetBorderData = targetBorderData;
        }
        #endregion

        #region Draw
        private void Draw(Graphics g, int width, int height)
        {
            //-----------------------------------------------------------------
            g.SmoothingMode = SmoothingMode.AntiAlias;
            //-----------------------------------------------------------------
            List<PointF> polygon = new List<PointF>(mTargetBorderData.TargetBorder);
            List<RectangleF> ellipseRectsPolygon = new List<RectangleF>();
            for (int i = 0; i < polygon.Count; i++)
            {
                ellipseRectsPolygon.Add(clsGraphicsCommon.GetEllipseRectangleFromCenterAndRadius(polygon[i], mTargetBorderConfig.CornerCircleRadiusDiagonal));
            }
            polygon.Add(polygon[0]);
            clsGraphicsCommon.Translate_0P1_ToRealDimensions(polygon, width, height);
            clsGraphicsCommon.Translate_0P1_ToRealDimensions(ellipseRectsPolygon, width, height);
            //-----------------------------------------------------------------
            g.DrawPolygon(new Pen(mTargetBorderConfig.LineColor, mTargetBorderConfig.LineThickness), polygon.ToArray());
            //-----------------------------------------------------------------
            for (int i = 0; i < ellipseRectsPolygon.Count; i++)
            {
                g.FillEllipse(new SolidBrush((i == 0) ? mTargetBorderConfig.CornerFirstCircleFillColor : mTargetBorderConfig.CornerOtherCircleFillColor), ellipseRectsPolygon[i]);
                g.DrawEllipse(new Pen(mTargetBorderConfig.CornerCircleBorderColor, mTargetBorderConfig.CornerCircleBorderThickness), ellipseRectsPolygon[i]);
                //-----------------------------------------------------------------
                float horizontalCoefficient = width * mTargetBorderConfig.CornerCircleRadiusDiagonal;
                float verticalCoefficient = height * mTargetBorderConfig.CornerCircleRadiusDiagonal;
                float horizontalOffset = 0.0f;
                float verticalOffset = 0.0f;
                switch (i)
                {
                    case 0:
                        horizontalOffset = -2.0f * horizontalCoefficient;
                        verticalOffset = -2.5f * verticalCoefficient;
                        break;
                    case 1:
                        horizontalOffset = 2.0f * horizontalCoefficient;
                        verticalOffset = -2.5f * verticalCoefficient;
                        break;
                    case 2:
                        horizontalOffset = 2.0f * horizontalCoefficient;
                        verticalOffset = 1.5f * verticalCoefficient;
                        break;
                    case 3:
                        horizontalOffset = -2.0f * horizontalCoefficient;
                        verticalOffset = 1.5f * verticalCoefficient;
                        break;
                }
                g.DrawString((i + 1).ToString(), new Font("Arial", 2 * verticalCoefficient), new SolidBrush(mTargetBorderConfig.CornerCircleBorderColor), new PointF(ellipseRectsPolygon[i].X + horizontalOffset, ellipseRectsPolygon[i].Y + verticalOffset));
            }
            //-----------------------------------------------------------------
        }
        #endregion

        #region OnEvents
        public void OnPaint(Graphics g, int width, int height)
        {
            Draw(g, width, height);
        }
        public void OnMouseMove(float x, float y)
        {
            switch (mState)
            {
                case eStates.Normal:
                    OnNormal(x, y);
                    break;
                case eStates.Changing:
                    OnChangeTargetBorder(x, y);
                    if (OnRefreshNeeded != null)
                    {
                        OnRefreshNeeded();
                    }
                    break;
            }
        }
        private void OnNormal(float x, float y)
        {
            int index;
            IsPointOnSignificantPoints(new PointF(x, y), out index);
            if (index >= 0)
            {
                mLastIndex = index;
                if (OnCursorChanged != null)
                {
                    OnCursorChanged(Cursors.Hand);
                }
            }
            else
            {
                if (OnCursorChanged != null)
                {
                    OnCursorChanged(Cursors.Default);
                }
            }
        }
        private void OnChangeTargetBorder(float x, float y)
        {
            if (mLastIndex >= 0)
            {
                PointF tempPoint = new PointF(x, y);
                double angleInRad = clsGraphicsCommon.GetAngle(mTargetBorderData.TargetBorder[(mLastIndex + 4 - 1) % 4], tempPoint, mTargetBorderData.TargetBorder[(mLastIndex + 4 + 1) % 4]);
                double angleInDeg = angleInRad * (180.0 / Math.PI);
                if (angleInDeg < 130)
                {
                    mTargetBorderData.TargetBorder[mLastIndex].X = Math.Max(Math.Min(x, 1), 0);
                    mTargetBorderData.TargetBorder[mLastIndex].Y = Math.Max(Math.Min(y, 1), 0);
                }
            }
        }
        public void OnMouseDown(float x, float y)
        {
            int index;
            IsPointOnSignificantPoints(new PointF(x, y), out index);
            if (index >= 0)
            {
                mState = eStates.Changing;
            }
        }
        public void OnMouseUp(float x, float y)
        {
            mState = eStates.Normal;
            if (OnTargetBorderDataChanged != null)
            {
                OnTargetBorderDataChanged(mTargetBorderData);
            }
        }
        #endregion

        #endregion
    }
}