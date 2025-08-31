//
// Animation.h

#pragma once

enum class AnimateState
{
	Idle, Attacking, FinalAttack, usingSkill, Dodging, Dying, Died
};

struct AttackDetail
{
	int attackFrame;
	int damage;
	AttackDetail(int attackFrame, int damage)
		:attackFrame(attackFrame), damage(damage){ }
};

struct TAnimation
{
	std::string name;
	int tileY;
	int frames;
	int speed;
	bool flip;
	bool canInterrupt;
	AnimateState state;
	std::shared_ptr<AttackDetail> atcDetail;

	TAnimation() {};
	TAnimation(std::string name, int ntileY , int nframe, int nspeed, bool nflip, bool nCanInterrupt = true, AnimateState nState = AnimateState::Idle)
		:name(name), tileY(ntileY), frames(nframe), speed(nspeed), flip(nflip), canInterrupt(nCanInterrupt), state(nState)
	{

	}

	TAnimation(std::string name, int ntileY, int nframe, int nspeed, bool nflip, std::shared_ptr<AttackDetail> atcDetail, AnimateState nState = AnimateState::Attacking)
		:name(name), tileY(ntileY), frames(nframe), speed(nspeed), flip(nflip),
		canInterrupt(false), atcDetail(atcDetail), state(nState)
	{

	}
};