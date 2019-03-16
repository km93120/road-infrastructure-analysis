#include "PedestrianRect.h"



PedestrianRect::PedestrianRect(Rect r)
{
	rect = r;

	// 2D image points. 
	RWdimensions.push_back(Point3d(0.0f, 0.0f, 0.0f));
	RWdimensions.push_back(Point3d(0.40f, 0.0f, 0.0f));
	RWdimensions.push_back(Point3d(0.40f, 1.80f, 0.0f));
	RWdimensions.push_back(Point3d(0.0f, 1.80f, 0.0f));
}

PedestrianRect::~PedestrianRect()
{

}
