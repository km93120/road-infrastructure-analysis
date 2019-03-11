#pragma once

#include "opencv2/core/core.hpp" 

using namespace std;
using namespace cv;

class StopSignRect
{
public:
	vector <Point2d> RWdimensions;
	Rect rect;

	StopSignRect(Rect r);
	~StopSignRect();
};