#pragma once
#include "opencv2/core/core.hpp" 
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>


using namespace std;
using namespace cv;


class CarAlertObject
{


public:
	vector <Point2d> dimensions;
	Mat img;
	CarAlertObject();
	~CarAlertObject();
};

