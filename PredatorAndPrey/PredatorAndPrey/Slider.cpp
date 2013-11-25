#include "Slider.h"
#include <iostream>

Slider::Slider() : Clickable()
{
	this->min = 0;
	this->max = 10;
	this->clicked = false;
}

Slider::Slider(float x, float y, float width, float height, std::string label) : Clickable(x, y, width, height, label)
{
	// Hardcoded color. Change this later maybe

	this->setColor(vec3(0.0, 0.7, 0.8));
	
	this->value = 0;
	this->min = 0;
	this->max = 10;
	this->clicked = false;
}

Slider::Slider(float x, float y, float width, float height,std::string label, int min, int max) : Clickable(x, y, width, height, label)
{
	// Hardcoded color. Change this later maybe

	this->setColor(vec3(0.0, 0.7, 0.8));
	
	this->value = 0;
	this->min = min;
	this->max = max;
	this->clicked = false;

}

void Slider::mouseDown(float x, float y)
{
	if(overlap(x,y))
	{
		int value = max * calcXpercent(x);
		setValue(value);
		//std::cout<<"Slider value changed: "<<value<<'\n';
		this->clicked = true;
	}
}


void Slider::mouseUp(float x, float y)
{
	if (this->clicked && this->action && overlap(x,y))
	{
		this->clicked = false;
		this->executeAction();
		printf("%s", this->getLabel());
	}
	
		
}

void Slider::mouseDrag(float x, float y)
{
	if(this->clicked && overlap(x,y))
	{
		int value = max * calcXpercent(x);
		setValue(value);
		//std::cout<<"Slider value changed: "<<value<<'\n';
	}
}



float Slider::calcXpercent(float x)
{
	return (x - this->x) / this->width;
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
	return this->value + 1;
}


int Slider::sliderBarPos()
{
	return  width / max * value;
}

int Slider::getSliderBarWidth()
{
	return  width / max;
}


