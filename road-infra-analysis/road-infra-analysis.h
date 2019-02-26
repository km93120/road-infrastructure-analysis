#pragma once

#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "constants.h"

#include "BoundingRects.h"

#include <iostream>

using namespace std;
using namespace cv;

void detectAndDraw(Mat& img, CascadeClassifier& cascade, double scale, int op_code,BoundingRects &);

void carDetectAndDraw(Mat& img, CascadeClassifier& cascade, double scale);

void pedestrianDetectAndDraw(Mat& img, CascadeClassifier& cascade, double scale);
void signDetectAndDraw(Mat& img, CascadeClassifier& cascade, double scale);

static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0);

vector <int> shapeDetect(Mat &img,BoundingRects &);