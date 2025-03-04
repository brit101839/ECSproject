#pragma once

#include "ECS/Animation.h"
#include <unordered_map>
#include <string>
#include <functional>
#include "EventManager.h"

class AnimationSet {
private:

	std::unordered_map<std::string, Animation> animations;
	Animation* _currentAnimation = nullptr;
	float _lastFrameTime = 0.f;
	int _currentFrame = 0;
	std::string _nextAnimate = "idle";
	bool _flip = false;
	EventManager* _componentEventManager;

public:

	AnimationSet(EventManager* eventManager) : _componentEventManager(eventManager) {
		if (_componentEventManager) {
			_componentEventManager->subscribe<AttackStepEvent>([this](Event& event) {
				onAnimationEvent(event); });
		}
		else {
			std::cerr << "event manager not inject successfully" << std::endl;
		}
	}

	void addAnimation(const std::string& name, const Animation& anim) {
		animations[name] = anim;
	}

	const Animation& getAnimation(const std::string& name) const {
		return animations.at(name);
	}

	void setAnimation(const std::string& name) {
		if (name == "dying") {
			_nextAnimate = "died";
			if (_currentAnimation->state != AnimateState::Dying && _currentAnimation->state != AnimateState::Died) {
				if (animations.find(name) != animations.end()) {
					_currentAnimation = &animations[name];
					if (!_currentAnimation->canInterrupt) {
						_currentFrame = 0;
					}
				}
			}
			return;
		}
		// playing cannot interrupt animate
		if (_currentAnimation && !_currentAnimation->canInterrupt) {
			if (name == "attack_1") {
				if (_currentAnimation == &animations.at("attack_1")) {
					_nextAnimate = "attack_2";
				}
				else if (_currentAnimation == &animations.at("attack_2")) {
					_nextAnimate = "attack_3";
				}
				else if (_currentAnimation == &animations.at("attack_3") && animations.find("attack_4") != animations.end()) {
					_nextAnimate = "attack_4";
				}
			}
			return;
		}
		// playing can interrupt animate
		if (animations.find(name) != animations.end()) {
			_currentAnimation = &animations[name];
			animateSwitching();
			if (name == "walkL" || name == "walkR") { _flip = _currentAnimation->flip; }
			if (!_currentAnimation->canInterrupt) {
				_currentFrame = 0;
			}
		}
	}

	void animateSwitching() {
		_nextAnimate = "idle";
		AttackStepEvent event("normal attack");
		switch (_currentAnimation->state)
		{
		case AnimateState::Attacking:
			// if (attackingCheck) { attackingCheck(); }
			_componentEventManager->notify<AttackStepEvent&>(event);
			break;
		case AnimateState::Dying:
			_nextAnimate = "died";
			break;
		case AnimateState::Died:
			_nextAnimate = "died";
			break;
		default:
			break;
		}
		return;
	}

	void onAnimationEvent(Event& event) {
		auto& animateEvent = static_cast<AttackStepEvent&>(event);
		// Handle the attack event
		if (animateEvent.attackStep == "startAttack") {
			std::cout << "attack event: " << animateEvent.attackStep << std::endl;
			setAnimation("attack_1");
		}
	}

	bool getFrameInterrupt() { return _currentAnimation->canInterrupt; }
	AnimateState getAnimateState() { return _currentAnimation->state; }

	void update(Sprite* sprite) {
		if (_currentAnimation) {
			float frameInterval = 1.0f / _currentAnimation->speed; // 每帧持续时间
			int totalFrames = _currentAnimation->frames; // 动画总帧数
			int framesPerRow = _currentAnimation->frames; // 每行的帧数

			float currentTime = glfwGetTime();

			if (_currentAnimation->state == AnimateState::Died) {
				sprite->updateAnimateVertex(totalFrames - 1, _currentAnimation->tileY, framesPerRow);
				_lastFrameTime = currentTime;
				return;
			}
			// std::cout << _currentAnimation->speed << "/" << _currentFrame << "/" << currentTime - _lastFrameTime << "/" << currentTime << std::endl;
			if (currentTime - _lastFrameTime > frameInterval) {
				if (!_currentAnimation->canInterrupt && _currentFrame + 1 >= totalFrames) {
					_currentAnimation = &animations[_nextAnimate];
					animateSwitching();
				}
				_currentFrame = (_currentFrame + 1) % totalFrames; // 循环播放
				sprite->updateAnimateVertex(_currentFrame, _currentAnimation->tileY, framesPerRow);
				_lastFrameTime = currentTime;
			}
			// sprite->setFlip(_currentAnimation->flip);
			sprite->setFlip(_flip);
		}
	}
};
