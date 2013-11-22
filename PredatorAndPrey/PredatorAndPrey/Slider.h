#ifndef SLIDER_H
#define SLIDER_H

#include "Clickable.h"


#define DOWN 1
#define UP 0

class Slider : public Clickable
{

	vec3 color;
	
	void setColor(vec3 color);

	int max, min, value;

	bool clicked;

	float calcXpercent(float x);

public:
	Slider();
	Slider(float x, float y, float width, float height);
	Slider(float x, float y, float width, float height, int min, int max);


	
	void mouseDown(float x, float y);
	void mouseUp(float x, float y);
	void mouseDrag(float x, float y);
	
	vec3 getColor();
	
	void setValue(int value);
	int getValue();

	
	int sliderBarPos();
	int getSliderBarWidth();


};

#endif