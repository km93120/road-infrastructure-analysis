#pragma once

#include "opencv2/core/core.hpp" 
#include "BoundingRects.h"
#include "CarAlertObject.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

class dPoints
{
public:

	BoundingRects boundingRects;	
	CarAlertObject carAlertObject;
	Mat rvec, tvec,cameraMatrix,dist_coeffs;
	void setBoundingRects(BoundingRects);
	void computePose(Mat &img);



	dPoints();
	~dPoints();
};
