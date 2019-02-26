#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>

#include "road-infra-analysis.h"

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