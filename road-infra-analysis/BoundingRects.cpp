#include "BoundingRects.h"



BoundingRects::BoundingRects()
{
}


BoundingRects::~BoundingRects()
{
}

void BoundingRects::clearAllContainers()
{
	circularSignRects.clear();
	pedestrianRects.clear();
	carRects.clear();
	triangleSignRects.clear();
	  
	circleBoundingRects.clear();
	rects.clear();
	triangleBoundingRects.clear();
}