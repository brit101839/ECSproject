﻿#pragma once

#include "ECS/Animation.h"
#include <unordered_map>
#include <string>

class AnimationSet {
private:

	std::unordered_map<std::string, Animation> animations;
	Animation* _currentAnimation = nullptr;
	float _lastFrameTime = 0.f;
	int _currentFrame = 0;
	std::string _nextAnimate = "idle";
	bool _flip = false;

public:

	void addAnimation(const std::string& name, const Animation& anim) {
		animations[name] = anim;
	}

	const Animation& getAnimation(const std::string& name) const {
		return animations.at(name);
	}

	void setAnimation(const std::string& name) {
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
		if (animations.find(name) != animations.end()) {
			_currentAnimation = &animations[name];
			if (name == "walkL" || name == "walkR") { _flip = _currentAnimation->flip; }
			if (!_currentAnimation->canInterrupt) {
				_currentFrame = 0;
			}
		}
	}

	bool getFrameInterrupt() { return _currentAnimation->canInterrupt; }

	void update(Sprite* sprite) {
		if (_currentAnimation) {
			
			float frameInterval = 1.0f / _currentAnimation->speed; // 每帧持续时间
			int totalFrames = _currentAnimation->frames; // 动画总帧数
			int framesPerRow = _currentAnimation->frames; // 每行的帧数

			float currentTime = glfwGetTime();
			// std::cout << _currentAnimation->speed << "/" << _currentFrame << "/" << currentTime - _lastFrameTime << "/" << currentTime << std::endl;
			if (currentTime - _lastFrameTime > frameInterval) {
				if (!_currentAnimation->canInterrupt && _currentFrame + 1 >= totalFrames) {
					_currentAnimation = &animations[_nextAnimate];
					_nextAnimate = "idle";
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
