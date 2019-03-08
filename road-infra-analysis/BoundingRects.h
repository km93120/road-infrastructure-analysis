#pragma once

#include "opencv2/core/core.hpp" 

#include "CarRect.h"
#include "CircularSignRect.h"
#include "PedestrianRect.h"
#include "CrossSignRect.h"
#include "RPSignRect.h"
#include "StopSignRect.h"


using namespace std;
using namespace cv;


class BoundingRects 
{
public:
	//containers for real detected objects
	vector <CircularSignRect > circularSignRects;
	vector <PedestrianRect> pedestrianRects;
	vector <CarRect > carRects;
	vector <CrossSignRect> crossSignRects;
	vector <RPSignRect> rpSignRects;
	vector <StopSignRect> stopSignRects;

	// containers for potential signs(shapes)

	vector<Rect> circleBoundingRects;
	vector<Rect> rects;
	vector<Rect> triangleBoundingRects;
	vector<Rect> octogonRects;

	BoundingRects();
	~BoundingRects();
	void clearAllContainers();
};

