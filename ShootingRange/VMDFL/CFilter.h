#ifndef __CFILTER_H__
#define __CFILTER_H__

//http://stackoverflow.com/questions/10715170/receiving-rtsp-stream-using-ffmpeg-library

#include "CFFCommon.h"

class CFilter
{
public:
	CFilter();
	~CFilter();
	static BOOL Sobel(CFFCommon::FilterParams filterParams);
};

#endif //__CFILTER_H__