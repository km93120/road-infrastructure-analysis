#pragma once
#include "opencv2/core/core.hpp" 

using namespace std;
using namespace cv;

class CrossSignRect
{
public:
	vector <Point2d> RWdimensions;
	Rect  rect;

	CrossSignRect(Rect);
	~CrossSignRect();
};