//
// Animation.h

#pragma once

struct Animation
{
	int tileY;
	int frames;
	int speed;
	bool flip;
	bool canInterrupt;

	Animation() {};
	Animation(int ntileY , int nframe, int nspeed, bool nflip, bool nCanInterrupt = true)
		:tileY(ntileY), frames(nframe), speed(nspeed), flip(nflip), canInterrupt(nCanInterrupt)
	{

	}
};