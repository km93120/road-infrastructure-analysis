#include "CircularSignRect.h"


CircularSignRect::CircularSignRect(Rect r)
{
	rect = r; 

	// 2D image points. 
	RWdimensions.push_back(Point2d(rect.x, rect.y));
	RWdimensions.push_back(Point2d(rect.x, rect.y + -rect.height));
	RWdimensions.push_back(Point2d(rect.x + -rect.width, rect.y));
	RWdimensions.push_back(Point2d(rect.x, (rect.y + -rect.height)/2));
	RWdimensions.push_back(Point2d((rect.x + -rect.width)/2, rect.y));
	RWdimensions.push_back(Point2d((rect.x + -rect.width)/2, rect.y + -rect.height));
	RWdimensions.push_back(Point2d(rect.x + -rect.width, (rect.y + -rect.height)/2));
	RWdimensions.push_back(Point2d(rect.x + -rect.width, rect.y + -rect.height));
}


CircularSignRect::~CircularSignRect()
{

}
