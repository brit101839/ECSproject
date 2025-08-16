#pragma once

#include "ECS/Animation.h"
#include <unordered_map>
#include <string>
#include <functional>
#include "EventSystem.h"

class AnimationSet {
private:

	std::unordered_map<std::string, Animation> animations;
	Animation* _currentAnimation = nullptr;
	double _lastFrameTime = 0.f;
	int _currentFrame = 0;
	std::string _nextAnimate = "idle";
	bool _flip = false;
	EventSystem* _localEvent;

public:

	AnimationSet(EventSystem* eventManager) : _localEvent(eventManager) {
		if (_localEvent) {
			_localEvent->subscribe<AttackStepEvent>([this](Event& event) {
				startAttackEvent(event); });
			_localEvent->subscribe<DodgeStepEvent>([this](Event& event) {
				startDodgeEvent(event); });
			_localEvent->subscribe<SkillStepEvent>([this](Event& event) {
				startSkillEvent(event); });
		}
		else {
			std::cerr << "event manager not inject successfully" << std::endl;
		}
	}

	void addAnimation(const std::string& name, const Animation& anim) {
		animations[name] = anim;
		// animations[name] = std::move(anim);
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
					if (animations.find("attack_2") == animations.end()) _nextAnimate = "idle";
					else _nextAnimate = "attack_2";
				}
				else if (_currentAnimation == &animations.at("attack_2")) {
					if(animations.find("attack_3") == animations.end()) _nextAnimate = "idle";
					else _nextAnimate = "attack_3";
				}
				else if (_currentAnimation == &animations.at("attack_3")) {
					if (animations.find("attack_4") == animations.end()) _nextAnimate = "idle";
					else _nextAnimate = "attack_4";
				}
				else {
					_nextAnimate = "idle";
				}
			}
			return;
		}
		// playing can interrupt animate
		if (animations.find(name) != animations.end()) {
			_currentAnimation = &animations[name];
			OnAnimateStart();
			if (name == "walkL" || name == "walkR" || name == "idle") { _flip = _currentAnimation->flip; }
			if (!_currentAnimation->canInterrupt) {
				_currentFrame = 0;
			}
		}
	}

	void onAnimateEnd() {
		if (_currentAnimation->state == AnimateState::Dodging) {
			endDodgeEvent();
		}
		else if (_currentAnimation->state == AnimateState::Attacking) {
			if (animations.at(_nextAnimate).state != AnimateState::Attacking && animations.at(_nextAnimate).state != AnimateState::FinalAttack) {
				std::cerr << "trigger end attack" << std::endl;
				endAttackEvent();
			}
		}
		else if (_currentAnimation->state == AnimateState::FinalAttack) {
			endAttackAndRest();
		}
		else if (_currentAnimation->state == AnimateState::usingSkill)
		{
			if (animations.at(_nextAnimate).state != AnimateState::usingSkill) {
				endSkillEvent();
			}
		}
		else if(_currentAnimation->state == AnimateState::Dying) {
			ProjectileExplosionComplete event(10);
			_localEvent->publish<ProjectileExplosionComplete&>(event);
		}
	}

	void OnAnimateStart() {
		_nextAnimate = "idle";
		/*AttackStepEvent event("normal attack");*/
		switch (_currentAnimation->state)
		{
		case AnimateState::Attacking:
			// if (attackingCheck) { attackingCheck(); }
			/*_componentEventManager->publish<AttackStepEvent&>(event);*/
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

	void startSkillEvent(Event& event) {
		auto& skillEvent = static_cast<SkillStepEvent&>(event);
		if (skillEvent.step == SkillStep::startSkill) {
			setAnimation("useSkill");
		}
	}

	void endSkillEvent() {
		SkillStepEvent event(SkillStep::endSkill);
		_localEvent->publish<SkillStepEvent>(event);
	}

	void startAttackEvent(Event& event) {
		auto& animateEvent = static_cast<AttackStepEvent&>(event);
		// Handle the attack event
		if (animateEvent.attackStep == AttackStep::startAttack) {
			// std::cout << "attack event: " << animateEvent.attackStep << std::endl;
			setAnimation("attack_1");
		}
	}

	void endAttackEvent() {
		AttackStepEvent event(AttackStep::endAttack);
		_localEvent->publish<AttackStepEvent>(event);
	}

	void endAttackAndRest() {
		AttackStepEvent event(AttackStep::endAttackAndRest);
		_localEvent->publish<AttackStepEvent>(event);
	}

	void startDodgeEvent(Event& event) {
		auto& dodgeStep = static_cast<DodgeStepEvent&>(event);
		if (dodgeStep.dodgeStep == DodgeStep::startDodge) {
			setAnimation("dodge");
		}
	}

	void endDodgeEvent() {
		DodgeStepEvent event(DodgeStep::endDodge);
		_localEvent->publish<DodgeStepEvent>(event);
	}

	bool getFrameInterrupt() { return _currentAnimation->canInterrupt; }
	AnimateState getAnimateState() { return _currentAnimation->state; }

	void update(Sprite* sprite) {
		if (_currentAnimation) {
			float frameInterval = 1.0f / _currentAnimation->speed; // 每帧持续时间
			int totalFrames = _currentAnimation->frames; // 动画总帧数
			int framesPerRow = _currentAnimation->frames; // 每行的帧数

			double currentTime = glfwGetTime();

			if (_currentAnimation->state == AnimateState::Died) {
				sprite->updateAnimateVertex(totalFrames - 1, _currentAnimation->tileY, framesPerRow);
				_lastFrameTime = currentTime;
				return;
			}
			// std::cout << _currentAnimation->speed << "/" << _currentFrame << "/" << currentTime - _lastFrameTime << "/" << currentTime << std::endl;
			if (currentTime - _lastFrameTime > frameInterval) {
				if (!_currentAnimation->canInterrupt && _currentFrame + 1 >= totalFrames) {
					onAnimateEnd();
					_currentAnimation = &animations[_nextAnimate];
					OnAnimateStart();
				}
				_currentFrame = (_currentFrame + 1) % totalFrames; // 循环播放
				sprite->updateAnimateVertex(_currentFrame, _currentAnimation->tileY, framesPerRow);
				_lastFrameTime = currentTime;

				if (_currentAnimation->state == AnimateState::Attacking) {
					if (_currentFrame == _currentAnimation->atcDetail.get()->attackFrame) {
						AttackDamageEvent event("normal attack", _currentAnimation->atcDetail.get()->damage);
						_localEvent->publish<AttackDamageEvent&>(event);
						// std::cout << "current/ attack frame: (" << _currentFrame << "/ " << _currentAnimation->atcDetail->attackFrame << ")" << std::endl;
					}
				}
				else if (_currentAnimation->state == AnimateState::usingSkill) {
					if (_currentFrame == _currentAnimation->atcDetail.get()->attackFrame) {
						SkillStepEvent event(SkillStep::SkillTrigger);
						_localEvent->publish<SkillStepEvent&>(event);
						// std::cout << "current/ attack frame: (" << _currentFrame << "/ " << _currentAnimation->atcDetail->attackFrame << ")" << std::endl;
					}
				}
			}
			// sprite->setFlip(_currentAnimation->flip);
			sprite->setFlip(_flip);
		}
	}
};
