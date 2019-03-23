#include "CircularSignRect.h"


CircularSignRect::CircularSignRect(Rect r)
{
	rect = r; 

	// 2D image points. 
	RWdimensions.push_back(Point3d(0.0f, 0.0f, 0.0f));
	RWdimensions.push_back(Point3d(0.40f, 0.0f, 0.0f));
	RWdimensions.push_back(Point3d(0.40f, 0.40f, 0.0f));
	RWdimensions.push_back(Point3d(0.40f, 0.0f, 0.0f));
}


CircularSignRect::~CircularSignRect()
{

}
