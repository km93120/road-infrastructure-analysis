#include "3dPoints.h"
#include "opencv2/calib3d/calib3d.hpp"
#include <iostream>


dPoints::dPoints()
{

	cameraMatrix = (Mat_<double>(3, 3) << 828.6221, 0.0000, 326.4434,
		0.0000, 826.9493, 245.2840,
		0.0000, 0.0000, 1.0000);
   
	dist_coeffs = Mat::zeros(4, 1, DataType<double>::type);

	carAlertObject = CarAlertObject();

}

void dPoints::setBoundingRects(BoundingRects  boundingRects)
{
	this->boundingRects = boundingRects;
}

void dPoints::computePose(Mat &img)
{
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
		
		Mat H;

		//findHomography(carAlertObject.dimensions, imagePoints, H);

		cout << "H rows : " << H.rows << endl 
		     <<	"H cols : " << H.cols << endl;
		 
		//warpPerspective(carAlertObject.img, img, H,img.size());

		// <recalage virtuel>-------------------------------------------------------------------------
		/*Mat H;
		vector <Point3f> inputPoints = carAlertObject.dimensions;
		vector <Point2d> carAlertObjectImagePoints;
		Mat persTrainImage;
		Mat imv;
		cv::projectPoints(inputPoints, rvec, tvec, cameraMatrix, dist_coeffs, carAlertObjectImagePoints);
		findHomography(carAlertObjectImagePoints, imagePoints, H);

		
		warpPerspective(imv, img, H, img.size());
		//warpPerspective(trainImages, persTrainImage, H, testImage.size());


		vector<Point> vecSceneCorners;
		for (int i = 0; i < 4; i++) vecSceneCorners.push_back(sc_corn[i]);

		UMat mask_image(imv.size(), CV_8U, Scalar(0));
		vector<vector<Point>> polyRecObject;
		polyRecObject.push_back(vecSceneCorners);
		fillPoly(mask_image, polyRecObject, Scalar(255));
		imshow("mask", mask_image);

		persTrainImage.copyTo(imv, mask_image);
		*/
		//</recalage virtuel> -----------------------------------------------------------------------
		
		std::cout << tvec.at<double>(0,2)<< endl;
		double z = tvec.at<double>(0, 2);
		std::ostringstream strs;
		strs << z;
		std::string str = strs.str();
		
		if (z < 25)
		{
			cv::line(img, Point(img.cols / 2, img.rows), Point(r.x + r.width / 2, r.y + r.height / 2), Scalar(0, 0, 255),2);
			putText(img, str, Point(r.x + r.width + 10, r.y + r.height / 2), 1, 1, Scalar(0, 0, 255));
			rectangle(img, Point(cvRound(r.x), cvRound(r.y)),
				Point(cvRound((r.x + r.width - 1)), cvRound((r.y + r.height - 1))),
				Scalar(0, 0, 255), 3, 8, 0);
		}

		else
		{
			cv::line(img, Point(img.cols / 2, img.rows), Point(r.x + r.width / 2, r.y + r.height / 2), Scalar(255, 0, 0));
			putText(img, str, Point(r.x + r.width + 10, r.y + r.height / 2), 1, 1, Scalar(255, 0, 0));
		}


	}

	for (int i = 0; i < boundingRects.pedestrianRects.size(); i++)
	{
		vector<Point3d> RWpoints = boundingRects.pedestrianRects.at(i).RWdimensions;
		Rect r = boundingRects.pedestrianRects.at(i).rect;
		vector<Point2d> imagePoints;
		imagePoints.push_back(Point2d(r.x, r.y));
		imagePoints.push_back(Point2d(r.x + r.width, r.y));
		imagePoints.push_back(Point2d(r.x + r.width, r.y + r.height));
		imagePoints.push_back(Point2d(r.x, r.y + r.height));

		solvePnP(RWpoints, imagePoints, cameraMatrix, dist_coeffs, rvec, tvec, false, SOLVEPNP_ITERATIVE);
		double z = tvec.at<double>(0, 2);
		std::ostringstream strs;
		strs << z;
		std::string str = strs.str();
		if (z < 20)
		{
			cv::line(img, Point(img.cols / 2, img.rows), Point(r.x + r.width / 2, r.y + r.height / 2), Scalar(0, 0, 255), 2);
			putText(img, str, Point(r.x + r.width + 10, r.y + r.height / 2), 1, 1, Scalar(0, 0, 255));
		}

		else
		{
			cv::line(img, Point(img.cols / 2, img.rows), Point(r.x + r.width / 2, r.y + r.height / 2), Scalar(255, 0, 0));
			putText(img, str, Point(r.x + r.width + 10, r.y + r.height / 2), 1, 1, Scalar(255, 0, 0));
		}

	}

	for (int i = 0; i < boundingRects.stopSignRects.size(); i++)
	{
		vector<Point3d> RWpoints = boundingRects.stopSignRects.at(i).RWdimensions;
		Rect r = boundingRects.stopSignRects.at(i).rect;
		vector<Point2d> imagePoints;
		imagePoints.push_back(Point2d(r.x, r.y));
		imagePoints.push_back(Point2d(r.x + r.width, r.y));
		imagePoints.push_back(Point2d(r.x + r.width, r.y + r.height));
		imagePoints.push_back(Point2d(r.x, r.y + r.height));

		solvePnP(RWpoints, imagePoints, cameraMatrix, dist_coeffs, rvec, tvec, false, SOLVEPNP_ITERATIVE);
		double z = tvec.at<double>(0, 2);
		if (z < 30)
		{
			cv::line(img, Point(img.cols / 2, img.rows), Point(r.x + r.width / 2, r.y + r.height / 2), Scalar(0, 0, 255), 2);
		}

		else
		{
			cv::line(img, Point(img.cols / 2, img.rows), Point(r.x + r.width / 2, r.y + r.height / 2), Scalar(255, 0, 0));
		}
		std::ostringstream strs;
		strs << z;
		std::string str = strs.str();
		putText(img, str, Point(r.x + r.width + 10, r.y + r.height / 2), 1, 1, Scalar(0, 0, 255));

	}
	for (int i = 0; i < boundingRects.circularSignRects.size(); i++)
	{
		vector<Point3d> RWpoints = boundingRects.circularSignRects.at(i).RWdimensions;
		Rect r = boundingRects.circularSignRects.at(i).rect;
		vector<Point2d> imagePoints;
		imagePoints.push_back(Point2d(r.x, r.y));
		imagePoints.push_back(Point2d(r.x + r.width, r.y));
		imagePoints.push_back(Point2d(r.x + r.width, r.y + r.height));
		imagePoints.push_back(Point2d(r.x, r.y + r.height));

		solvePnP(RWpoints, imagePoints, cameraMatrix, dist_coeffs, rvec, tvec, false, SOLVEPNP_ITERATIVE);

		// <recalage virtuel>
		/*vector <Point3f> inputPoints = carAlertObject.dimensions;
		vector <Point2d> carAlertObjectImagePoints;

		cv::projectPoints(inputPoints, rvec, tvec, cameraMatrix, dist_coeffs, carAlertObjectImagePoints);

		Mat clone = img.clone();
		Mat M;
		findHomography(carAlertObjectImagePoints, imagePoints, M);
		warpPerspective(clone, img, M, img.size());*/

		//</recalage virtuel> 
		std::cout << tvec.at<double>(0, 2) << endl;
		double z = tvec.at<double>(0, 2);
		std::ostringstream strs;
		strs << z;
		std::string str = strs.str();

		if (z < 25)
		{
			cv::line(img, Point(img.cols / 2, img.rows), Point(r.x + r.width / 2, r.y + r.height / 2), Scalar(0, 0, 255), 2);
			putText(img, str, Point(r.x + r.width + 10, r.y + r.height / 2), 1, 1, Scalar(0, 0, 255));
		}

		else
		{
			cv::line(img, Point(img.cols / 2, img.rows), Point(r.x + r.width / 2, r.y + r.height / 2), Scalar(255, 0, 0));
			putText(img, str, Point(r.x + r.width + 10, r.y + r.height / 2), 1, 1, Scalar(255, 0, 0));
		}


	}

	imshow("pose", img);
}




