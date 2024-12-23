#pragma once

#include "ECS/Animation.h"
#include <unordered_map>
#include <string>

class AnimationSet {
private:

	std::unordered_map<std::string, Animation> animations;
	Animation* _currentAnimation = nullptr;
	float _lastFrameTime = 0.f;
	int _currentFrame = 0;

public:

	void addAnimation(const std::string& name, const Animation& anim) {
		animations[name] = anim;
	}

	const Animation& getAnimation(const std::string& name) const {
		return animations.at(name);
	}

	void setAnimation(const std::string& name) {
		if (animations.find(name) != animations.end()) {
			_currentAnimation = &animations[name];
			// _currentFrame = 0;
			// _lastFrameTime = 0;
		}
	}

	void update(Sprite* sprite) {
		if (_currentAnimation) {
			
			float frameInterval = 1.0f / _currentAnimation->speed; // 每帧持续时间
			int totalFrames = _currentAnimation->frames; // 动画总帧数
			int framesPerRow = _currentAnimation->frames; // 每行的帧数

			float currentTime = glfwGetTime();
			// std::cout << _currentAnimation->speed << "/" << _currentFrame << "/" << currentTime - _lastFrameTime << "/" << currentTime << std::endl;
			if (currentTime - _lastFrameTime > frameInterval) {
				_currentFrame = (_currentFrame + 1) % totalFrames; // 循环播放
				sprite->updateAnimateVertex(_currentFrame, _currentAnimation->tileY, framesPerRow);
				_lastFrameTime = currentTime;
			}
			sprite->setFlip(_currentAnimation->flip);
			
		}
	}
};
