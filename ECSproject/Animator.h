#pragma once

#include "ECS/Animation.h"
#include <unordered_map>
#include <string>
#include <functional>
#include "EventSystem.h"

class Sprite;

class Animator {
private:

	struct TAnimateBuffer 
	{
		std::string name;
		Animation* anim;
		bool isEmpty = true;
	};

	std::unordered_map<std::string, Animation> animations;
	Animation* _currentAnimation = nullptr;
	double _lastFrameTime = 0.f;
	int _currentFrame = 0;
	std::string _nextAnimate = "idle";
	bool _flip = false;
	EventSystem* _localEvent;

public:

	Animator(EventSystem* eventManager);

	void addAnimation(const std::string& name, const Animation& anim);

	const Animation& getAnimation(const std::string& name) const;

	void setAnimation(const std::string& name);

	void onAnimateEnd();

	void onAnimateStart();

	void startSkillEvent(Event& event);

	void endSkillEvent();

	void startAttackEvent(Event& event);

	void endAttackEvent();

	void endAttackAndRest();

	void startDodgeEvent(Event& event);

	void endDodgeEvent();

	bool getFrameInterrupt() { return _currentAnimation->canInterrupt; }
	AnimateState getAnimateState() { return _currentAnimation->state; }

	void update(Sprite* sprite);
};
