#include "stdafx.h"
#include "CVMDFrame.h"

CVMDFrame::CVMDFrame(int width, int height, int frameState)
{
	mWidth = width;
	mHeight = height;
	mData = new int[mWidth * mHeight];
	mFrameState = frameState;
}

CVMDFrame::CVMDFrame(int typeOfFrame, int width, int height, BYTE* data, int resizeSquare)
{
	if ((width < 1) || (height < 1)) return;
	if (resizeSquare < 1) return;
	mWidth = width / resizeSquare;
	mHeight = height / resizeSquare;
	mData = new int[mWidth * mHeight];
	switch (typeOfFrame)
	{
	    case VMD_RGB_COLOR_TYPE:
			for(int i = 0; i < mHeight; i++)
			{
				for(int j = 0; j < mWidth; j++)
				{
					int sourceIndex = ((i * resizeSquare) * width + (j * resizeSquare)) * 3;
					int targetIndex = i * mWidth + j;
					int tempRed = data[sourceIndex + 0];
					int tempGreen = data[sourceIndex + 1];
					int tempBlue = data[sourceIndex + 2];
					mData[targetIndex] = (tempRed * 76 + tempGreen * 150 + tempBlue * 28) / 256;
				}
			}
			break;
	    case VMD_YUY2_COLOR_TYPE:
			for(int i = 0; i < mHeight; i++)
			{
				for(int j = 0; j < mWidth; j++)
				{
					int sourceIndex = ((i * resizeSquare) * width + (j * resizeSquare)) * 2;
					int targetIndex = i * mWidth + j;
					mData[targetIndex] = data[sourceIndex];
				}
			}
			break;
	}
	mFrameState = VMD_FRAME_STATE;
}

CVMDFrame::~CVMDFrame()
{
	delete [] mData;
}

CVMDFrame* CVMDFrame::DeepClone() 
{
	CVMDFrame* tempVMDFrame = new CVMDFrame(mWidth, mHeight, mFrameState);
	for(int i = 0; i < mWidth * mHeight; i++) tempVMDFrame->mData[i] = mData[i];  
	return tempVMDFrame;
}

CVMDFrame* CVMDFrame::DifferenceFrame(CVMDFrame* vmdFrame) 
{
	if (mFrameState != VMD_FRAME_STATE) return NULL;
	if (vmdFrame->mFrameState != VMD_FRAME_STATE) return NULL;
	if ((mWidth != vmdFrame->mWidth) || (mHeight != vmdFrame->mHeight)) return NULL;
	CVMDFrame* tempVMDFrame = new CVMDFrame(mWidth, mHeight, VMD_FRAME_STATE);
	for(int i = 0; i < mWidth * mHeight; i++)
	{
		if (mData[i] >= vmdFrame->mData[i]) 
			tempVMDFrame->mData[i] = mData[i] - vmdFrame->mData[i];
		else                                  
			tempVMDFrame->mData[i] = vmdFrame->mData[i] - mData[i];
	}
	return tempVMDFrame;
}

void CVMDFrame::AddFrame(CVMDFrame* vmdFrame)
{
	if ((mFrameState & (VMD_FRAME_STATE | SUM_FRAME_STATE)) == 0) return;
	if (vmdFrame->mFrameState != VMD_FRAME_STATE) return;
	if ((mWidth != vmdFrame->mWidth) || (mHeight != vmdFrame->mHeight)) return;
	for (int i = 0; i < mWidth * mHeight; i++) mData[i] += vmdFrame->mData[i];
	mFrameState = SUM_FRAME_STATE;
}

void CVMDFrame::SubFrame(CVMDFrame* vmdFrame)
{
	if ((mFrameState & (VMD_FRAME_STATE | SUM_FRAME_STATE)) == 0) return;
	if (vmdFrame->mFrameState != VMD_FRAME_STATE) return;
	if ((mWidth != vmdFrame->mWidth) || (mHeight != vmdFrame->mHeight)) return;
	for (int i = 0; i < mWidth * mHeight; i++) mData[i] -= vmdFrame->mData[i];
	mFrameState = SUM_FRAME_STATE;
}

