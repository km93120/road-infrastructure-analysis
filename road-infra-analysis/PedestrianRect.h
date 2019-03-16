#pragma once

#include "opencv2/core/core.hpp" 

using namespace std;
using namespace cv;

class PedestrianRect
{
public:
	vector <Point3d> RWdimensions;
	Rect  rect;

	PedestrianRect(Rect );
	~PedestrianRect();
};

