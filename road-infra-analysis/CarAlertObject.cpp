#include "CarAlertObject.h"



CarAlertObject::CarAlertObject()
{

	dimensions.push_back(Point3d(0.0f, 0.0f, 0.0f));
	dimensions.push_back(Point3d(1.19f, 0.0f, 0.0f));
	dimensions.push_back(Point3d(1.19f, 0.9f, 0.0f));
	dimensions.push_back(Point3d(0.9f, 0.0f, 0.0f));
}



CarAlertObject::~CarAlertObject()
{
}