CVMDFrame* CVMDFrame::DivFrame(int framesCount) 
{
	if ((mFrameState & (VMD_FRAME_STATE | SUM_FRAME_STATE)) == 0) return NULL;
	if (framesCount < 1) return NULL;
	CVMDFrame* tempVMDFrame = new CVMDFrame(mWidth, mHeight, VMD_FRAME_STATE);
	for(int i = 0; i < mWidth * mHeight; i++) tempVMDFrame->mData[i] = mData[i] / framesCount;
	return tempVMDFrame;
}

CVMDFrame* CVMDFrame::ResizeFrame(int resizeSquare) 
{
	if (resizeSquare < 1) return NULL;
	CVMDFrame* tempVMDFrame = new CVMDFrame(mWidth / resizeSquare, mHeight / resizeSquare, mFrameState);
	for(int i = 0; i < tempVMDFrame->mHeight; i++)
	{
		for(int j = 0; j < tempVMDFrame->mWidth; j++)
		{
			tempVMDFrame->mData[i * tempVMDFrame->mWidth + j] = mData[(i * resizeSquare) * mWidth + (j * resizeSquare)];
		}
	}
	return tempVMDFrame;
}

CVMDFrame* CVMDFrame::ToleranceFrame(int tolerance) 
{
	if (mFrameState != VMD_FRAME_STATE) return NULL;
	if (tolerance < 0) return NULL;
	CVMDFrame* tempVMDFrame = new CVMDFrame(mWidth, mHeight, BIN_FRAME_STATE);
	for(int i = 0; i < mWidth * mHeight; i++)
	{
		tempVMDFrame->mData[i] = (mData[i] > tolerance) ? 255 : 0;
	}
	return tempVMDFrame;
}

CVMDFrame* CVMDFrame::MedianFrame(int medianSquare) 
{
	if (mFrameState != BIN_FRAME_STATE) return NULL;
	if (medianSquare < 1) return NULL;
	CVMDFrame* tempVMDFrame = new CVMDFrame(mWidth / medianSquare, mHeight / medianSquare, BIN_FRAME_STATE);
	int blacks;
	for(int i = 0; i < tempVMDFrame->mHeight; i++)
	{
		for(int j = 0; j < tempVMDFrame->mWidth; j++)
		{
			blacks = 0;
			for(int k = 0; k < medianSquare; k++)
			{
				for(int l = 0; l < medianSquare; l++)
				{
					if (mData[(i * medianSquare + k) * mWidth + (j * medianSquare + l)] == 0) blacks++;
				}
			}
			tempVMDFrame->mData[i * tempVMDFrame->mWidth + j] = (blacks > medianSquare * medianSquare / 2) ? 0 : 255;
		}
	}
	return tempVMDFrame;
}

CVMDFrame* CVMDFrame::PixelateFrame(int pixelateSquare) 
{
	if (mFrameState != BIN_FRAME_STATE) return NULL;
	if (pixelateSquare < 1) return NULL;
    CVMDFrame* tempVMDFrame = new CVMDFrame(mWidth / pixelateSquare, mHeight / pixelateSquare, VMD_FRAME_STATE);
	int pixelateSquareSum;
    for(int i = 0; i < tempVMDFrame->mHeight; i++)
	{
		for(int j = 0; j < tempVMDFrame->mWidth; j++)
		{
			pixelateSquareSum = 0;
            for(int k = 0; k < pixelateSquare; k++)
			{
				for(int l = 0; l < pixelateSquare; l++)
				{
					pixelateSquareSum += mData[(i * pixelateSquare + k) * mWidth + (j * pixelateSquare + l)];
                }
            }
            tempVMDFrame->mData[i * tempVMDFrame->mWidth + j] = pixelateSquareSum / (pixelateSquare * pixelateSquare);
        }
	}
    return tempVMDFrame;
}

