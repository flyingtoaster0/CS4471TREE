#include "Clickable.h"

Clickable::Clickable()
{
	
	this->x = 0;
	this->y = 0;
	this->width = 0;
	this->height = 0;
}

Clickable::Clickable(float x, float y, float height, float width)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

bool Clickable::overlap(float x, float y)
{
	if((x > this->x) && (x < this->x + this->width) && (y > this->y) && (y < this->y + this->height))
		return true;
	return false;
}



float Clickable::getX() {return this->x;}
float Clickable::getY() {return this->y;}

float Clickable::getWidth() {return this->width;}
float Clickable::getHeight() {return this->height;}