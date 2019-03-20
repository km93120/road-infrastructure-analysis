#pragma once
#include "opencv2/core/core.hpp" 
#include <iostream>

using namespace std;
using namespace cv;


class CarAlertObject
{


public:
	vector <Point3f> dimensions;
	CarAlertObject();
	~CarAlertObject();
};