CVMDFrame* CVMDFrame::BorderFrame() 
{
	if (mFrameState != BIN_FRAME_STATE) return NULL;
	CVMDFrame* tempVMDFrame = new CVMDFrame(mWidth, mHeight, BIN_FRAME_STATE);
	for(int i = 0; i < tempVMDFrame->mWidth * tempVMDFrame->mHeight; i++) tempVMDFrame->mData[i] = 0;
	int point0;
	int point1;
	int point2;
	int point3;
	for(int i = 0; i < tempVMDFrame->mHeight - 1; i++)
	{
		for(int j = 0; j < tempVMDFrame->mWidth - 1; j++)
		{
			point0 = mData[(i + 0) * mWidth + (j + 0)];
			point1 = mData[(i + 0) * mWidth + (j + 1)];
			point2 = mData[(i + 1) * mWidth + (j + 0)];
			point3 = mData[(i + 1) * mWidth + (j + 1)];
			if ((point0 == point1) && (point1 == point2) && (point2 == point3))
			{
				tempVMDFrame->mData[(i + 0) * tempVMDFrame->mWidth + (j + 0)] |= 0;
				tempVMDFrame->mData[(i + 0) * tempVMDFrame->mWidth + (j + 1)] |= 0;
				tempVMDFrame->mData[(i + 1) * tempVMDFrame->mWidth + (j + 0)] |= 0;
				tempVMDFrame->mData[(i + 1) * tempVMDFrame->mWidth + (j + 1)] |= 0;
			}
			else
			{
				tempVMDFrame->mData[(i + 0) * tempVMDFrame->mWidth + (j + 0)] = point0;
				tempVMDFrame->mData[(i + 0) * tempVMDFrame->mWidth + (j + 1)] = point1;
				tempVMDFrame->mData[(i + 1) * tempVMDFrame->mWidth + (j + 0)] = point2;
				tempVMDFrame->mData[(i + 1) * tempVMDFrame->mWidth + (j + 1)] = point3;
			}
		}
	}
	return tempVMDFrame;
}

//--------------------------------------------------------------------
//Filters: http://www.student.kuleuven.be/~m0216922/CG/filtering.html
const FILTERINFO CVMDFrame::FILTER_BLUR_01 = 
{
	{0, 1, 0, 
	 1, 1, 1, 
	 0, 1, 0},
	1, 
	3, 
	5, 
	0
};

const FILTERINFO CVMDFrame::FILTER_BLUR_02 = 
{
	{0, 0, 1, 0, 0, 
	 0, 1, 1, 1, 0, 
	 1, 1, 1, 1, 1, 
	 0, 1, 1, 1, 0, 
	 0, 0, 1, 0, 0},
	1, 
	5, 
	13, 
	0
};

const FILTERINFO CVMDFrame::FILTER_BLUR_03 = 
{
	{1, 1, 1, 1, 1, 
	 1, 1, 1, 1, 1, 
	 1, 1, 1, 1, 1, 
	 1, 1, 1, 1, 1, 
	 1, 1, 1, 1, 1},
	1, 
	5, 
	25, 
	0
};

const FILTERINFO CVMDFrame::FILTER_MOTION_BLUR = 
{
    {1, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 1, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 1, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 1, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 1, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 1, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 1, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 1, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 1},
    1,
	9,
	9,
	0
};

const FILTERINFO CVMDFrame::FILTER_EDGES_01 = 
{
	{0,  0,  0,  0,  0,
     0,  0,  0,  0,  0,
    -1, -1,  2,  0,  0,
     0,  0,  0,  0,  0,
     0,  0,  0,  0,  0},
	1, 
	5, 
	1, 
	0
};

const FILTERINFO CVMDFrame::FILTER_EDGES_02 = 
{
	{0,  0, -1,  0,  0,
     0,  0, -1,  0,  0,
     0,  0,  4,  0,  0,
     0,  0, -1,  0,  0,
     0,  0, -1,  0,  0},
	1, 
	5, 
	1, 
	0
};

