#include "CarRect.h"



CarRect::CarRect(Rect r)
{
	rect = r;

	RWdimensions.push_back(Point3d(0.0f, 0.0f,0.0f));
	RWdimensions.push_back(Point3d(1.78f, 0.0f, 0.0f));
	RWdimensions.push_back(Point3d(1.78f, 1.35f, 0.0f));
	RWdimensions.push_back(Point3d(1.35f, 0.0f, 0.0f));
}


CarRect::~CarRect()
{
}
