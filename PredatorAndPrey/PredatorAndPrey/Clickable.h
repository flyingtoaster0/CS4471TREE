#ifndef CLICKABLE_H
#define CLICKABLE_H

#include "vec.h"

using namespace Angel;
class Clickable
{
protected:
	float x, y, width, height;
public:
	Clickable();
	Clickable(float x, float y, float height, float width);

	void mouseDown(float x, float y);
	void mouseUp(float x, float y);
	
	bool overlap(float x, float y);
	
	float getX();
	float getY();
	float getWidth();
	float getHeight();

};

#endif