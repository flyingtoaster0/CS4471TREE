#ifndef CLICKABLE_H
#define CLICKABLE_H

#include "vec.h"

using namespace Angel;
class Clickable
{
protected:
	float x, y, width, height;
	std::string label;
public:
	Clickable();
	Clickable(float x, float y, float height, float width, std::string label);

	void mouseDown(float x, float y);
	void mouseUp(float x, float y);
	
	bool overlap(float x, float y);
	
	float getX();
	float getY();
	float getWidth();
	float getHeight();
	std::string getLabel();

	
	

};

#endif