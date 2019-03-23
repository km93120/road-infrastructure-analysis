#include "CarAlertObject.h"



CarAlertObject::CarAlertObject()
{
	img = imread("C:/Users/khafireen/source/repos/road-infra-analysis/road-infra-analysis/warning.jpg");

	dimensions.push_back(Point2d(0, 0));
	dimensions.push_back(Point2d(img.rows-1, 0.0f));
	dimensions.push_back(Point2d(img.rows-1 , img.cols-1));
	dimensions.push_back(Point2d(0, img.cols-1));
}



CarAlertObject::~CarAlertObject()
{
}
