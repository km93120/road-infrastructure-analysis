#pragma once

#include "opencv2/core/core.hpp" 

using namespace std;
using namespace cv;

class RPSignRect
{

public:
	vector <Point2d> RWdimensions;
	Rect  rect;

	RPSignRect(Rect r);
	~RPSignRect();
};