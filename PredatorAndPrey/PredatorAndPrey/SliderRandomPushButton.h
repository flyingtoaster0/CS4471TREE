#include "Slider.h"
#include "PushButton.h"
#ifndef SLIDERRANDOMPUSHBUTTON_H
#define SLIDERRANDOMPUSHBUTTON_H

class SliderRandomPushButton : public PushButton
{
	private:
		void Execute();
		Slider *slider;
	public:
		SliderRandomPushButton();
		SliderRandomPushButton(float x, float y, float width, float height, std::string label, Slider *slider);
};
#endif