const FILTERINFO CVMDFrame::FILTER_EDGES_03 = 
{
	{-1,  0,  0,  0,  0,
      0, -2,  0,  0,  0,
      0,  0,  6,  0,  0,
      0,  0,  0, -2,  0,
      0,  0,  0,  0, -1},
	1, 
	5, 
	1, 
	0
};

const FILTERINFO CVMDFrame::FILTER_EDGES_04 = 
{
	{-1, -1, -1,
     -1,  8, -1,
     -1, -1, -1},
	1, 
	3, 
	1, 
	0
};

const FILTERINFO CVMDFrame::FILTER_EDGES_PREWITT = 
{
	{-1,  0,  1, 
	 -1,  0,  1,
	 -1,  0,  1,
	 -1, -1, -1, 
	  0,  0,  0, 
	  1,  1,  1},
	2, 
	3, 
	1, 
	0
};

const FILTERINFO CVMDFrame::FILTER_EDGES_SOBEL = 
{
	{-1,  0,  1, 
	 -2,  0,  2,
	 -1,  0,  1,
	 -1, -2, -1, 
	  0,  0,  0, 
	  1,  2,  1},
	2, 
	3, 
	1, 
	0
};

const FILTERINFO CVMDFrame::FILTER_EDGES_LAPLACIAN = 
{
	{-1, -1, -1, -1, -1, 
	 -1, -1, -1, -1, -1, 
	 -1, -1, 24, -1, -1, 
	 -1, -1, -1, -1, -1, 
	 -1, -1, -1, -1, -1},
	1, 
	5, 
	1, 
	0
};

const FILTERINFO CVMDFrame::FILTER_SHAPEN_01 = 
{
	{-1, -1, -1,
     -1,  9, -1,
     -1, -1, -1},
	1, 
	3, 
	1, 
	0
};

const FILTERINFO CVMDFrame::FILTER_SHAPEN_02 = 
{
	{-1, -1, -1, -1, -1,
     -1,  2,  2,  2, -1,
     -1,  2,  8,  2, -1,
     -1,  2,  2,  2, -1,
     -1, -1, -1, -1, -1},
	1, 
	5, 
	8, 
	0
};

const FILTERINFO CVMDFrame::FILTER_SHAPEN_03 = 
{
	{1,  1,  1,
     1, -7,  1,
     1,  1,  1},
	1, 
	3, 
	1, 
	0
};

const FILTERINFO CVMDFrame::FILTER_EMBOSS_01 = 
{
	{-1, -1,  0,
     -1,  0,  1,
      0,  1,  1},
	1, 
	3, 
	1, 
	128
};

const FILTERINFO CVMDFrame::FILTER_EMBOSS_02 = 
{
	{-1, -1, -1, -1,  0,
     -1, -1, -1,  0,  1,
     -1, -1,  0,  1,  1,
     -1,  0,  1,  1,  1,
      0,  1,  1,  1,  1},
	1, 
	5, 
	1, 
	128
};

const FILTERINFO CVMDFrame::FILTER_MEAN = 
{
	{1,  1,  1,
     1,  1,  1,
     1,  1,  1},
	1, 
	3, 
	9, 
	0
};

