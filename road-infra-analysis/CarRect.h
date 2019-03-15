#pragma once

#include "opencv2/core/core.hpp" 

using namespace std;
using namespace cv;

class CarRect
{
public:
	vector <Point3d> RWdimensions;
	Rect  rect;

	CarRect(Rect );
	~CarRect();
};

