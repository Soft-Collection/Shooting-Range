#include "stdafx.h"
#include "CVMDGeometry.h"

/*************************************
PolygonF Class Implementation.
*************************************/

PolygonF::PolygonF(): mLength(0), mCapacity(0), mPoints(NULL) { }

PolygonF::~PolygonF() 
{
	delete [] mPoints;
}

PolygonF::PolygonF(const PolygonF& r)
{
	*this = r;
}

PolygonF& PolygonF::operator=(const PolygonF& r)
{
	return(DeepClone(r));
}

POINTF& PolygonF::operator [] (int offset) const
{
	if ((offset < 0) || (offset >= mLength)) throw IndexOutOfRangeException();
	return mPoints[offset];
}

PolygonF& PolygonF::DeepClone(const PolygonF& r)
{
	if (this != &r)
	{
		mLength = r.mLength;
		mCapacity = r.mCapacity;
		mPoints = new POINTF[mCapacity];
		for (int i = 0; i < mCapacity; i++) mPoints[i] = r.mPoints[i];
	}
	return *this;
}

void PolygonF::Add(POINTF& point)
{
	if (mLength == mCapacity)
	{
		POINTF* tempPoints = mPoints;
		mCapacity += 10;
		mPoints = new POINTF[mCapacity];
		for (int i = 0; i < mCapacity; i++)
		{
			if (i < mLength) mPoints[i] = tempPoints[i];
			else {mPoints[i].X = 0; mPoints[i].Y = 0;}
		}
		delete [] tempPoints;
	}
	mPoints[mLength++] = point;
}

/*************************************
Geometry Class Implementation.
*************************************/

//http://www.blackpawn.com/texts/pointinpoly/default.html
// b 
// |\
// | \
// |.p\
// |___\
// a    c
bool Geometry::IsPointInTriangle(const POINTF& a, const POINTF& b, const POINTF& c, const POINTF& p)
{
	//Compute vectors.
    POINTF ab;
	ab.X = b.X - a.X;
	ab.Y = b.Y - a.Y;
    POINTF ac;
	ac.X = c.X - a.X;
	ac.Y = c.Y - a.Y;
    POINTF ap;
	ap.X = p.X - a.X;
	ap.Y = p.Y - a.Y;
    //Compute dot products.
    float dot_ac_ac = ac.X * ac.X + ac.Y * ac.Y;
    float dot_ac_ab = ac.X * ab.X + ac.Y * ab.Y;
    float dot_ac_ap = ac.X * ap.X + ac.Y * ap.Y;
    float dot_ab_ab = ab.X * ab.X + ab.Y * ab.Y;
    float dot_ab_ap = ab.X * ap.X + ab.Y * ap.Y;
    //Compute barycentric coordinates.
    float invDenom = 1 / (dot_ac_ac * dot_ab_ab - dot_ac_ab * dot_ac_ab);
    float u = (dot_ab_ab * dot_ac_ap - dot_ac_ab * dot_ab_ap) * invDenom;
    float v = (dot_ac_ac * dot_ab_ap - dot_ac_ab * dot_ac_ap) * invDenom;
    //Check if point is in triangle.
    return((u > 0) && (v > 0) && (u + v < 1));
}

//http://www.blackpawn.com/texts/pointinpoly/default.html
//         d
//        /
// b-----/------c
//      /
//     /
//    a
bool Geometry::AreTwoLinesIntersects(const POINTF& a, const POINTF& b, const POINTF& c, const POINTF& d)
{
	//Compute vectors.
    POINTF ab;
	ab.X = b.X - a.X;
	ab.Y = b.Y - a.Y;
    POINTF ac;
	ac.X = c.X - a.X;
	ac.Y = c.Y - a.Y;
    POINTF ad;
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
    return((u > 0) && (v > 0) && (u + v > 1));
}

bool Geometry::ArePolygonAndLineIntersects(const PolygonF& polygon, const POINTF& a, const POINTF& b)
{
	for (int i=0;i<polygon.Length();i++)
	{
		if(AreTwoLinesIntersects(a,polygon[i],polygon[(i+1)%polygon.Length()],b)) return(true);
	}
	return(false);
}

bool Geometry::ArePolygonAndRectangleIntersects(const PolygonF& polygon, const RECTANGLEF& rectangle)
{
	POINTF leftTop;
	leftTop.X = rectangle.left;
	leftTop.Y = rectangle.top;
	POINTF rightTop;
	rightTop.X = rectangle.right;
	rightTop.Y = rectangle.top;
	POINTF rightBottom;
	rightBottom.X = rectangle.right;
	rightBottom.Y = rectangle.bottom;
	POINTF leftBottom;
	leftBottom.X = rectangle.left; 
	leftBottom.Y = rectangle.bottom;
	POINTF leftTopCorner;
	leftTopCorner.X = 0;
	leftTopCorner.Y = 0;
	//1. Side of Polygon intersects with a side of Rectangle.
	if(ArePolygonAndLineIntersects(polygon,leftTop,rightTop)) return(true);
	if(ArePolygonAndLineIntersects(polygon,rightTop,rightBottom)) return(true);
	if(ArePolygonAndLineIntersects(polygon,rightBottom,leftBottom)) return(true);
	if(ArePolygonAndLineIntersects(polygon,leftBottom,leftTop)) return(true);
	//2. Rectangle is inside the Polygon.
	int intersectionsCounter;
	intersectionsCounter = 0;
	for (int i=0;i<polygon.Length();i++)
	{
		if(AreTwoLinesIntersects(leftTopCorner,polygon[i],polygon[(i+1)%polygon.Length()],leftTop)) intersectionsCounter++;
	}
	if (intersectionsCounter % 2 == 1) return(true);
	//3. Polygon is inside the Rectangle.
	intersectionsCounter = 0;
	if(AreTwoLinesIntersects(leftTopCorner,leftTop,rightTop,polygon[0])) intersectionsCounter++;
	if(AreTwoLinesIntersects(leftTopCorner,rightTop,rightBottom,polygon[0])) intersectionsCounter++;
	if(AreTwoLinesIntersects(leftTopCorner,rightBottom,leftBottom,polygon[0])) intersectionsCounter++;
	if(AreTwoLinesIntersects(leftTopCorner,leftBottom,leftTop,polygon[0])) intersectionsCounter++;
	if (intersectionsCounter % 2 == 1) return(true);
	return(false);
}