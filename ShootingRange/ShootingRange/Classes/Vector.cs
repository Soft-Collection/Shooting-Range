using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ShootingRange
{
    public class Vector
    {
        #region Variables
        public double X = 0.0f;
        public double Y = 0.0f;
        #endregion

        #region Constructor/Destructor
        public Vector(double x, double y)
        {
            X = x;
            Y = y;
        }
        public Vector(PointF from, PointF to)
        {
            X = to.X - from.X;
            Y = to.Y - from.Y;
        }
        #endregion

        #region Methods

        #region Vector Only
        public static Vector ScalarMultiply(double scalar, Vector vector)
        {
            return (new Vector(scalar * vector.X, scalar * vector.Y));
        }
        public static Vector VectorSum(Vector vector1, Vector vector2)
        {
            return (new Vector(vector1.X + vector2.X, vector1.Y + vector2.Y));
        }
        public static Vector VectorSubtract(Vector vector1, Vector vector2)
        {
            return (new Vector(vector1.X - vector2.X, vector1.Y - vector2.Y));
        }
        public static double VectorLength(Vector vector)
        {
            return (Math.Sqrt((vector.X * vector.X) + (vector.Y * vector.Y)));
        }
        public static double VectorRelativeLength(Vector vector, Vector length1Vec)
        {
            return (VectorLength(vector) / VectorLength(length1Vec));
        }
        public static double VectorDotProduct(Vector vector1, Vector vector2)
        {
            return ((vector1.X * vector2.X) + (vector1.Y * vector2.Y));
        }
        #endregion

        #region Vector And Point
        public static PointF PointPlusVector(PointF pt, Vector vector)
        {
            return (new PointF(pt.X + (float)vector.X, pt.Y + (float)vector.Y));
        }
        public static PointF PointPlusVectorCoefficient(PointF pt, Vector length1Vec, double coefficient)
        {
            return (PointPlusVector(pt, ScalarMultiply(coefficient, length1Vec)));
        }
        public static PointF[] BoundedPolygon(PointF[] polygon, double lower_bound, double upper_bound)
        {
            PointF lower_bound_01 = PointPlusVectorCoefficient(polygon[0], new Vector(polygon[0], polygon[1]), lower_bound);
            PointF upper_bound_01 = PointPlusVectorCoefficient(polygon[0], new Vector(polygon[0], polygon[1]), upper_bound);
            PointF lower_bound_02 = PointPlusVectorCoefficient(polygon[3], new Vector(polygon[3], polygon[2]), lower_bound);
            PointF upper_bound_02 = PointPlusVectorCoefficient(polygon[3], new Vector(polygon[3], polygon[2]), upper_bound);
            return (new PointF[] { lower_bound_01, upper_bound_01, upper_bound_02, lower_bound_02 });
        }
        public static bool IsPointInBoundedPolygon(PointF[] polygon, double lower_bound, double upper_bound, PointF pt)
        {
            return (IsPointInPolygon(BoundedPolygon(polygon, lower_bound, upper_bound), pt));
        }
        #endregion

        #region Point Only
        //http://www.blackpawn.com/texts/pointinpoly/default.html
        //         d
        //        /
        // b-----/------c
        //      /
        //     /
        //    a
        public static bool AreTwoLinesIntersects(PointF a, PointF b, PointF c, PointF d)
        {
            //Compute vectors.
            PointF ab = new PointF();
            ab.X = b.X - a.X;
            ab.Y = b.Y - a.Y;
            PointF ac = new PointF();
            ac.X = c.X - a.X;
            ac.Y = c.Y - a.Y;
            PointF ad = new PointF();
            ad.X = d.X - a.X;
            ad.Y = d.Y - a.Y;
            //Compute dot products.
            float dot_ac_ac = ac.X * ac.X + ac.Y * ac.Y;
            float dot_ac_ab = ac.X * ab.X + ac.Y * ab.Y;
            float dot_ac_ad = ac.X * ad.X + ac.Y * ad.Y;
            float dot_ab_ab = ab.X * ab.X + ab.Y * ab.Y;
            float dot_ab_ad = ab.X * ad.X + ab.Y * ad.Y;
            //Compute barycentric coordinates.
            float invDenom = 1 / (dot_ac_ac * dot_ab_ab - dot_ac_ab * dot_ac_ab);
            float u = (dot_ab_ab * dot_ac_ad - dot_ac_ab * dot_ab_ad) * invDenom;
            float v = (dot_ac_ac * dot_ab_ad - dot_ac_ab * dot_ac_ad) * invDenom;
            //Check if point is in triangle.
            return ((u > 0) && (v > 0) && (u + v > 1));
        }
        public static bool ArePolygonAndLineIntersects(PointF[] polygon, PointF a, PointF b)
        {
            for (int i = 0; i < polygon.Length; i++)
            {
                if (AreTwoLinesIntersects(a, polygon[i], polygon[(i + 1) % polygon.Length], b)) return (true);
            }
            return (false);
        }
        public static bool IsPointInPolygon(PointF[] polygon, PointF pt)
        {
            PointF inPolygonPoint = new PointF((polygon[0].X + polygon[1].X + polygon[2].X + polygon[3].X) / 4, (polygon[0].Y + polygon[1].Y + polygon[2].Y + polygon[3].Y) / 4);
            return (!ArePolygonAndLineIntersects(polygon, inPolygonPoint, pt));
        }
        public static float DistanceBetweenTwoPoints(PointF pt1, PointF pt2)
        {
            return ((float)Math.Sqrt(((pt2.X - pt1.X) * (pt2.X - pt1.X)) + ((pt2.Y - pt1.Y) * (pt2.Y - pt1.Y))));
        }
        #endregion

        #region Tools
        public static bool Translate_TargetBorder_0P1(PointF[] targetBorder, PointF pt, float acceptedDistance, out PointF targetPoint)
        {
            targetPoint = new PointF();
            //------------------------------------------------------
            if (!IsPointInBoundedPolygon(targetBorder, 0.0f, 1.0f, pt)) return false;
            //------------------------------------------------------
            double lower_bound = 0.0f;
            double last_upper_bound = 1.0f;
            double upper_bound = 0.5f;
            //------------------------------------------------------
            while (upper_bound - lower_bound > acceptedDistance)
            {
                if (IsPointInBoundedPolygon(targetBorder, lower_bound, upper_bound, pt))
                {
                    last_upper_bound = upper_bound;
                    upper_bound = (lower_bound + upper_bound) / 2;
                }
                else
                {
                    lower_bound = upper_bound;
                    upper_bound = (lower_bound + last_upper_bound) / 2;
                }
            }
            //------------------------------------------------------
            double middle = (lower_bound + upper_bound) / 2;
            float X = (float)middle;
            PointF tempPointAbove = PointPlusVectorCoefficient(targetBorder[0], new Vector(targetBorder[0], targetBorder[1]), middle);
            PointF tempPointBelow = PointPlusVectorCoefficient(targetBorder[3], new Vector(targetBorder[3], targetBorder[2]), middle);
            float Y = DistanceBetweenTwoPoints(tempPointAbove, pt) / DistanceBetweenTwoPoints(tempPointAbove, tempPointBelow);
            //------------------------------------------------------
            targetPoint = new PointF(X, Y);
            return true;
        }
        #endregion

        #endregion
    }
}
