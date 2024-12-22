#pragma once

#include "ECS/Animation.h"
#include <unordered_map>
#include <string>

class AnimationSet {
private:

	std::unordered_map<std::string, Animation> animations;

public:

	void addAnimation(const std::string& name, const Animation& anim) {
		animations[name] = anim;
	}

	const Animation& getAnimation(const std::string& name) const {
		return animations.at(name);
	}
};
