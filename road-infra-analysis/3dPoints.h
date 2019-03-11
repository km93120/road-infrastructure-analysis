#pragma once

#include "opencv2/core/core.hpp" 

using namespace std;
using namespace cv;

class dPoints
{
public:
	vector<Point3d> triangles_points;
	vector<Point3d> cercles_points;
	vector<Point3d> octos_points;
	vector<Point3d> carres_points;
	vector<Point3d> pedestrians_points;
	vector<Point3d> cars_points;

	dPoints();
	~dPoints();
};