/*
THE BASIC FORMULA (Paint Shop Pro 7.02 User Defined Filter Help)
A filter alters each pixel's color based on its current color and the colors of any neighboring pixels. 
The heart of a filter is an array of coefficients called a filter matrix. 
A filter processes an image on a pixel-by-pixel basis. Each pixel's color value is multiplied by the coefficient in the matrix center, 
and any pixels within the matrix are multiplied by the corresponding coefficients. 
The sum of the products becomes the target pixel's new value. 
The new value is saved in a separate bitmap so that it does not affect the remaining pixels. 
The formula for this calculation is:

F = (i = 1 to 49) SIGMA Pi * Ci

Where F is the filtered value of the target pixel, P is a pixel in the grid, and C is a coefficient in the matrix.

THE DIVISION FACTOR
You can think of the division factor as the coefficient denominator: 
each coefficient in the filter matrix is divided by the division factor before being applied to a pixel. 
In actuality, the division factor is applied to the product of the matrix calculation. 
The division factor changes the filter formula to: 

F = ( (i = 1 to 49) SIGMA Pi * Ci ) / D

The division factor allows you to achieve effects that would otherwise require decimal coefficients. 
The advantage of requiring non-decimal coefficients is execution speed: 
Paint Shop Pro can accelerate the filtering process by using integers to perform the math. 

THE BIAS
The bias is added to the product of the matrix calculation and the division factor. 
You use the bias to shift the value of each pixel by a fixed amount. 
Bias adjustments are particularly useful for creating embossing effects. 
The bias changes the filter formula to: 

F = ( ( (i = 1 to 49) SIGMA Pi * Ci ) / D ) + B

Where F is the filtered value of the target pixel, P is a pixel in the grid, C is a coefficient in the matrix, D is the division factor, and B is the bias. 
*/
CVMDFrame* CVMDFrame::FilteredFrame(FILTERINFO* fi) 
{
	if (mFrameState != VMD_FRAME_STATE) return NULL;
	if ((fi->numberOfFilterMatrices < MIN_NUMBER_OF_MATRICES) || (fi->numberOfFilterMatrices > MAX_NUMBER_OF_MATRICES)) return NULL;
	if ((fi->filterMatricesSquare < MIN_MATRIX_SQUARE) || (fi->filterMatricesSquare > MAX_MATRIX_SQUARE) || (fi->filterMatricesSquare % 2 != 1)) return NULL;
	if (fi->divisionFactor < 1) return NULL;
	int filterMatricesSquareHalf = fi->filterMatricesSquare / 2;
	int totalSum = 0; //Sum of absolute values of F's (below) per Filter Matrix.
	int sum = 0; //F = (i = 1 to 49) SIGMA Pi * Ci
	int temp = 0; //totalSum / divisionFactor + bias
	int currentY = 0;
	int currentX = 0;
	CVMDFrame* tempVMDFrame = new CVMDFrame(mWidth, mHeight, mFrameState);
	for (int y = 0; y < mHeight; y++)
	{
		for (int x = 0; x < mWidth; x++)
		{
			totalSum = 0;
			for (int k = 0; k < fi->numberOfFilterMatrices; k++)
			{
				sum = 0;
				for (int j = 0; j < fi->filterMatricesSquare; j++)
				{
					for (int i = 0; i < fi->filterMatricesSquare; i++)
					{
						currentY = y + (j - filterMatricesSquareHalf);
						currentX = x + (i - filterMatricesSquareHalf);
						if ((currentY >= 0) && (currentY < mHeight) && (currentX >= 0) && (currentX < mWidth)){
							sum += mData[currentY * mWidth + currentX] * fi->filterMatrices[(k * fi->filterMatricesSquare * fi->filterMatricesSquare) + (j * fi->filterMatricesSquare + i)];
						}
					}
				}
				(sum > 0) ? totalSum += sum : totalSum -= sum;
			}
			temp = totalSum / fi->divisionFactor + fi->bias;
			if (temp < 0) temp = 0;
			if (temp > 255) temp = 255;
			tempVMDFrame->mData[y * mWidth + x] = temp;
		}
	}
	return tempVMDFrame;
}
//End Filters.
//--------------------------------------------------------------------
//Enumerate Moving Objects.
void CVMDFrame::GetPreviousAdjacentPixelsCoordinates(int x, int y, int index, int* paX, int* paY)
{
	switch(index)
	{
		case 0:
			*paX = x - 1;
			*paY = y - 1;
			break;
		case 1:
			*paX = x;
			*paY = y - 1;
			break;
		case 2:
			*paX = x + 1;
			*paY = y - 1;
			break;
		case 3:
			*paX = x - 1;
			*paY = y;
			break;
	}
}

