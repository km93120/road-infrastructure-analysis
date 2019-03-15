#include "road-infra-analysis.h"

static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;

	return (dx1*dx2 + dy1 * dy2) / sqrt((dx1*dx1 + dy1 * dy1)*(dx2*dx2 + dy2 * dy2) + 1e-10);
}

vector <int> shapeDetect(Mat &img,BoundingRects &boundingRects) 
{
	
	clock_t begin = clock();
	int lowCannyThreshold = 75;
	
	bool triangleDetected = false;
	bool rectangleDetected = false;
	bool circleDetected = false;
	bool octogonDetected = false;

	vector<int> outVector;
	// Convert to grayscale
	Mat gray,thresholded;

	
	cvtColor(img, gray, COLOR_BGR2GRAY);
	//threshold(gray, thresholded, 100, 255, 0);

	// Use Canny instead of threshold to catch squares with gradient shading
	Mat bw,structuringElement;

	bilateralFilter(gray, bw, 5,10,10);
	//bw = gray.clone();
	//GaussianBlur(bw, blurred, Size(5, 5), 0.0, 0.0);
	Canny(bw, bw, lowCannyThreshold,lowCannyThreshold*3); //0,50 originally
	
	structuringElement = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
	morphologyEx(bw, bw, MORPH_CLOSE, structuringElement);

	// Find contours
	vector<std::vector<cv::Point> > contours;
	findContours(bw.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	
	vector<cv::Point> approx;
	cv::Mat dst = img.clone();

	for (int i = 0; i < contours.size(); i++)
	{
		// Approximate contour with accuracy proportional
		// to the contour perimeter
		approxPolyDP(Mat(contours[i]), approx, 3, true);
		//approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);

		// Skip small or non-convex objects 
		if (std::fabs(cv::contourArea(contours[i])) < 1000 || !cv::isContourConvex(approx))
			continue;
	
		if (approx.size() == 3)
		{
			int verticesNumber = approx.size();

			cout << "triangle";
			Rect bdr = boundingRect(approx);
			rectangle(bw, bdr, Scalar(255, 0, 0));
			boundingRects.triangleBoundingRects.push_back(boundingRect(approx));
			triangleDetected = true;
			/*if (abs(mincos) <= 0.55 && abs(maxcos) >= 0.45)
			{
				triangleDetected = true;
				Rect bdr = boundingRect(approx);
				rectangle(bw, bdr, Scalar(255, 0, 0));
				boundingRects.triangleBoundingRects.push_back(boundingRect(approx));
				cout << "triangleTrue";
			}*/
			   // Triangles
		}
		if (approx.size() == 8)
		{
			cout << "octo";
			octogonDetected = true;
			Rect bdr = boundingRect(approx);
			boundingRects.octogonRects.push_back(bdr);
		}
		else
		{
			// Detect and label circles
			double area = cv::contourArea(contours[i]);
			cv::Rect r = cv::boundingRect(contours[i]);
			int radius = r.width / 2;

			if (std::abs(1 - ((double)r.width / r.height)) <= 0.2 &&
				std::abs(1 - (area / (CV_PI * std::pow(radius, 2)))) <= 0.2
				)
			{
				cout << "circle" << endl;
				//cout << "APPROX SIZE"<< approx.size() << endl ;
				boundingRects.circleBoundingRects.push_back(r);
				circleDetected = true;
			}
				
		}
		//cout << approx.size();
	}

	/*vector<Vec3f> circles;

	/// Apply the Hough Transform to find the circles
	HoughCircles(gray, circles, HOUGH_GRADIENT, 1, gray.rows / 16, 255, 50, 0, 0);

	/// Draw the circles detected
	for (size_t i = 0; i < circles.size(); i++)
	{
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		Rect r = Rect(center.x - radius, center.y - radius, 2 * radius, 2 * radius);
		rectangle(bw, r, Scalar(255, 0, 0), -1, 8, 0);
		boundingRects.circleBoundingRects.push_back(r);
		circleDetected = true;
		cout << "circle";
	}*/


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

	if (octogonDetected)
	{
		outVector.push_back(STOP_SIGN_DETECTION);
	}

	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	std::cout << elapsed_secs << std::endl;
	imshow("s", bw);
	return outVector;
	


}

void detectAndDraw(Mat & img, CascadeClassifier & cascade, double scale, int op_code,BoundingRects & boundingRects)
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
	Mat gray, smallImg,ROI;

	cvtColor(img, gray, COLOR_BGR2GRAY);
	double fx = 1 / scale;
	resize(gray, smallImg, Size(), fx, fx, INTER_LINEAR_EXACT);
	equalizeHist(gray, gray);
	Rect temp;
	switch (op_code)
	{
		case CARS_DETECTION:
			t = (double)getTickCount();
			cascade.detectMultiScale(
				gray, 
				objects,
				1.05, 
				6,
				CASCADE_DO_CANNY_PRUNING,
				Size(30, 30));
			
			outString = "car";
			for (int i = 0; i < objects.size() ; i++)
			{
				boundingRects.carRects.push_back(objects.at(i));
			}


		break;
		case PEDESTRIAN_DETECTION:
			cascade.detectMultiScale(
				gray, 
				objects,
				1.1, 
				6, 
				CASCADE_DO_CANNY_PRUNING,
				Size(20, 20));
			for (int i = 0; i < objects.size(); i++)
			{
				boundingRects.pedestrianRects.push_back(objects.at(i));
			}

			outString = "pedestrian";
		break;
		case STOP_SIGN_DETECTION:
			for (int i = 0; i < boundingRects.octogonRects.size(); i++)
			{
				temp = boundingRects.octogonRects.at(i);
				Rect r;
				cv::Size deltaSize(boundingRects.octogonRects.at(i).width * 0.1f,
					boundingRects.octogonRects.at(i).height *0.1f);

				cv::Point offset(deltaSize.width / 2, deltaSize.height / 2);

				//boundingRects.octogonRects.at(i) += deltaSize;
				//boundingRects.octogonRects.at(i) -= offset;
				/*Rect expanded = boundingRects.circleBoundingRects.at(i) + Size(boundingRects.circleBoundingRects.at(i).width*1.1,
																			   boundingRects.circleBoundingRects.at(i).height*1.1);*/
				rectangle(img, boundingRects.octogonRects.at(i), Scalar(255, 0, 0));
				ROI = img(boundingRects.octogonRects.at(i));
				//imshow("ss", img);

				cascade.detectMultiScale(
					ROI,
					objects,
					1.05,
					2,
					CASCADE_FIND_BIGGEST_OBJECT,
					Size(5, 5));

				if (!objects.empty())
				{
					boundingRects.stopSignRects.push_back(StopSignRect(temp));//candidate contour is accepted
				}

			}
			outString = "stop_sign";
			break;
		case PEDESTRIAN_CROSSING_SIGN_DETECTION:

			for (int i = 0; i < boundingRects.triangleBoundingRects.size(); i++)
			{
				temp = boundingRects.triangleBoundingRects.at(i);
				Rect r;
				cv::Size deltaSize(boundingRects.triangleBoundingRects.at(i).width * 0.1f,
					boundingRects.triangleBoundingRects.at(i).height *0.1f);

				cv::Point offset(deltaSize.width / 2, deltaSize.height / 2);

				//boundingRects.triangleBoundingRects.at(i) += deltaSize;
				//boundingRects.triangleBoundingRects.at(i) -= offset;
				/*Rect expanded = boundingRects.circleBoundingRects.at(i) + Size(boundingRects.circleBoundingRects.at(i).width*1.1,
																			   boundingRects.circleBoundingRects.at(i).height*1.1);*/
				rectangle(img, boundingRects.triangleBoundingRects.at(i), Scalar(255, 0, 0));
				ROI = img(boundingRects.triangleBoundingRects.at(i));
				//imshow("ss", img);

				cascade.detectMultiScale(
					ROI,
					objects,
					1.05,
					5,
					CASCADE_FIND_BIGGEST_OBJECT,
					Size(5, 5));

				if (!objects.empty())
				{
					boundingRects.crossSignRects.push_back(CrossSignRect(temp));//candidate contour is accepted
				}
				
			}
			outString = "pedestrian crossing";
			break;
		case PROHIBITED_DIRECTION_SIGN_DETECTION:
			for (int i = 0; i < boundingRects.circleBoundingRects.size(); i++)
			{
				temp = boundingRects.circleBoundingRects.at(i);
				if (temp.x + temp.width > img.cols || temp.y + temp.height > img.rows|| temp.x < 0 || temp.y < 0 )
					continue;
				Rect r;
				cv::Size deltaSize(boundingRects.circleBoundingRects.at(i).width * 0.2f,
					boundingRects.circleBoundingRects.at(i).height *0.2f);

				cv::Point offset(deltaSize.width / 2, deltaSize.height / 2); 
				 
				
				//boundingRects.circleBoundingRects.at(i) += deltaSize;
				//boundingRects.circleBoundingRects.at(i) -= offset;
				/*Rect expanded = boundingRects.circleBoundingRects.at(i) + Size(boundingRects.circleBoundingRects.at(i).width*1.1,
																			   boundingRects.circleBoundingRects.at(i).height*1.1);*/
				rectangle(img, boundingRects.circleBoundingRects.at(i), Scalar(255, 0, 0));
				ROI = img(boundingRects.circleBoundingRects.at(i));
				imshow("ss", img);
				
				cascade.detectMultiScale(
					ROI,
					objects,
					1.05,
					5,
					CASCADE_FIND_BIGGEST_OBJECT,
					Size(5, 5));

				if (!objects.empty())
				{
					boundingRects.circularSignRects.push_back(CircularSignRect(temp));//candidate contour is accepted
				}
				outString = "sensInterdit";
			}
			break;
		case RIGHT_PRECEDENCE_SIGN_DETECTION:

			for (int i = 0; i < boundingRects.triangleBoundingRects.size(); i++)
			{
				temp = boundingRects.triangleBoundingRects.at(i);
				Rect r;
				cv::Size deltaSize(boundingRects.triangleBoundingRects.at(i).width * 0.1f,
					boundingRects.triangleBoundingRects.at(i).height *0.1f);
				cv::Point offset(deltaSize.width / 2, deltaSize.height / 2);


				//boundingRects.triangleBoundingRects.at(i) += deltaSize;
				//boundingRects.triangleBoundingRects.at(i) -= offset;
				/*Rect expanded = boundingRects.circleBoundingRects.at(i) + Size(boundingRects.circleBoundingRects.at(i).width*1.1,
																			   boundingRects.circleBoundingRects.at(i).height*1.1);*/
				rectangle(img, boundingRects.triangleBoundingRects.at(i), Scalar(255, 0, 0));
				ROI = img(boundingRects.triangleBoundingRects.at(i));
				//imshow("ss", img);

				cascade.detectMultiScale(
					ROI,
					objects,
					1.05,
					2,
					CASCADE_FIND_BIGGEST_OBJECT,
					Size(5, 5));

				if (!objects.empty())
				{
					boundingRects.rpSignRects.push_back(RPSignRect(temp));//candidate contour is accepted
				}

			}
			outString = "right precedence warning";
			break;

		


	}

	return;

	
			// on affiche le resultat

}

