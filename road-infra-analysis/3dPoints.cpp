#include "3dPoints.h"


dPoints::dPoints()
{
	// 3D model points.
	// Pour les triangles
	triangles_points.push_back(Point3d(0.0f, 0.0f, 0.0f));
	triangles_points.push_back(Point3d(0.0f, 0.0f, 0.5f));
	triangles_points.push_back(Point3d(0.5f, 0.0f, 0.0f));
	triangles_points.push_back(Point3d(0.0f, 0.0f, 1.0f));
	triangles_points.push_back(Point3d(1.0f, 0.0f, 0.0f));
	triangles_points.push_back(Point3d(0.5f, 0.0f, 1.0f));
	triangles_points.push_back(Point3d(1.0f, 0.0f, 0.5f));
	triangles_points.push_back(Point3d(1.0f, 0.0f, 1.0f));

	// Pour les cercles
	cercles_points.push_back(Point3d(0.0f, 0.0f, 0.0f));
	cercles_points.push_back(Point3d(0.0f, 0.0f, 0.85f / 2));
	cercles_points.push_back(Point3d(0.85f / 2, 0.0f, 0.0f));
	cercles_points.push_back(Point3d(0.0f, 0.0f, 0.85f));
	cercles_points.push_back(Point3d(0.85f, 0.0f, 0.0f));
	cercles_points.push_back(Point3d(0.85f / 2, 0.0f, 0.85f));
	cercles_points.push_back(Point3d(0.85f, 0.0f, 0.85f / 2));
	cercles_points.push_back(Point3d(0.85f, 0.0f, 0.85f));

	// Pour les octogones
	octos_points.push_back(Point3d(0.0f, 0.0f, 0.0f));
	octos_points.push_back(Point3d(0.0f, 0.0f, 0.4f));
	octos_points.push_back(Point3d(0.4f, 0.0f, 0.0f));
	octos_points.push_back(Point3d(0.0f, 0.0f, 0.8f));
	octos_points.push_back(Point3d(0.8f, 0.0f, 0.0f));
	octos_points.push_back(Point3d(0.4, 0.0f, 0.8f));
	octos_points.push_back(Point3d(0.8f, 0.0f, 0.4));
	octos_points.push_back(Point3d(0.8f, 0.0f, 0.8f));

	// Pour les carrés
	carres_points.push_back(Point3d(0.0f, 0.0f, 0.0f));
	carres_points.push_back(Point3d(0.0f, 0.0f, 0.35f));
	carres_points.push_back(Point3d(0.35f, 0.0f, 0.0f));
	carres_points.push_back(Point3d(0.0f, 0.0f, 0.7f));
	carres_points.push_back(Point3d(0.7f, 0.0f, 0.0f));
	carres_points.push_back(Point3d(0.35f, 0.0f, 0.7f));
	carres_points.push_back(Point3d(0.7f, 0.0f, 0.35f));
	carres_points.push_back(Point3d(0.7f, 0.0f, 0.7f));


	// Pour les piétons 
	pedestrians_points.push_back(Point3d(0.0f, 0.0f, 0.0f));
	pedestrians_points.push_back(Point3d(0.0f, 0.0f, 0.9f));
	pedestrians_points.push_back(Point3d(0.5f, 0.0f, 0.0f));
	pedestrians_points.push_back(Point3d(0.0f, 0.0f, 1.8f));
	pedestrians_points.push_back(Point3d(1.0f, 0.0f, 0.0f));
	pedestrians_points.push_back(Point3d(0.5f, 0.0f, 1.8f));
	pedestrians_points.push_back(Point3d(1.0f, 0.0f, 0.9f));
	pedestrians_points.push_back(Point3d(1.0f, 0.0f, 1.8f));


	// Pour les voitures 
	// ???
}


dPoints::~dPoints()
{
}
