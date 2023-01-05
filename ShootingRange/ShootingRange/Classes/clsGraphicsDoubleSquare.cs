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
    public class clsGraphicsDoubleSquare
    {
        #region Inner Classes
        public class DoubleSquareConfig
        {
            public float CenterDiagonal = 0.01f;
            //----------------------------------------
            public int LineThickness = 1;
            public Color LineColor = Color.Yellow;
            //----------------------------------------
            public float CornerCircleRadiusDiagonal = 0.01f;
            public int CornerCircleBorderThickness = 1;
            public Color CornerCircleBorderColor = Color.Yellow;
            public Color CornerCircleFillColor = Color.Yellow;
        }
        public class DoubleSquareData
        {
            public PointF Center;
            public float OuterDiagonal = 0;
            public float InnerDiagonal = 0;
        }
        #endregion

        #region Delegates
        public delegate void dOnCursorChanged(Cursor cursor);
        public delegate void dOnRefreshNeeded();
        public delegate void dOnDoubleSquareDataChanged(DoubleSquareData doubleSquareData);
        #endregion

        #region Events
        public event dOnCursorChanged OnCursorChanged = null;
        public event dOnRefreshNeeded OnRefreshNeeded = null;
        public event dOnDoubleSquareDataChanged OnDoubleSquareDataChanged = null;
        #endregion

        #region Enums
        public enum eStates
        {
            Normal,
            Resizing,
            Moving
        }
        public enum ePointTypes
        {
            Inner,
            Outer,
            Center
        }
        #endregion

        #region Constants
        #endregion

        #region Variables
        private DoubleSquareConfig mDoubleSquareConfig = new DoubleSquareConfig();
        private DoubleSquareData mDoubleSquareData = new DoubleSquareData();
        private eStates mState = eStates.Normal;
        private ePointTypes mPointType = ePointTypes.Center;
        #endregion

        #region Properties
        #endregion

        #region Constructor / Destructor
        #endregion

        #region Methods

        #region Static Methods
        #endregion

        #region Non-static Methods
        private void IsPointOnSignificantPoints(PointF point, out int index, out ePointTypes squareType)
        {
            index = -1;
            squareType = ePointTypes.Outer;
            //-----------------------------------------------------------------
            List<PointF> pointsOuter = clsGraphicsCommon.GetSquarePoints(mDoubleSquareData.Center, mDoubleSquareData.OuterDiagonal);
            List<PointF> pointsInner = clsGraphicsCommon.GetSquarePoints(mDoubleSquareData.Center, mDoubleSquareData.InnerDiagonal);
            //-----------------------------------------------------------------
            for (int i = 0; i < pointsOuter.Count; i++)
            {
                if (clsGraphicsCommon.IsPointOnThePoint(point, pointsOuter[i], mDoubleSquareConfig.CornerCircleRadiusDiagonal))
                {
                    index = i;
                    squareType = ePointTypes.Outer;
                }
            }
            for (int i = 0; i < pointsInner.Count; i++)
            {
                if (clsGraphicsCommon.IsPointOnThePoint(point, pointsInner[i], mDoubleSquareConfig.CornerCircleRadiusDiagonal))
                {
                    index = i;
                    squareType = ePointTypes.Inner;
                }
            }
            if (clsGraphicsCommon.IsPointOnThePoint(point, mDoubleSquareData.Center, mDoubleSquareConfig.CornerCircleRadiusDiagonal))
            {
                squareType = ePointTypes.Center;
            }
        }
        public void SetDoubleSquareData(DoubleSquareData doubleSquareData)
        {
            mDoubleSquareData = doubleSquareData;
        }
        #endregion

        #region Draw
        private void Draw(Graphics g, int width, int height)
        {
            //-----------------------------------------------------------------
            g.SmoothingMode = SmoothingMode.AntiAlias;
            //-----------------------------------------------------------------
            List<PointF[]> linesCenter = clsGraphicsCommon.GetCenterLines(mDoubleSquareData.Center, mDoubleSquareConfig.CenterDiagonal);
            clsGraphicsCommon.Translate_0P1_ToRealDimensions(linesCenter, width, height);
            //-----------------------------------------------------------------
            List<PointF[]> linesOuter = clsGraphicsCommon.GetSquareLines(mDoubleSquareData.Center, mDoubleSquareData.OuterDiagonal);
            clsGraphicsCommon.Translate_0P1_ToRealDimensions(linesOuter, width, height);
            List<PointF[]> linesInner = clsGraphicsCommon.GetSquareLines(mDoubleSquareData.Center, mDoubleSquareData.InnerDiagonal);
            clsGraphicsCommon.Translate_0P1_ToRealDimensions(linesInner, width, height);
            //-----------------------------------------------------------------
            List<PointF> pointsOuter = clsGraphicsCommon.GetSquarePoints(mDoubleSquareData.Center, mDoubleSquareData.OuterDiagonal);
            List<RectangleF> ellipseRectsOuter = new List<RectangleF>();
            for (int i = 0; i < pointsOuter.Count; i++)
            {
                ellipseRectsOuter.Add(clsGraphicsCommon.GetEllipseRectangleFromCenterAndRadius(pointsOuter[i], mDoubleSquareConfig.CornerCircleRadiusDiagonal));
            }
            clsGraphicsCommon.Translate_0P1_ToRealDimensions(pointsOuter, width, height);
            clsGraphicsCommon.Translate_0P1_ToRealDimensions(ellipseRectsOuter, width, height);
            //-----------------------------------------------------------------
            List<PointF> pointsInner = clsGraphicsCommon.GetSquarePoints(mDoubleSquareData.Center, mDoubleSquareData.InnerDiagonal);
            List<RectangleF> ellipseRectsInner = new List<RectangleF>();
            for (int i = 0; i < pointsInner.Count; i++)
            {
                ellipseRectsInner.Add(clsGraphicsCommon.GetEllipseRectangleFromCenterAndRadius(pointsInner[i], mDoubleSquareConfig.CornerCircleRadiusDiagonal));
            }
            clsGraphicsCommon.Translate_0P1_ToRealDimensions(pointsInner, width, height);
            clsGraphicsCommon.Translate_0P1_ToRealDimensions(ellipseRectsInner, width, height);
            //-----------------------------------------------------------------
            for (int i = 0; i < linesCenter.Count; i++)
            {
                g.DrawLine(new Pen(mDoubleSquareConfig.LineColor, mDoubleSquareConfig.LineThickness), linesCenter[i][0], linesCenter[i][1]);
            }
            //-----------------------------------------------------------------
            for (int i = 0; i < linesOuter.Count; i++)
            {
                g.DrawLine(new Pen(mDoubleSquareConfig.LineColor, mDoubleSquareConfig.LineThickness), linesOuter[i][0], linesOuter[i][1]);
            }
            for (int i = 0; i < linesInner.Count; i++)
            {
                g.DrawLine(new Pen(mDoubleSquareConfig.LineColor, mDoubleSquareConfig.LineThickness), linesInner[i][0], linesInner[i][1]);
            }
            //-----------------------------------------------------------------
            for (int i = 0; i < pointsOuter.Count; i++)
            {
                g.FillEllipse(new SolidBrush(mDoubleSquareConfig.CornerCircleFillColor), ellipseRectsOuter[i]);
                g.DrawEllipse(new Pen(mDoubleSquareConfig.CornerCircleBorderColor, mDoubleSquareConfig.CornerCircleBorderThickness), ellipseRectsOuter[i]);
            }
            for (int i = 0; i < pointsInner.Count; i++)
            {
                g.FillEllipse(new SolidBrush(mDoubleSquareConfig.CornerCircleFillColor), ellipseRectsInner[i]);
                g.DrawEllipse(new Pen(mDoubleSquareConfig.CornerCircleBorderColor, mDoubleSquareConfig.CornerCircleBorderThickness), ellipseRectsInner[i]);
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
                case eStates.Resizing:
                    OnResize(x, y);
                    if (OnRefreshNeeded != null)
                    {
                        OnRefreshNeeded();
                    }
                    break;
                case eStates.Moving:
                    OnMove(x, y);
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
            ePointTypes squareType;
            IsPointOnSignificantPoints(new PointF(x, y), out index, out squareType);
            switch (squareType)
            {
                case ePointTypes.Center:
                    if (OnCursorChanged != null)
                    {
                        OnCursorChanged(Cursors.SizeAll);
                    }
                    break;
                case ePointTypes.Inner:
                case ePointTypes.Outer:
                    if (index >= 0)
                    {
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
                    break;
            }
        }
        private void OnResize(float x, float y)
        {
            switch (mPointType)
            {
                case ePointTypes.Inner:
                    mDoubleSquareData.InnerDiagonal = clsGraphicsCommon.DistanceBetweenPoints(mDoubleSquareData.Center, new PointF(x, y));
                    if (mDoubleSquareData.InnerDiagonal < mDoubleSquareConfig.CornerCircleRadiusDiagonal) mDoubleSquareData.InnerDiagonal = mDoubleSquareConfig.CornerCircleRadiusDiagonal;
                    if (mDoubleSquareData.InnerDiagonal > mDoubleSquareData.OuterDiagonal - mDoubleSquareConfig.CornerCircleRadiusDiagonal) mDoubleSquareData.InnerDiagonal = mDoubleSquareData.OuterDiagonal - mDoubleSquareConfig.CornerCircleRadiusDiagonal;
                    break;
                case ePointTypes.Outer:
                    {
                        mDoubleSquareData.OuterDiagonal = clsGraphicsCommon.DistanceBetweenPoints(mDoubleSquareData.Center, new PointF(x, y));
                        if (mDoubleSquareData.OuterDiagonal < mDoubleSquareData.InnerDiagonal + mDoubleSquareConfig.CornerCircleRadiusDiagonal) mDoubleSquareData.OuterDiagonal = mDoubleSquareData.InnerDiagonal + mDoubleSquareConfig.CornerCircleRadiusDiagonal;
                        float dist1 = mDoubleSquareData.Center.X;
                        float dist2 = (float)1 - mDoubleSquareData.Center.X;
                        float dist3 = mDoubleSquareData.Center.Y;
                        float dist4 = (float)1 - mDoubleSquareData.Center.Y;
                        float minDist = Math.Min(dist1, Math.Min(dist2, Math.Min(dist3, dist4)));
                        float minDiag = minDist * (float)Math.Sqrt(2);
                        if (mDoubleSquareData.OuterDiagonal > minDiag) mDoubleSquareData.OuterDiagonal = minDiag;
                    }
                    break;
            }
        }
        private void OnMove(float x, float y)
        {
            List<PointF> retVal = new List<PointF>();
            float sqrt2 = mDoubleSquareData.OuterDiagonal / (float)Math.Sqrt(2);
            if (x < sqrt2) x = sqrt2;
            if (y < sqrt2) y = sqrt2;
            if (x > 1.0f - sqrt2) x = 1.0f - sqrt2;
            if (y > 1.0f - sqrt2) y = 1.0f - sqrt2;
            //------------------------------------------------------
            mDoubleSquareData.Center = new PointF(x, y);
        }
        public void OnMouseDown(float x, float y)
        {
            int index;
            ePointTypes squareType;
            IsPointOnSignificantPoints(new PointF(x, y), out index, out squareType);
            switch (squareType)
            {
                case ePointTypes.Center:
                    mState = eStates.Moving;
                    mPointType = ePointTypes.Center;
                    break;
                case ePointTypes.Inner:
                    if (index >= 0)
                    {
                        mState = eStates.Resizing;
                        mPointType = ePointTypes.Inner;
                    }
                    break;
                case ePointTypes.Outer:
                    if (index >= 0)
                    {
                        mState = eStates.Resizing;
                        mPointType = ePointTypes.Outer;
                    }
                    break;
            }
        }
        public void OnMouseUp(float x, float y)
        {
            mState = eStates.Normal;
            if (OnDoubleSquareDataChanged != null)
            {
                OnDoubleSquareDataChanged(mDoubleSquareData);
            }
        }
        #endregion

        #endregion
    }
}