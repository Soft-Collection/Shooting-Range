#include "stdafx.h"
#include "CFilter.h"
#include <tchar.h>

CFilter::CFilter()
{
}

CFilter::~CFilter()
{
}

BOOL CFilter::Sobel(CFFCommon::FilterParams filterParams)
{
	return(TRUE);
	uint8_t arr[10];
	int size = filterParams.ColorConversion.Width * filterParams.ColorConversion.Height;
	for (int i = 0; i < size; i++)
	{
		filterParams.ColorConversion.Data[0][i * 3 + 0] = 75;
	}
	return(TRUE);
}
