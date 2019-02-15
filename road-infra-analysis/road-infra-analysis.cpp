#include "road-infra-analysis.h"

static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;

	return (dx1*dx2 + dy1 * dy2) / sqrt((dx1*dx1 + dy1 * dy1)*(dx2*dx2 + dy2 * dy2) + 1e-10);
}

vector <int> shapeDetect(Mat &img) 
{

	clock_t begin = clock();

	bool triangleDetected = false;
	bool rectangleDetected = false;
	bool circleDetected = false;
	bool hexagonDetected = false;

	vector<int> outVector;
	// Convert to grayscale
	Mat gray,thresholded;

	
	cvtColor(img, gray, COLOR_BGR2GRAY);
	//threshold(gray, thresholded, 100, 255, 0);

	// Use Canny instead of threshold to catch squares with gradient shading
	Mat bw;
	Canny(gray, bw, 100, 200, 5); //0,50 originally

	// Find contours
	vector<std::vector<cv::Point> > contours;
	findContours(bw.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	vector<cv::Point> approx;
	cv::Mat dst = img.clone();

	for (int i = 0; i < contours.size(); i++)
	{
		// Approximate contour with accuracy proportional
		// to the contour perimeter
		cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);

		// Skip small or non-convex objects 
		if (std::fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx))
			continue;

		if (approx.size() == 3)
		{
			
			int vtc = approx.size();

			// Get the cosines of all corners
			std::vector<double> cos;
			for (int j = 2; j < vtc + 1; j++)
				cos.push_back(angle(approx[j%vtc], approx[j - 2], approx[j - 1]));

			// Sort ascending the cosine values
			sort(cos.begin(), cos.end());

			// Get the lowest and the highest cosine
			double mincos = cos.front();
			double maxcos = cos.back();

			if (abs(mincos) <= 0.55 && abs(maxcos) >= 0.45)
			{
				triangleDetected = true;
			}
			   // Triangles
			



		}
		else if (approx.size() >= 4 && approx.size() <= 6)
		{
			// Number of vertices of polygonal curve
			int vtc = approx.size();

			// Get the cosines of all corners
			vector<double> cos;
			for (int j = 2; j < vtc + 1; j++)
				cos.push_back(angle(approx[j%vtc], approx[j - 2], approx[j - 1]));

			// Sort ascending the cosine values
			std::sort(cos.begin(), cos.end());

			// Get the lowest and the highest cosine
			double mincos = cos.front();
			double maxcos = cos.back();

			// Use the degrees obtained above and the number of vertices
			// to determine the shape of the contour
			if (vtc == 4 && mincos >= -0.1 && maxcos <= 0.3)
			{
				cout << "rectangle";
				rectangleDetected = true;

			}
			else if (vtc == 5 && mincos >= -0.34 && maxcos <= -0.27)
			{
				cout << "penta";
			}
			else if (vtc == 6 && mincos >= -0.55 && maxcos <= -0.45)
			{
				hexagonDetected = true;
				cout << "hexa";
			}
				
		}
		else
		{
			// Detect and label circles
			double area = cv::contourArea(contours[i]);
			cv::Rect r = cv::boundingRect(contours[i]);
			int radius = r.width / 2;

			if (std::abs(1 - ((double)r.width / r.height)) <= 0.2 &&
				std::abs(1 - (area / (CV_PI * std::pow(radius, 2)))) <= 0.2)
			{
				circleDetected = true;
			}
				
		}
	}

	if (triangleDetected)
	{
		outVector.push_back(PEDESTRIAN_CROSSING_SIGN_DETECTION);
		outVector.push_back(RIGHT_PRECEDENCE_SIGN_DETECTION);
	}
	
	if (rectangleDetected)
	{
		
	}
	
	if (circleDetected)
	{
		outVector.push_back(PROHIBITED_DIRECTION_SIGN_DETECTION);
		
	}

	if (hexagonDetected)
	{
		outVector.push_back(STOP_SIGN_DETECTION);
	}

	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	std::cout << elapsed_secs << std::endl;
	imshow("s", thresholded);
	return outVector;
	


}

