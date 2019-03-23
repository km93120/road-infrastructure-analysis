#pragma once

#include "opencv2/core/core.hpp" 
#include <vector>

using namespace std;
using namespace cv;

class CircularSignRect
{
public:
	vector <Point3d> RWdimensions;
	Rect  rect;

	CircularSignRect(Rect);
	~CircularSignRect();
};


