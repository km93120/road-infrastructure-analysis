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
	crossSignRects.clear();
	rpSignRects.clear();
	stopSignRects.clear();

	circleBoundingRects.clear();
	rects.clear();
	triangleBoundingRects.clear();
	octogonRects.clear();
}