//
// Animation.h

#pragma once

struct Animation
{
	int tileY;
	int frames;
	int speed;
	bool flip;

	Animation() {};
	Animation(int ntileY , int nframe, int nspeed, bool nfilp)
	{
		tileY = ntileY;
		frames = nframe;
		speed = nspeed;
		flip = nfilp;
	}

};