void detectAndDraw(Mat & img, CascadeClassifier & cascade, double scale, int op_code)
{
	double t = 0;
	string outString;
	vector<Rect> objects;  
	const static Scalar colors[] =
	{
		Scalar(255,0,0),
		Scalar(255,128,0),
		Scalar(255,255,0),
		Scalar(0,255,0),
		Scalar(0,128,255),
		Scalar(0,255,255),
		Scalar(0,0,255),
		Scalar(255,0,255)
	};
	Mat gray, smallImg;

	cvtColor(img, gray, COLOR_BGR2GRAY);
	double fx = 1 / scale;
	resize(gray, smallImg, Size(), fx, fx, INTER_LINEAR_EXACT);
	equalizeHist(smallImg, smallImg);
	switch (op_code)
	{
		case CARS_DETECTION:
			t = (double)getTickCount();
			cascade.detectMultiScale(
				smallImg, 
				objects,
				1.05, 
				6,
				CASCADE_DO_CANNY_PRUNING,
				Size(30, 30));
			
			outString = "car";


		break;
		case PEDESTRIAN_DETECTION:
			cascade.detectMultiScale(
				smallImg, 
				objects,
				1.1, 
				6, 
				CASCADE_DO_CANNY_PRUNING,
				Size(20, 20));

			outString = "pedestrian";
		break;
		case STOP_SIGN_DETECTION:
			t = (double)getTickCount();
			cascade.detectMultiScale(
				smallImg,
				objects,
				1.05,
				10,
				CASCADE_DO_CANNY_PRUNING,
				Size(30, 30));

			outString = "stop_sign";


	}

			

	

	t = (double)getTickCount() - t;
	printf("detection time = %g ms\n", t * 1000 / getTickFrequency());
	for (size_t i = 0; i < objects.size(); i++)
	{
		Rect r = objects[i];
		Mat smallImgROI;

		Point center;
		Scalar color = colors[i % 8];	//tourner et changer la couleur de l'image augmenterait les chances de détection
		int radius;

		double aspect_ratio = (double)r.width / r.height;
		if (0.75 < aspect_ratio && aspect_ratio < 1.3)
		{
			center.x = cvRound((r.x + r.width*0.5)*scale);
			center.y = cvRound((r.y + r.height*0.5)*scale);
			radius = cvRound((r.width + r.height)*0.25*scale);
			circle(img, center, radius, color, 3, 8, 0);
		}
		else
			rectangle(img, Point(cvRound(r.x*scale), cvRound(r.y*scale)),
				Point(cvRound((r.x + r.width - 1)*scale), cvRound((r.y + r.height - 1)*scale)),
				color, 3, 8, 0);


		putText(img, outString, Point(r.x + r.width, r.y + r.height), FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(255, 255, 255));


	}
	imshow("result", img);			// on affiche le resultat

}

void carDetectAndDraw(Mat& img, CascadeClassifier& cascade,
	double scale)
{
	double t = 0;
	vector<Rect> cars;  // les rectangles qui contiendront les visages
	const static Scalar colors[] =
	{
		Scalar(255,0,0),
		Scalar(255,128,0),
		Scalar(255,255,0),
		Scalar(0,255,0),
		Scalar(0,128,255),
		Scalar(0,255,255),
		Scalar(0,0,255),
		Scalar(255,0,255)
	};
	Mat gray, smallImg;

	cvtColor(img, gray, COLOR_BGR2GRAY);
	double fx = 1 / scale;
	resize(gray, smallImg, Size(), fx, fx, INTER_LINEAR_EXACT);
	equalizeHist(smallImg, smallImg);

	t = (double)getTickCount();
	cascade.detectMultiScale(smallImg, cars,
		1.05, 6, 0
		//|CASCADE_FIND_BIGGEST_OBJECT,
		//|CASCADE_DO_ROUGH_SEARCH,
		//| CASCADE_SCALE_IMAGE
		| CASCADE_DO_CANNY_PRUNING,
		Size(30, 30));

	t = (double)getTickCount() - t;
	printf("detection time = %g ms\n", t * 1000 / getTickFrequency()); 
	for (size_t i = 0; i < cars.size(); i++)		
	{
		Rect r = cars[i];
		Mat smallImgROI;

		Point center;
		Scalar color = colors[i % 8];	//tourner et changer la couleur de l'image augmenterait les chances de détection
		int radius;

		double aspect_ratio = (double)r.width / r.height;
		if (0.75 < aspect_ratio && aspect_ratio < 1.3)
		{
			center.x = cvRound((r.x + r.width*0.5)*scale);
			center.y = cvRound((r.y + r.height*0.5)*scale);
			radius = cvRound((r.width + r.height)*0.25*scale);
			circle(img, center, radius, color, 3, 8, 0);
		}
		else
			rectangle(img, Point(cvRound(r.x*scale), cvRound(r.y*scale)),
				Point(cvRound((r.x + r.width - 1)*scale), cvRound((r.y + r.height - 1)*scale)),
				color, 3, 8, 0);

		string outString = string("car");
		putText(img, outString, Point(r.x + r.width, r.y + r.height), FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(255, 255, 255));


	}
	imshow("result", img);			// on affiche le resultat
}