void CVMDFrame::GetPreviousAdjacentPixels(int x, int y, bool* previousAdjacentPixels)
{
	previousAdjacentPixels[0] = ((y > 0) && (x > 0));
	previousAdjacentPixels[1] =  (y > 0);
	previousAdjacentPixels[2] = ((y > 0) && (x < mWidth - 1));
	previousAdjacentPixels[3] =  (x > 0);
}

void CVMDFrame::UpdateObjectContainingSquareAndCenterPoint(int x, int y, int width, int height, RECTANGLEF **objectContainingSquares, RECTANGLEF *objectContainingSquaresValues, POINTF* averagePointSum, int* averagePointCount, int index)
{
	float tempX = (float)x / (float)width;
	float tempY = (float)y / (float)height;
	if (objectContainingSquares[index] == NULL)
	{
		objectContainingSquares[index] = &objectContainingSquaresValues[index];
		objectContainingSquares[index]->left   = tempX;
		objectContainingSquares[index]->top    = tempY;
		objectContainingSquares[index]->right  = tempX;
		objectContainingSquares[index]->bottom = tempY;
		//----------------------------------------------------
		averagePointSum[index].X = tempX;
		averagePointSum[index].Y = tempY;
		//----------------------------------------------------
		averagePointCount[index] = 1;
	}
	else
	{
		if (objectContainingSquares[index]->left   > tempX) objectContainingSquares[index]->left   = tempX;
		if (objectContainingSquares[index]->top    > tempY) objectContainingSquares[index]->top    = tempY;
		if (objectContainingSquares[index]->right  < tempX) objectContainingSquares[index]->right  = tempX;
		if (objectContainingSquares[index]->bottom < tempY) objectContainingSquares[index]->bottom = tempY;
		//--------------------------------------------------------------------------------------------------
		averagePointSum[index].X += tempX;
		averagePointSum[index].Y += tempY;
		//--------------------------------------------------------------------------------------------------
		averagePointCount[index]++;
	}
}

