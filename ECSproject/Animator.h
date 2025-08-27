#pragma once

#include "ECS/Animation.h"
#include <unordered_map>
#include <string>
#include <functional>
#include "EventSystem.h"

class Sprite;

class Animator {
public:

	Animator(EventSystem* eventManager);

	void addAnimation(const std::string& name, const Animation& anim);

	const Animation* getAnimation(const std::string& name);

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

	bool getFrameInterrupt() { return _currentAnimation->anim->canInterrupt; }
	AnimateState getAnimateState() { return _currentAnimation->anim->state; }

	void update(Sprite* sprite);

private:

	struct TAnimateBuffer
	{
		std::string name;
		const Animation* anim = nullptr;
		bool isFull = false;

		TAnimateBuffer(std::string name, const Animation* anim, bool isFull);
	};

	std::unique_ptr<TAnimateBuffer> makeAnimateBuffer(std::string name, bool isFull);

	void pushNextAnimate(const std::string& name, bool isFull);

	std::unique_ptr<TAnimateBuffer> popNextAnimate( void );

	std::unordered_map<std::string, Animation> m_animations;
	std::unique_ptr<TAnimateBuffer> _currentAnimation;
	double _lastFrameTime = 0.f;
	int _currentFrame = 0;
	std::unique_ptr<TAnimateBuffer> _nextAnimate;
	bool _flip = false;
	EventSystem* _localEvent;
};
