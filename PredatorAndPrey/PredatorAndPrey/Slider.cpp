#include "Slider.h"
#include <iostream>

Slider::Slider() : Clickable()
{
	this->min = 0;
	this->max = 10;
}

Slider::Slider(float x, float y, float width, float height) : Clickable(x, y, width, height)
{
	// Hardcoded color. Change this later maybe

	this->setColor(vec3(0.0, 0.7, 0.8));
	
	this->value = 0;
	this->min = 0;
	this->max = 10;

}

Slider::Slider(float x, float y, float width, float height, int min, int max) : Clickable(x, y, width, height)
{
	// Hardcoded color. Change this later maybe

	this->setColor(vec3(0.0, 0.7, 0.8));
	
	this->value = 0;
	this->min = min;
	this->max = max;

}

void Slider::mouseDown(float x, float y)
{
	if(overlap(x,y))
	{
		//setState(DOWN);
		//setColor(downColor);

		int value = max * calcXpercent(x);
		setValue(value);
		std::cout<<"Slider value changed: "<<value<<'\n';

	}
}


float Slider::calcXpercent(float x)
{
	return x / (this->x + this->width);
}


void Slider::setColor(vec3 color)
{
	this->color = color;
}

vec3 Slider::getColor()
{
	return this->color;
}

void Slider::setValue(int value)
{
	this->value = value;
}

int Slider::getValue()
{
	return this->value;
}


int Slider::sliderBarPos()
{
	return  width / max * value;
}

int Slider::getSliderBarWidth()
{
	return  width / max;
}


