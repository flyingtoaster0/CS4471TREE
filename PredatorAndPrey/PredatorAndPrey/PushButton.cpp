#include "PushButton.h"

PushButton::PushButton() : Clickable()
{

}

PushButton::PushButton(float x, float y, float width, float height) : Clickable(x, y, width, height)
{
	// Hardcoded color. Change this later maybe

	this->upColor = vec3(0.5, 0.0, 0.8);
	this->downColor = vec3(1.0, 1.0, 1.0) - this->upColor;
}

void PushButton::mouseDown(float x, float y)
{
	if(overlap(x,y))
	{
		setState(DOWN);
		setColor(downColor);
	}
}

void PushButton::mouseUp(float x, float y)
{
	if(getState() == DOWN && overlap(x,y))
		int x = 1;
	setState(UP);
	setColor(upColor);
}

void PushButton::setState(int state)
{
	this->state = state;
}


int PushButton::getState()
{
	return this->state;
}

void PushButton::setColor(vec3 color)
{
	this->color = color;
}

vec3 PushButton::getColor()
{
	return this->color;
}