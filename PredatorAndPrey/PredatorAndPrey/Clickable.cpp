#include "Clickable.h"

Clickable::Clickable()
{
	
	this->x = 0;
	this->y = 0;
	this->width = 0;
	this->height = 0;
}

Clickable::Clickable(float x, float y, float width, float height, std::string label)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->label = label;
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

std::string Clickable::getLabel() {return this->label;}