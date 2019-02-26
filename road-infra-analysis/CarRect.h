#pragma once

#include "opencv2/core/core.hpp" 

using namespace std;
using namespace cv;

class CarRect
{
public:
	Vec2d RWcoordinates;
	Rect  rect;

	CarRect(Rect );
	~CarRect();
};

