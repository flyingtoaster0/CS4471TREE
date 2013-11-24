#include "SliderRandomPushButton.h"
#include <iostream>
#include <random>
#include <time.h>
SliderRandomPushButton::SliderRandomPushButton() : PushButton()
{

}

SliderRandomPushButton::SliderRandomPushButton(float x, float y, float width, float height, std::string label, Slider *slider) : PushButton(x, y, width, height, label)
{
	this->slider = slider;
	srand(time(NULL));
}

void SliderRandomPushButton::Execute()
{
	printf("In Execute");
	this->slider->setValue(rand() % (this->slider->getMaxValue() + 1));
	printf("%d", this->slider->getValue());
}

