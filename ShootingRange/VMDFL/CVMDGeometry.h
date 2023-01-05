#ifndef CVMDGEOMETRY_H
#define CVMDGEOMETRY_H

#include "CFFCommon.h"
#include "CVMDStructures.h"

class PolygonF
{
public:
	PolygonF(); //Constructor.
	~PolygonF(); //Destructor.
	PolygonF(const PolygonF& r); //Copy Constructor. Is called each time a temporary copy of the objest is put on the stack.
	PolygonF& operator = (const PolygonF& r);
	POINTF& operator [] (int offset) const;
	PolygonF& DeepClone(const PolygonF& r);
	void Add(POINTF& point);
	int Length() const {return mLength;}
	//Define the exception classes
	class IndexOutOfRangeException {};
private:
	int mLength;
	int mCapacity;
	POINTF* mPoints;
};

class Geometry
{
public:
	static bool IsPointInTriangle(const POINTF& a, const POINTF& b, const POINTF& c, const POINTF& p);
	static bool AreTwoLinesIntersects(const POINTF& a, const POINTF& b, const POINTF& c, const POINTF& d);
	static bool ArePolygonAndLineIntersects(const PolygonF& polygon, const POINTF& a, const POINTF& b);
	static bool ArePolygonAndRectangleIntersects(const PolygonF& polygon, const RECTANGLEF& rectangle);
};

#endif //CVMDGEOMETRY_H