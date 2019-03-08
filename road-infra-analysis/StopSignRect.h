#pragma once

#include "opencv2/core/core.hpp" 

using namespace std;
using namespace cv;

class StopSignRect
{
public:
	Vec2d RWcoordinates;
	Rect rect;

	StopSignRect(Rect r);
	~StopSignRect();
};

