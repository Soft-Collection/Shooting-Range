#ifndef CVMDFRAMEBASE_H
#define CVMDFRAMEBASE_H

#include "CFFCommon.h"

#define VMD_RGB_COLOR_TYPE  0
#define VMD_YUY2_COLOR_TYPE 1

class CVMDFrameBase
{
protected:
	int mWidth;
	int mHeight;
public:
	CVMDFrameBase() : mWidth(0), mHeight(0) { };
	virtual ~CVMDFrameBase() { };
	virtual int GetWidth() {return mWidth;} //Inline Function.
	virtual int GetHeight() {return mHeight;} //Inline Function.
};

#endif //CVMDFRAMEBASE_H