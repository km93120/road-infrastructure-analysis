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

	circleBoundingRects.clear();
	rects.clear();
	triangleBoundingRects.clear();
}