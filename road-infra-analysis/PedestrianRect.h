#pragma once

#include "opencv2/core/core.hpp" 

using namespace std;
using namespace cv;

class PedestrianRect
{
public:
	Vec2d RWcoordinates;
	Rect  rect;

	PedestrianRect(Rect );
	~PedestrianRect();
};

