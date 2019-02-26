#pragma once

#include "opencv2/core/core.hpp" 

using namespace std;
using namespace cv;

class CircularSignRect
{
public:
	Vec2d RWcoordinates;
	Rect  rect;

	CircularSignRect(Rect);
	~CircularSignRect();
};


