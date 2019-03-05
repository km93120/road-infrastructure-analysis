#pragma once

#include "opencv2/core/core.hpp" 

#include "CarRect.h"
#include "CircularSignRect.h"
#include "PedestrianRect.h"
#include "TriangleSignRect.h"

using namespace std;
using namespace cv;


class BoundingRects
{
public:
	//containers for real detected objects
	vector <CircularSignRect > circularSignRects;
	vector <PedestrianRect> pedestrianRects;
	vector <CarRect > carRects;
	vector <TriangleSignRect> triangleSignRects;

	// containers for potential signs(shapes)

	vector<Rect> circleBoundingRects;
	vector<Rect> rects;
	vector<Rect> triangleBoundingRects;

	BoundingRects();
	~BoundingRects();
	void clearAllContainers();
};

