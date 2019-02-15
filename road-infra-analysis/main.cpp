#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "constants.h"

#include <iostream>

#include "road-infra-analysis.h"

using namespace std;
using namespace cv;

int main(int argc, const char** argv)
{


	string cascadeName, cascadeName2, cascadeName3;
	VideoCapture capture;
	Mat frame, image;
	string inputName;
	bool tryflip;
	CascadeClassifier carCascade, pedestrianCascade, signCascade;
	double scale;

	vector<int> op_codes;

	cv::CommandLineParser parser(argc, argv,
		"{help h||}"
		"{carsCascade |cars|}"
		"{pedestriansCascade|haarcascade_pedestrians|}"
		"{signsCascade|stopsigns|}"
		"{nested-cascade|upper_bodyy.xml|}"
		"{scale|1|}{try-flip||}{@filename||}"
	);
	parser.printMessage();
	/*if (parser.has("help"))
	{
		help();
		return 0;
	}*/
	cascadeName = parser.get<string>("carsCascade") + ".xml";
	cascadeName2 = parser.get<string>("pedestriansCascade") + ".xml";
	cascadeName3 = parser.get <string>("signsCascade") + ".xml";

	cout << cascadeName;
	cout << cascadeName2;
	cout << cascadeName3;

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

	if (!carCascade.load(cascadeName) || !pedestrianCascade.load(cascadeName2)|| !signCascade.load(cascadeName3))
	{
		cerr << "ERROR: Could not load classifier cascade" << endl;
		//help();
		return -1;
	}
	if (inputName.empty() || (isdigit(inputName[0]) && inputName.size() == 1))
	{
		int camera = inputName.empty() ? 0 : inputName[0] - '0';
		if (!capture.open(camera))
			cout << "Capture from camera #" << camera << " didn't work" << endl;
	}


	if (capture.isOpened())
	{
		cout << "Video capturing has been started ..." << endl;

		for (;;)
		{
			capture >> frame;
			if (frame.empty()) { continue; }

			Mat frame1 = frame.clone();
			
			/*op_codes = shapeDetect(frame1);
			for (vector<int>::iterator it = op_codes.begin(); it != op_codes.end(); ++it)
			{ 
				int code = *it;
				switch (code)
				{
					case 
				}
				cout << code;


			}*/
			detectAndDraw(frame1, signCascade, scale, STOP_SIGN_DETECTION);
			/*detectAndDraw(frame1, carCascade, scale, CARS_DETECTION);
			detectAndDraw(frame1, pedestrianCascade, scale, PEDESTRIAN_DETECTION);*/
			
			
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