#include "3dPoints.h"
#include "opencv2/calib3d/calib3d.hpp"

#include <iostream>


void dPoints::setBoundingRects(BoundingRects  boundingRects)
{
	this->boundingRects = boundingRects;
}

void dPoints::computePose(Mat img)
{
	Mat rvec, tvec;
	static  Mat cameraMatrix = (Mat_<double>(3, 3) << 828.6221, 0.0000, 326.4434, 
													  0.0000, 826.9493, 245.2840,
													  0.0000, 0.0000, 1.0000 );
	Mat dist_coeffs = Mat::zeros(4, 1, DataType<double>::type);

	for (int i = 0; i < boundingRects.carRects.size(); i++)
	{
		vector<Point3d> RWpoints = boundingRects.carRects.at(i).RWdimensions;
		Rect r = boundingRects.carRects.at(i).rect;
		vector<Point2d> imagePoints;
		imagePoints.push_back(Point2d(r.x, r.y));
		imagePoints.push_back(Point2d(r.x + r.width, r.y));
		imagePoints.push_back(Point2d(r.x + r.width, r.y+ r.height));
		imagePoints.push_back(Point2d(r.x, r.y + r.height));

		solvePnP(RWpoints, imagePoints, cameraMatrix, dist_coeffs, rvec, tvec, false, SOLVEPNP_ITERATIVE);
		std::cout << tvec.at<double>(0,2)<< endl;
		if (tvec.at<double>(0, 2) < 20)
		{
			cv::line(img, Point(img.cols / 2, img.rows), Point(r.x + r.width / 2, r.y + r.height / 2), Scalar(0, 0, 255),2);
		}

		else
		{
			cv::line(img, Point(img.cols / 2, img.rows), Point(r.x + r.width / 2, r.y + r.height / 2), Scalar(255, 0, 0));
		}

	}
	imshow("pose", img);
}

dPoints::dPoints()
{
	// 3D model points.
	// Pour les triangles


}


dPoints::~dPoints()
{
}
