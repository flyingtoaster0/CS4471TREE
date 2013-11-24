#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include "Clickable.h"


#define DOWN 1
#define UP 0

class PushButton : public Clickable
{

	vec3 upColor;
	vec3 downColor;
	vec3 color;

	int state;
	
	void setColor(vec3 color);
	void setDownColor(vec3 color);
	void setUpColor(vec3 color);


	void setState(int state);
	int getState();
	virtual void Execute() = 0;

public:
	PushButton();
	PushButton(float x, float y, float width, float height, std::string label);


	
	void mouseDown(float x, float y);
	void mouseUp(float x, float y);
	
	vec3 getColor();
};

#endif