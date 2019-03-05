#pragma once
#include "opencv2/core/core.hpp" 

using namespace std;
using namespace cv;

class TriangleSignRect
{
public:
public:
	Vec2d RWcoordinates;
	Rect  rect;

	TriangleSignRect(Rect);
	~TriangleSignRect();
};

