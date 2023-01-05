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
    public class clsGraphicsCommon
    {
        #region Variables
        #endregion

        #region Static Methods
        //http://mathworld.wolfram.com/Point-LineDistance2-Dimensional.html
        public static bool IsPointOnTheLine(PointF point, PointF[] line, float distance)
        {
            double x0 = point.X;
            double y0 = point.Y;
            double x1 = line[0].X;
            double y1 = line[0].Y;
            double x2 = line[1].X;
            double y2 = line[1].Y;
            //-----------------------------------------
            double distanceFromLine = Math.Abs((x2 - x1) * (y1 - y0) - (x1 - x0) * (y2 - y1)) / Math.Sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)); //Formula (14).
            //-----------------------------------------
            return (distanceFromLine < distance);
        }
        public static float DistanceBetweenPoints(PointF point1, PointF point2)
        {
            return (float)Math.Sqrt(Math.Pow((double)point2.X - (double)point1.X, 2) + Math.Pow((double)point2.Y - (double)point1.Y, 2));
        }
        public static bool IsPointOnThePoint(PointF point, PointF pointOfSquare, float distance)
        {
            return (DistanceBetweenPoints(point, pointOfSquare) < distance);
        }
        public static List<PointF> GetSquarePoints(PointF center, float diagonal)
        {
            List<PointF> retVal = new List<PointF>();
            float sqrt2 = diagonal / (float)Math.Sqrt(2);
            retVal.Add(new PointF(center.X - sqrt2, center.Y - sqrt2));
            retVal.Add(new PointF(center.X + sqrt2, center.Y - sqrt2));
            retVal.Add(new PointF(center.X + sqrt2, center.Y + sqrt2));
            retVal.Add(new PointF(center.X - sqrt2, center.Y + sqrt2));
            return retVal;
        }
        public static List<PointF[]> GetSquareLines(PointF center, float diagonal)
        {
            List<PointF[]> retVal = new List<PointF[]>();
            List<PointF> points = GetSquarePoints(center, diagonal);
            for (int i = 0; i < points.Count; i++)
            {
                retVal.Add(new PointF[] { points[i], (i < points.Count - 1) ? points[i + 1] : points[0] });
            }
            return retVal;
        }
        public static List<PointF[]> GetCenterLines(PointF center, float diagonal)
        {
            List<PointF[]> retVal = new List<PointF[]>();
            retVal.Add(new PointF[] { new PointF(center.X - diagonal, center.Y), new PointF(center.X + diagonal, center.Y) });
            retVal.Add(new PointF[] { new PointF(center.X, center.Y - diagonal), new PointF(center.X, center.Y + diagonal) });
            return retVal;
        }
        public static RectangleF GetEllipseRectangleFromCenterAndRadius(PointF center, float diagonal)
        {
            List<PointF> tempRectPoints = clsGraphicsCommon.GetSquarePoints(center, diagonal);
            PointF tempLocation = tempRectPoints[0];
            SizeF tempSize = new SizeF(tempRectPoints[2].X - tempRectPoints[0].X, tempRectPoints[2].Y - tempRectPoints[0].Y);
            RectangleF tempRect = new RectangleF(tempLocation,tempSize);
            return (tempRect);
        }
        //http://www.wikihow.com/Find-the-Angle-Between-Two-Vectors
        public static double GetAngle(PointF p0, PointF center, PointF p1)
        {
            PointF v = new PointF(p0.X - center.X, p0.Y - center.Y);
            PointF u = new PointF(p1.X - center.X, p1.Y - center.Y);
            //------------------------------------------------------
            double IvI = Math.Sqrt((v.X * v.X) + (v.Y * v.Y));
            double IuI = Math.Sqrt((u.X * u.X) + (u.Y * u.Y));
            //------------------------------------------------------
            double dot_v_u = (v.X * u.X) + (v.Y * u.Y);
            return Math.Acos(dot_v_u / (IvI * IuI));
        }
        public static PointF Translate_M1P1_0P1(PointF pt) //Translate from [-1..1] to [0..1].
        {
            return new PointF((pt.X + 1) / 2, (pt.Y + 1) / 2);
        }
        public static PointF Translate_0P1_M1P1(PointF pt) //Translate from [0..1] to [-1..1].
        {
            return new PointF((pt.X * 2) - 1, (pt.Y * 2) - 1);
        }
        public static PointF Translate_0P1_P10(PointF pt) //Translate from [0..1] to [1..0].
        {
            return new PointF(1 - pt.X, pt.Y);
        }
        public static void Translate_0P1_ToRealDimensions(ref PointF pt, int width, int height)
        {
            pt.X *= (float)width;
            pt.Y *= (float)height;
        }
        public static void Translate_0P1_ToRealDimensions(PointF[] pts, int width, int height)
        {
            for (int i = 0; i < pts.Length; i++)
            {
                Translate_0P1_ToRealDimensions(ref pts[i], width, height);
            }
        }
        public static void Translate_0P1_ToRealDimensions(List<PointF> lpt, int width, int height)
        {
            for (int i = 0; i < lpt.Count; i++)
            {
                PointF pt = lpt[i];
                Translate_0P1_ToRealDimensions(ref pt, width, height);
                lpt[i] = pt;
            }
        }
        public static void Translate_0P1_ToRealDimensions(List<PointF[]> lpts, int width, int height)
        {
            for (int i = 0; i < lpts.Count; i++)
            {
                PointF[] pts = lpts[i];
                Translate_0P1_ToRealDimensions(lpts[i], width, height);
                lpts[i] = pts;
            }
        }
        public static void Translate_0P1_ToRealDimensions(ref RectangleF rc, int width, int height)
        {
            rc.X *= (float)width;
            rc.Y *= (float)height;
            rc.Width *= (float)width;
            rc.Height *= (float)height;
        }
        public static void Translate_0P1_ToRealDimensions(List<RectangleF> lrc, int width, int height)
        {
            for (int i = 0; i < lrc.Count; i++)
            {
                RectangleF rc = lrc[i];
                Translate_0P1_ToRealDimensions(ref rc, width, height);
                lrc[i] = rc;
            }
        }
        #endregion
    }
}