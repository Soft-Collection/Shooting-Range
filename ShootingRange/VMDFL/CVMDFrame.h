#ifndef CVMDFRAME_H
#define CVMDFRAME_H

#include "CFFCommon.h"
#include "CVMDFrameBase.h"
#include "CVMDStructures.h"

#define VMD_FRAME_STATE 0x01 //0 to 255. (GrayScale Frame).
#define SUM_FRAME_STATE 0x02 //Every integer number. (Sum of many Frames).
#define BIN_FRAME_STATE 0x04 //0 = Black, or 255 = White.

class CVMDFrame : public CVMDFrameBase
{
public:
	//Constructors and Destructors.
	CVMDFrame(int typeOfFrame, int width, int height, BYTE* data, int resizeSquare);
	virtual ~CVMDFrame();
	//Functions.
	CVMDFrame* DeepClone();
	CVMDFrame* DifferenceFrame(CVMDFrame* vmdFrame);
	void AddFrame(CVMDFrame* vmdFrame);
	void SubFrame(CVMDFrame* vmdFrame);
	CVMDFrame* DivFrame(int framesCount);
	CVMDFrame* ResizeFrame(int resizeSquare);
	CVMDFrame* ToleranceFrame(int tolerance);
	CVMDFrame* MedianFrame(int medianSquare);
	CVMDFrame* PixelateFrame(int pixelateSquare) ;
	CVMDFrame* BorderFrame();
	//--------------------------------------------------------------------
	//Filters: http://www.student.kuleuven.be/~m0216922/CG/filtering.html
	static const FILTERINFO FILTER_BLUR_01;
	static const FILTERINFO FILTER_BLUR_02;
	static const FILTERINFO FILTER_BLUR_03;
	static const FILTERINFO FILTER_MOTION_BLUR;
	static const FILTERINFO FILTER_EDGES_01;
	static const FILTERINFO FILTER_EDGES_02;
	static const FILTERINFO FILTER_EDGES_03;
	static const FILTERINFO FILTER_EDGES_04;
	static const FILTERINFO FILTER_EDGES_PREWITT;
	static const FILTERINFO FILTER_EDGES_SOBEL;
	static const FILTERINFO FILTER_EDGES_LAPLACIAN;
	static const FILTERINFO FILTER_SHAPEN_01;
	static const FILTERINFO FILTER_SHAPEN_02;
	static const FILTERINFO FILTER_SHAPEN_03;
	static const FILTERINFO FILTER_EMBOSS_01;
	static const FILTERINFO FILTER_EMBOSS_02;
	static const FILTERINFO FILTER_MEAN;
	CVMDFrame* FilteredFrame(FILTERINFO* fi);
	//End Filters.
	//--------------------------------------------------------------------
	//Enumerate Moving Objects.
	void GetPreviousAdjacentPixelsCoordinates(int x, int y, int index, int* paX, int* paY);
	void GetPreviousAdjacentPixels(int x, int y, bool* previousAdjacentPixels);
	void UpdateObjectContainingSquareAndCenterPoint(int x, int y, int width, int height, RECTANGLEF **objectContainingSquares, RECTANGLEF *objectContainingSquaresValues, POINTF* averagePointSum, int* averagePointCount, int index);
	void EnumerateMovingObjects(int minimalDistanceBetweenTwoObjects, float minWidth, float minHeight, float maxWidth, float maxHeight, int maximalAmountOfObjectsToFind, RECTANGLEF *instantMotion, POINTF* instantMotionAveragePoint, int *instantMotionCount);
	//End Enumerate Moving Objects.
	//--------------------------------------------------------------------
	void GetDimensions(int* width, int* height);
	void GetData(BYTE* data);
protected:
	CVMDFrame(int width, int height, int frameState);
	int* mData;
	int  mFrameState;
};

#endif CVMDFRAME_H
