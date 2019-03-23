#include "StopSignRect.h"



StopSignRect::StopSignRect(Rect r)
{
	rect = r;

	RWdimensions.push_back(Point3d(0.0f, 0.0f, 0.0f));
	RWdimensions.push_back(Point3d(0.40f, 0.0f, 0.0f));
	RWdimensions.push_back(Point3d(0.40f, 0.40f, 0.0f));
	RWdimensions.push_back(Point3d(0.40f, 0.0f, 0.0f));
}


StopSignRect::~StopSignRect()
{

}