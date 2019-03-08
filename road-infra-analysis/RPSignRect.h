#pragma once

#include "opencv2/core/core.hpp" 

using namespace std;
using namespace cv;

class RPSignRect
{

public:
	Vec2d RWcoordinates;
	Rect  rect;

	RPSignRect(Rect r);
	~RPSignRect();
};
	

