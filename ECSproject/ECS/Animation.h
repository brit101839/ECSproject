//
// Animation.h

#pragma once

enum class AnimateState
{
	Idle, Attacking
};

struct Animation
{
	int tileY;
	int frames;
	int speed;
	bool flip;
	bool canInterrupt;
	AnimateState state;

	Animation() {};
	Animation(int ntileY , int nframe, int nspeed, bool nflip, bool nCanInterrupt = true, AnimateState nState = AnimateState::Idle)
		:tileY(ntileY), frames(nframe), speed(nspeed), flip(nflip), canInterrupt(nCanInterrupt), state(nState)
	{

	}
};