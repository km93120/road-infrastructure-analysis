#pragma once
#include "opencv2/core/core.hpp" 

using namespace std;
using namespace cv;

class CrossSignRect
{
public:
public:
	Vec2d RWcoordinates;
	Rect  rect;

	CrossSignRect(Rect);
	~CrossSignRect();
};

