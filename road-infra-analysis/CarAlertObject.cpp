#include "CarAlertObject.h"



CarAlertObject::CarAlertObject()
{
	img = imread("C:/Users/khafireen/source/repos/road-infra-analysis/road-infra-analysis/warning.jpg");
	Mat clone;
	cvtColor(img, clone, COLOR_BGR2RGBA);
	for (int i = 0; i < clone.rows; i++) {
		for (int j = 0; j < clone.cols; j++) {
			Vec4b& v = clone.at<Vec4b>(i, j);
			
			v[3] = 0.5;

		}
	}
	//clone.copyTo(img);
	
	dimensions.push_back(Point2d(0, 0));
	dimensions.push_back(Point2d(899, 0));
	dimensions.push_back(Point2d(899, 539));
	dimensions.push_back(Point2d(0, 539));
}



CarAlertObject::~CarAlertObject()
{
}