void pedestrianDetectAndDraw(Mat& img, CascadeClassifier& cascade, double scale)
{
	double t = 0;
	vector<Rect> pedestrians;  // les rectangles qui contiendront les visages
	const static Scalar colors[] =
	{
		Scalar(255,0,0),
		Scalar(255,128,0),
		Scalar(255,255,0),
		Scalar(0,255,0),
		Scalar(0,128,255),
		Scalar(0,255,255),
		Scalar(0,0,255),
		Scalar(255,0,255)
	};
	Mat gray, smallImg;

	cvtColor(img, gray, COLOR_BGR2GRAY);
	double fx = 1 / scale;
	resize(gray, smallImg, Size(), fx, fx, INTER_LINEAR_EXACT);
	equalizeHist(smallImg, smallImg);

	t = (double)getTickCount();
	cascade.detectMultiScale(smallImg, pedestrians,
		1.1, 6, 0
		//|CASCADE_FIND_BIGGEST_OBJECT,
		//| CASCADE_DO_ROUGH_SEARCH,
		//|CASCADE_SCALE_IMAGE,
		| CASCADE_DO_CANNY_PRUNING,
		Size(20, 20));

	t = (double)getTickCount() - t;
	printf("detection time = %g ms\n", t * 1000 / getTickFrequency()); // détection du visage effectuée a ce niveau la
	for (size_t i = 0; i < pedestrians.size(); i++)		//recherche d'autres élements dans les rectanlges contenant les visages, essayer avec un classifieur visage + yeux? 
	{
		Rect r = pedestrians[i]; 
		Mat smallImgROI;
		vector<Rect> nestedObjects;	//les objets identifiables dans les rectangles
		Point center;
		Scalar color = colors[i % 8];	//tourner et changer la couleur de l'image augmenterait les chances de détection
		int radius;

		double aspect_ratio = (double)r.width / r.height;
		if (0.75 < aspect_ratio && aspect_ratio < 1.3)
		{
			center.x = cvRound((r.x + r.width*0.5)*scale);
			center.y = cvRound((r.y + r.height*0.5)*scale); 
			radius = cvRound((r.width + r.height)*0.25*scale);
			circle(img, center, radius, color, 3, 8, 0);
		}
		else
			rectangle(img, Point(cvRound(r.x*scale), cvRound(r.y*scale)),
				Point(cvRound((r.x + r.width - 1)*scale), cvRound((r.y + r.height - 1)*scale)),
				color, 3, 8, 0);

		string outString = string("pedestrian ");
		putText(img, outString, Point(r.x + r.width, r.y + r.height), FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(255, 255, 255));


	}
	imshow("result", img);
}



void signDetectAndDraw(Mat& img, CascadeClassifier& cascade, double scale)
{
	double t = 0;
	vector<Rect> pedestrians;  // les rectangles qui contiendront les visages
	const static Scalar colors[] =
	{
		Scalar(255,0,0),
		Scalar(255,128,0),
		Scalar(255,255,0),
		Scalar(0,255,0),
		Scalar(0,128,255),
		Scalar(0,255,255),
		Scalar(0,0,255),
		Scalar(255,0,255)
	};
	Mat gray, smallImg;

	cvtColor(img, gray, COLOR_BGR2GRAY);
	double fx = 1 / scale;
	resize(gray, smallImg, Size(), fx, fx, INTER_LINEAR_EXACT);
	equalizeHist(smallImg, smallImg);

	t = (double)getTickCount();
	cascade.detectMultiScale(smallImg, pedestrians,
		1.025, 10, 0
		//|CASCADE_FIND_BIGGEST_OBJECT,
		//| CASCADE_DO_ROUGH_SEARCH,
		//|CASCADE_SCALE_IMAGE,
		| CASCADE_DO_CANNY_PRUNING,
		Size(30, 30));

	t = (double)getTickCount() - t;
	printf("detection time = %g ms\n", t * 1000 / getTickFrequency()); // détection du visage effectuée a ce niveau la
	for (size_t i = 0; i < pedestrians.size(); i++)		//recherche d'autres élements dans les rectanlges contenant les visages, essayer avec un classifieur visage + yeux? 
	{
		Rect r = pedestrians[i];
		Mat smallImgROI;
		vector<Rect> nestedObjects;	//les objets identifiables dans les rectangles
		Point center;
		Scalar color = colors[i % 8];	//tourner et changer la couleur de l'image augmenterait les chances de détection
		int radius;

		double aspect_ratio = (double)r.width / r.height;
		if (0.75 < aspect_ratio && aspect_ratio < 1.3)
		{
			center.x = cvRound((r.x + r.width*0.5)*scale);
			center.y = cvRound((r.y + r.height*0.5)*scale);
			radius = cvRound((r.width + r.height)*0.25*scale);
			circle(img, center, radius, color, 3, 8, 0);
		}
		else
			rectangle(img, Point(cvRound(r.x*scale), cvRound(r.y*scale)),
				Point(cvRound((r.x + r.width - 1)*scale), cvRound((r.y + r.height - 1)*scale)),
				color, 3, 8, 0);

		string outString = string("sign");
		putText(img, outString, Point(r.x + r.width, r.y + r.height), FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(255, 255, 255));


	}
	imshow("result", img);
}