/*
void recalage(vector<Point2f> sc_corn, Mat trainImages) {
	UMat persTrainImage;
	UMat imv;
	vid >> imv;

	resize(imv, imv, trainImages.size());
	if (vid.get(CAP_PROP_POS_FRAMES) == countVidFrame) vid.set(CAP_PROP_POS_FRAMES, 0);
	warpPerspective(imv, persTrainImage, H, testImage.size());
	//warpPerspective(trainImages, persTrainImage, H, testImage.size());


	vector<Point> vecSceneCorners;
	for (int i = 0; i < 4; i++) vecSceneCorners.push_back(sc_corn[i]);

	UMat mask_image(testImage.size(), CV_8U, Scalar(0));
	vector<vector<Point>> polyRecObject;
	polyRecObject.push_back(vecSceneCorners);
	fillPoly(mask_image, polyRecObject, Scalar(255));
	imshow("mask", mask_image);

	persTrainImage.copyTo(testImage, mask_image); 
}
*/


dPoints::~dPoints()
{
}


/*vector <Point3f> inputPoints = carAlertObject.dimensions;
		vector <Point2d> carAlertObjectImagePoints;

		cv::projectPoints(inputPoints, rvec, tvec, cameraMatrix, dist_coeffs, carAlertObjectImagePoints);

		Mat clone = img.clone();
		Mat M;
		findHomography(carAlertObjectImagePoints, imagePoints, M);
		warpPerspective(clone, img, M, img.size());*/