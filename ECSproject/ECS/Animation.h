//
// Animation.h

#pragma once

struct Animation
{
	int tileY;
	int frames;
	int speed;

	Animation() {};
	Animation(int ntileY , int nframe, int nspeed)
	{
		tileY = ntileY;
		frames = nframe;
		speed = nspeed;
	}

};