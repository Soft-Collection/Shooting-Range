#ifndef CVMDSTRUCTURES_H
#define CVMDSTRUCTURES_H

#include "CFFCommon.h"

typedef struct _RECTANGLEF
{
	float left;
	float top;
	float right;
	float bottom;
} RECTANGLEF;

typedef struct _POINTF
{
	float X;
	float Y;
} POINTF;

typedef struct _ALARM
{
	int zone;
	int object;
} ALARM;

#define MIN_NUMBER_OF_MATRICES 1
#define MAX_NUMBER_OF_MATRICES 2
#define MIN_MATRIX_SQUARE 3
#define MAX_MATRIX_SQUARE 19

typedef struct _FILTERINFO
{
	int filterMatrices[MAX_NUMBER_OF_MATRICES * MAX_MATRIX_SQUARE * MAX_MATRIX_SQUARE];
	int numberOfFilterMatrices;
	int filterMatricesSquare;
	int divisionFactor;
	int bias;
} FILTERINFO;

#endif //CVMDSTRUCTURES_H