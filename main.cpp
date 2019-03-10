#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/opencv.hpp>

#include <iostream>

#include "road-infra-analysis.h"
#include "3dPoints.h"

using namespace std;
using namespace cv;

int main(int argc, const char** argv)
{


	string carCascadeName, 
		   pedestrianCascadeName, 
		   fDirCascadeName,
		   stopSignCascadeName;

	VideoCapture capture;
	Mat frame, image;
	string inputName;
	bool tryflip;

	CascadeClassifier 
		carCascade, 
		pedestrianCascade, 
		fDirSignCascade,
		stopSignCascade;

	vector<CascadeClassifier> classifiers;

	vector<int> op_codes;
	BoundingRects boundingRects;

	double scale;

	cv::CommandLineParser parser(argc, argv,
		"{help h||}"
		"{carsCascade |cars|}"
		"{pedestriansCascade|haarcascade_pedestrians|}"
		"{stopsignsCascade|f_direction_augmented|}"
		"{nested-cascade|upper_bodyy.xml|}"
		"{scale|1|}{try-flip||}{@filename||}"
	);
	parser.printMessage();
	/*if (parser.has("help"))
	{
		help();
		return 0;
	}*/
	carCascadeName = "cars.xml";
	pedestrianCascadeName = "haarcascade_pedestrians.xml";
	fDirCascadeName = "f_direction_augmented.xml";
	stopSignCascadeName = "stopsigns.xml";

	scale = 1.2;// parser.get<double>("scale");
	if (scale < 1)
		scale = 1;
	tryflip = parser.has("try-flip");
	inputName = parser.get<string>("@filename");
	if (!parser.check())
	{
		parser.printErrors();
		return 0;
	}

	if (!carCascade.load(carCascadeName) || 
		!pedestrianCascade.load(pedestrianCascadeName)|| 
		!fDirSignCascade.load(fDirCascadeName)|| 
		!stopSignCascade.load(stopSignCascadeName))
	{
		cerr << "ERROR: Could not load classifier cascade" << endl;
		//help();
		return -1;
	}

	classifiers.push_back(carCascade);
	classifiers.push_back(pedestrianCascade);
	classifiers.push_back(fDirSignCascade);
	classifiers.push_back(stopSignCascade);

	/*if (inputName.empty() || (isdigit(inputName[0]) && inputName.size() == 1))
	{
		int camera = inputName.empty() ? 0 : inputName[0] - '0';
		if (!capture.open(camera))
			cout << "Capture from camera #" << camera << " didn't work" << endl;
	}*/

	capture.open(0);


	if (capture.isOpened())
	{
		cout << "Video capturing has been started ..." << endl;

		for (;;)
		{
			capture >> frame;
			if (frame.empty()) { continue; }

			Mat frame1 = frame.clone();
			
			op_codes = shapeDetect(frame1,boundingRects);
			if (!op_codes.empty())
			{
				for (vector<int>::iterator it = op_codes.begin(); it != op_codes.end(); ++it)
				{
					int code = *it;
					detectAndDraw(frame1, classifiers.at(code), scale, code,boundingRects);
					//cout << classifiers.at(code).empty();


				}
			}
			

			
			cout << " candidate size : " << boundingRects.circleBoundingRects.size() << endl;
			cout << " accepted size : " << boundingRects.circularSignRects.size()<< endl;

			//------------------------------------------- AR computing starts here -------------------------------------------------------------------------------

			dPoints pts;

			// Camera internals
			double focal_length = frame1.cols; // Approximate focal length.
			Point2d center = Point2d(frame1.cols / 2, frame1.rows / 2);
			Mat camera_matrix = (Mat_<double>(3, 3) << focal_length, 0, center.x, 0, focal_length, center.y, 0, 0, 1);
			Mat dist_coeffs = Mat::zeros(4, 1, DataType<double>::type); // Assuming no lens distortion

			cout << "Camera Matrix " << endl << camera_matrix << endl;

			// Output rotation and translation
			Mat rotation_vector; // Rotation in axis-angle form
			Mat translation_vector;

			// Solve pour les triangles
			solvePnP(pts.triangles_points, boundingRects.circularSignRects, camera_matrix, dist_coeffs, rotation_vector, translation_vector);

			// Solve pour les cercles
			solvePnP(pts.cercles_points, boundingRects.circularSignRects, camera_matrix, dist_coeffs, rotation_vector, translation_vector);

			// Solve pour les octogones
			solvePnP(pts.octos_points, boundingRects.circularSignRects, camera_matrix, dist_coeffs, rotation_vector, translation_vector);

			// Solve pour les carrés
			solvePnP(pts.carres_points, boundingRects.circularSignRects, camera_matrix, dist_coeffs, rotation_vector, translation_vector);

			// Project a 3D point (0, 0, 1000.0) into the image plane.
			// Pourrait etre utilise pour la RA
			/*
			vector<Point3d> test_end_point3D;
			vector<Point2d> test_end_point2D;
			test_end_point3D.push_back(Point3d(0, 0, 1000.0));

			projectPoints(test_end_point3D, rotation_vector, translation_vector, camera_matrix, dist_coeffs, test_end_point2D);


			for (int i = 0; i < image_points.size(); i++)
			{
				circle(frame1, image_points[i], 3, Scalar(0, 0, 255), -1);
			}

			line(frame1, image_points[0], test_end_point2D[0], Scalar(255, 0, 0), 2);

			cout << "Rotation Vector " << endl << rotation_vector << endl;
			cout << "Translation Vector" << endl << translation_vector << endl;

			cout << test_end_point2D << endl;

			// Display image.
			imshow("Output", frame1);
			waitKey(0);
			*/

			//------------------------------------------- AR computing ends here -------------------------------------------------------------------------------

			boundingRects.clearAllContainers();

			//detectAndDraw(frame1, fDirSignCascade, scale, PROHIBITED_DIRECTION_SIGN_DETECTION);

			//detectAndDraw(frame1, pedestrianCascade, scale, PEDESTRIAN_DETECTION);
			
			
			//carDetectAndDraw(frame1, carCascade, scale);
			//pedestrianDetectAndDraw(frame1, pedestrianCascade, scale);
			//signDetectAndDraw(frame1, signCascade, scale);
			//detectAndDraw(frame1, carCascade, nestedCascade, scale, tryflip);
			//detectAndDraw(frame1, pedestrianCascade, nestedCascade, scale, tryflip);


			char c = (char)waitKey(10);
			if (c == 27 || c == 'q' || c == 'Q')
				break;
		}
	}


	return 0;
}