void drawRects(BoundingRects rects,Mat & frame)
{
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

	for (size_t i = 0; i < rects.carRects.size(); i++)
	{
		Rect r = rects.carRects[i].rect;

		Point center;
		Scalar color = colors[i % 8];
		int radius;

		double aspect_ratio = (double)r.width / r.height;
		if (0.75 < aspect_ratio && aspect_ratio < 1.3)
		{
			center.x = cvRound((r.x + r.width*0.5));
			center.y = cvRound((r.y + r.height*0.5));
			radius = cvRound((r.width + r.height)*0.25);
			circle(frame, center, radius, color, 3, 8, 0);
		}
		else
			rectangle(frame, Point(cvRound(r.x), cvRound(r.y)),
				Point(cvRound((r.x + r.width - 1)), cvRound((r.y + r.height - 1))),
				color, 3, 8, 0);


		putText(frame, "car", Point(r.x + r.width, r.y + r.height), FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(255, 255, 255));


	}

	for (size_t i = 0; i < rects.pedestrianRects.size(); i++)
	{
		Rect r = rects.pedestrianRects[i].rect;

		Point center;
		Scalar color = colors[i % 8];
		int radius;

		double aspect_ratio = (double)r.width / r.height;
		if (0.75 < aspect_ratio && aspect_ratio < 1.3)
		{
			center.x = cvRound((r.x + r.width*0.5));
			center.y = cvRound((r.y + r.height*0.5));
			radius = cvRound((r.width + r.height)*0.25);
			circle(frame, center, radius, color, 3, 8, 0);
		}
		else
			rectangle(frame, Point(cvRound(r.x), cvRound(r.y)),
				Point(cvRound((r.x + r.width - 1)), cvRound((r.y + r.height - 1))),
				color, 3, 8, 0);


		putText(frame, "pedestrian", Point(r.x + r.width, r.y + r.height), FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(255, 255, 255));


	}

	for (size_t i = 0; i < rects.circularSignRects.size(); i++) //sens interdit
	{
		Rect r = rects.circularSignRects[i].rect;

		Point center;
		Scalar color = colors[i % 8];
		int radius;

		double aspect_ratio = (double)r.width / r.height;
		if (0.75 < aspect_ratio && aspect_ratio < 1.3)
		{
			center.x = cvRound((r.x + r.width*0.5));
			center.y = cvRound((r.y + r.height*0.5));
			radius = cvRound((r.width + r.height)*0.25);
			circle(frame, center, radius, color, 3, 8, 0);
		}
		else
			rectangle(frame, Point(cvRound(r.x), cvRound(r.y)),
				Point(cvRound((r.x + r.width - 1)), cvRound((r.y + r.height - 1))),
				color, 3, 8, 0);


		putText(frame, "sens interdit", Point(r.x + r.width, r.y + r.height), FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(255, 255, 255));


	}
	for (size_t i = 0; i < rects.crossSignRects.size(); i++)
	{
		Rect r = rects.crossSignRects[i].rect;

		Point center;
		Scalar color = colors[i % 8];
		int radius;

		double aspect_ratio = (double)r.width / r.height;
		if (0.75 < aspect_ratio && aspect_ratio < 1.3)
		{
			center.x = cvRound((r.x + r.width*0.5));
			center.y = cvRound((r.y + r.height*0.5));
			radius = cvRound((r.width + r.height)*0.25);
			circle(frame, center, radius, color, 3, 8, 0);
		}
		else
			rectangle(frame, Point(cvRound(r.x), cvRound(r.y)),
				Point(cvRound((r.x + r.width - 1)), cvRound((r.y + r.height - 1))),
				color, 3, 8, 0);


		putText(frame, "pedestrian crossing", Point(r.x + r.width, r.y + r.height), FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(255, 255, 255));


	}
	for (size_t i = 0; i < rects.stopSignRects.size(); i++)
	{
		Rect r = rects.stopSignRects[i].rect;

		Point center;
		Scalar color = colors[i % 8];
		int radius;

		double aspect_ratio = (double)r.width / r.height;
		if (0.75 < aspect_ratio && aspect_ratio < 1.3)
		{
			center.x = cvRound((r.x + r.width*0.5));
			center.y = cvRound((r.y + r.height*0.5));
			radius = cvRound((r.width + r.height)*0.25);
			circle(frame, center, radius, color, 3, 8, 0);
		}
		else
			rectangle(frame, Point(cvRound(r.x), cvRound(r.y)),
				Point(cvRound((r.x + r.width - 1)), cvRound((r.y + r.height - 1))),
				color, 3, 8, 0);


		putText(frame, "stop sign", Point(r.x + r.width, r.y + r.height), FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(255, 255, 255));


	}
	for (size_t i = 0; i < rects.rpSignRects.size(); i++)
	{
		Rect r = rects.rpSignRects[i].rect;

		Point center;
		Scalar color = colors[i % 8];
		int radius;

		double aspect_ratio = (double)r.width / r.height;
		if (0.75 < aspect_ratio && aspect_ratio < 1.3)
		{
			center.x = cvRound((r.x + r.width*0.5));
			center.y = cvRound((r.y + r.height*0.5));
			radius = cvRound((r.width + r.height)*0.25);
			circle(frame, center, radius, color, 3, 8, 0);
		}
		else
			rectangle(frame, Point(cvRound(r.x), cvRound(r.y)),
				Point(cvRound((r.x + r.width - 1)), cvRound((r.y + r.height - 1))),
				color, 3, 8, 0);


		putText(frame, "rp_sign", Point(r.x + r.width, r.y + r.height), FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(255, 255, 255));


	}
	imshow("result", frame);			// on affiche le resultat


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
