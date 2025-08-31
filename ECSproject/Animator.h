#pragma once

#include "Animation.h"
#include <unordered_map>
#include <string>
#include <functional>
#include "EventSystem.h"

class Sprite;

class Animator {
public:

	Animator(EventSystem* eventManager);

	void addAnimation(const std::string& name, const TAnimation& anim);

	const std::shared_ptr<TAnimation> getAnimation(const std::string& name);

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

private:

	void pushNextAnimate(const std::string& name, bool isFull);

	std::shared_ptr<TAnimation> popNextAnimate( void );

private:

	EventSystem* _localEvent;

	bool _isNextAnimateBufferFull = false;

	std::unordered_map<std::string, TAnimation> m_animations;

	std::shared_ptr<TAnimation> _currentAnimation;

	std::shared_ptr<TAnimation> _nextAnimate;

	double _lastFrameTime = 0.f;
	int _currentFrame = 0;
	bool _flip = false;
	
};