void CVMDFrame::EnumerateMovingObjects(int minimalDistanceBetweenTwoObjects, float minWidth, float minHeight, float maxWidth, float maxHeight, int maximalAmountOfObjectsToFind, RECTANGLEF *instantMotion, POINTF* instantMotionAveragePoint, int *instantMotionCount)
{
	if (mFrameState != BIN_FRAME_STATE) return;
	if (minimalDistanceBetweenTwoObjects < 1) return;
	if (maximalAmountOfObjectsToFind < 1) return;
	//First Part Of Algorithm.
	//1. Red square is moving from left to right line by line.
	//2. If red square encounters not empty pixel:
	//    2.1 It scans all possible previous adjascent pixels (blue squares) in order: 1, 2, 3, 4:
	//        2.1.1 If blue square encounters first not empty pixel (that must have been already assigned by pointer to group number): Make pointer of Red Square point to the same address as pointer of Blue Square.
	//        2.1.2 If blue square encounters not first not empty pixel (that must have been already assigned by pointer to group number): Set group number, that Blue Square's pointer points to as number that Red Square's pointer points to.
	//        2.1.3 If blue square doesn't encounter not empty pixel: Assign to Red Square a new pointer that points to next group number (just increment group number counter).
	int count = 0;
	int** groupIDs = new int* [mWidth * mHeight];    //MUST BE FREED!!!
	int* groupIDsValues = new int[mWidth * mHeight]; //MUST BE FREED!!!
	int currentPixelIndex;
	int currentPreviousAdjacentPixelIndex;
	bool previousAdjacentPixels[4];
	int paX;
	int paY;
	for (int y = 0; y < mHeight; y++)
	{
		for (int x = 0; x < mWidth; x++)
		{
			currentPixelIndex = y * mWidth + x;
			groupIDs[currentPixelIndex] = NULL;
			groupIDsValues[currentPixelIndex] = 0;
			if (mData[currentPixelIndex] != 0)
			{
				GetPreviousAdjacentPixels(x, y, previousAdjacentPixels);
				for (int i = 0; i < 4; i++)
				{
					if (previousAdjacentPixels[i])
					{
						GetPreviousAdjacentPixelsCoordinates(x, y, i, &paX, &paY);
						currentPreviousAdjacentPixelIndex = paY * mWidth + paX;
						if (mData[currentPreviousAdjacentPixelIndex] != 0)
						{
							if (groupIDs[currentPixelIndex] == NULL)
							{
								groupIDs[currentPixelIndex] = groupIDs[currentPreviousAdjacentPixelIndex];
							}
							else
							{
								*groupIDs[currentPreviousAdjacentPixelIndex] = *groupIDs[currentPixelIndex];
							}
						}
					}
				}
				if (groupIDs[currentPixelIndex] == NULL)
				{
					groupIDs[currentPixelIndex] = &groupIDsValues[currentPixelIndex];
					*groupIDs[currentPixelIndex] = count++;
				}
			}
		}
	}

	//Second Part Of Algorithm.
	//1. Scan squares of size (minimalDistanceBetweenTwoObjects * 2) from left to right towards down. First square left top coordinates are (0, 0).
	//    1.1 Inside each square above scan small squares on even rows and columns and verify that they are not exceeds the image bounds.
	//        1.1.1 Get the group number of the first encountered isle and assign it to other isles.
	//2. Scan squares of size (minimalDistanceBetweenTwoObjects * 2) from left to right towards down. First square left top coordinates are (minimalDistanceBetweenTwoObjects, minimalDistanceBetweenTwoObjects).
	//    2.1 Inside each square above scan small squares on even rows and columns and verify that they are not exceeds the image bounds.
	//        2.1.1 Get the group number of the first encountered isle and assign it to other isles.
	if (minimalDistanceBetweenTwoObjects > 1)
	{
		minimalDistanceBetweenTwoObjects--;
		int tempGroupIDValue;
		for (int n = 0; n < 2; n++)
		{
			for (int j = (minimalDistanceBetweenTwoObjects * n); j < mHeight; j += (minimalDistanceBetweenTwoObjects * 2))
			{
				for (int i = (minimalDistanceBetweenTwoObjects * n); i < mWidth; i += (minimalDistanceBetweenTwoObjects * 2))
				{
					tempGroupIDValue = -1;
					for (int l = 0; (l < (minimalDistanceBetweenTwoObjects * 2)) && (j + l < mHeight); l += 2)
					{
						for (int k = 0; (k < (minimalDistanceBetweenTwoObjects * 2)) && (i + k < mWidth); k += 2)
						{
							if (groupIDs[(j + l) * mWidth + (i + k)] != NULL)
							{
								if (tempGroupIDValue >= 0)
								{
									*groupIDs[(j + l) * mWidth + (i + k)] = tempGroupIDValue;
								}
								else
								{
									tempGroupIDValue = *groupIDs[(j + l) * mWidth + (i + k)];
								}
							}
						}
					}
				}
			}
		}
	}

	//Third Part Of Algorithm.
	//1. Create an array of pointers to RECTANGLEF that it's size is the largest group number.
	//2. Initialize all the pointers in array to NULL.
	//3. Scan all the groupIDs array.
	//    3.1 If groupIDs[i] is not NULL then:
	//        3.1.1 Get group number (*groupIDs[i]).
	//        3.1.2 Look at the member of an array in (paragraph 1) with index of group number (paragraph 3.1.1)
	//            3.1.2.1 If it is NULL then create one (RECTANGLEF) with left, top, right, bottom of current point's x, y coordinates.
	//            3.1.2.2 If it is not NULL then extend the current rectangle that it will include current point's x, y coordinates.
	//4. Scan the array of objectContainingSquares until it's end or until the desired number of objects of filtered size is reached.
	RECTANGLEF** objectContainingSquares = new RECTANGLEF* [count];     //MUST BE FREED!!!
	RECTANGLEF*  objectContainingSquaresValues = new RECTANGLEF[count]; //MUST BE FREED!!!
	POINTF*      averagePointSum = new POINTF[count];                   //MUST BE FREED!!!
	int*         averagePointCount = new int[count];                    //MUST BE FREED!!!
	for (int i = 0; i < count; i++)
	{
		objectContainingSquares[i] = NULL;
		//-------------------------------------------
		objectContainingSquaresValues[i].left = 0;
		objectContainingSquaresValues[i].top = 0;
		objectContainingSquaresValues[i].right = 0;
		objectContainingSquaresValues[i].bottom = 0;
		//-------------------------------------------
		averagePointSum[i].X = 0;
		averagePointSum[i].Y = 0;
		//-------------------------------------------
		averagePointCount[i] = 0;
	}
	for (int y = 0; y < mHeight; y++)
	{
		for (int x = 0; x < mWidth; x++)
		{
			if (groupIDs[y * mWidth + x] != NULL)
			{
				UpdateObjectContainingSquareAndCenterPoint(x, y, mWidth, mHeight, objectContainingSquares, objectContainingSquaresValues, averagePointSum, averagePointCount, *groupIDs[y * mWidth + x]);
			}
		}
	}
	int j = 0;
	for (int i = 0; (i < count) && (j < maximalAmountOfObjectsToFind); i++)
	{
		if ((objectContainingSquares[i] != NULL) &&
			(objectContainingSquares[i]->right  - objectContainingSquares[i]->left > minWidth ) &&
			(objectContainingSquares[i]->right  - objectContainingSquares[i]->left < maxWidth ) &&
			(objectContainingSquares[i]->bottom - objectContainingSquares[i]->top  > minHeight) &&
			(objectContainingSquares[i]->bottom - objectContainingSquares[i]->top  < maxHeight))
		{
			instantMotion[j] = *objectContainingSquares[i];
			instantMotionAveragePoint[j].X = averagePointSum[i].X / averagePointCount[i];
			instantMotionAveragePoint[j].Y = averagePointSum[i].Y / averagePointCount[i];
			j++;
		}
	}
	*instantMotionCount = j;
	//Memory Free Part Of Algorithm.
	delete[] groupIDs;
	delete[] groupIDsValues;
	delete[] objectContainingSquares;
	delete[] objectContainingSquaresValues;
	delete[] averagePointSum;
	delete[] averagePointCount;
}
//End Enumerate Moving Objects.
//--------------------------------------------------------------------
void CVMDFrame::GetDimensions(int* width, int* height)
{
	//For GDI Drawing, Width and Height must be Dividable by 4. 
	*width = mWidth & 0xFFFFFFFC;
	*height = mHeight & 0xFFFFFFFC;
}
void CVMDFrame::GetData(BYTE* data) 
{ 
	//For GDI Drawing, Width and Height must be Dividable by 4. 
	int lVMDFrameWidthFourDividable = mWidth & 0xFFFFFFFC; //Needed for GDI Drawing.
	int lVMDFrameHeightFourDividable = mHeight & 0xFFFFFFFC; //Needed for GDI Drawing.
	int tempRedIndex = 0;
	int tempGreenIndex = 0;
	int tempBlueIndex = 0;
	BYTE tempVMDValue = 0;
	for(int i = 0; i < lVMDFrameHeightFourDividable; i++)
	{
		for(int j = 0; j < lVMDFrameWidthFourDividable; j++)
		{
			tempRedIndex = (i * lVMDFrameWidthFourDividable + j) * 3;
			tempGreenIndex = tempRedIndex + 1;
			tempBlueIndex = tempRedIndex + 2;
			tempVMDValue = (BYTE)(mData[i * mWidth + j]);
			data[tempRedIndex] = tempVMDValue;
			data[tempGreenIndex] = tempVMDValue;
			data[tempBlueIndex] = tempVMDValue;
		}
	}
}