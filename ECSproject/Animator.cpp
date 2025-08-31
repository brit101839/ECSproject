#include <memory>
#include "Sprite.h"
#include "Animator.h"

Animator::Animator(EventSystem* eventManager) : _localEvent(eventManager)
{
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

void Animator::addAnimation(const std::string& name, const TAnimation& anim) {
	m_animations[name] = anim;
}

const std::shared_ptr<TAnimation> Animator::getAnimation(const std::string & name) {
	if (m_animations.find(name) == m_animations.end()) {
		std::cerr << "animation not found: " << name << std::endl;
		return std::make_shared<TAnimation>(m_animations.at("idle"));
	}

	TAnimation* newAnimation = &m_animations.at(name);

	return std::make_shared<TAnimation>(*newAnimation);
}

void Animator::setAnimation(const std::string& name) {
	if (name == "dying") {
		pushNextAnimate("died", true);
		if (_currentAnimation->state != AnimateState::Dying && _currentAnimation->state != AnimateState::Died) {
			_currentAnimation = getAnimation(name);
		}
		return;
	}

	if (_isNextAnimateBufferFull == true) {
		return;
	}

	// playing cannot interrupt animate
	if (_currentAnimation && _currentAnimation->canInterrupt == false) {
		if (name == "attack_1") {
			// std::cout << "trigger attack" << std::endl;
			if (_currentAnimation->name == "attack_1") {
				if (m_animations.find("attack_2") == m_animations.end()) pushNextAnimate("idle", false);
				else pushNextAnimate("attack_2", true);
			}
			else if (_currentAnimation->name == "attack_2") {
				if (m_animations.find("attack_3") == m_animations.end()) pushNextAnimate("idle", false);
				else pushNextAnimate("attack_3", true);
			}
			else if (_currentAnimation->name == "attack_3") {
				if (m_animations.find("attack_4") == m_animations.end()) pushNextAnimate("idle", false);
				else pushNextAnimate("attack_4", true);
			}
			else {
				pushNextAnimate("idle", false);
			}
		}
		return;
	}

	_currentAnimation = getAnimation(name);
	if (name == "walkL" || name == "walkR" || name == "idle") { _flip = _currentAnimation->flip; }
	onAnimateStart();
}

void Animator::onAnimateEnd() {
	if (_currentAnimation->state == AnimateState::Dodging) {
		endDodgeEvent();
	}
	else if (_currentAnimation->state == AnimateState::Attacking) {
		if (_nextAnimate->state != AnimateState::Attacking && _nextAnimate->state != AnimateState::FinalAttack) {
			std::cerr << "trigger end attack" << std::endl;
			endAttackEvent();
		}
	}
	else if (_currentAnimation->state == AnimateState::FinalAttack) {
		endAttackAndRest();
	}
	else if (_currentAnimation->state == AnimateState::usingSkill)
	{
		if (_nextAnimate->state != AnimateState::usingSkill) {
			endSkillEvent();
		}
	}
	else if (_currentAnimation->state == AnimateState::Dying) {
		ProjectileExplosionComplete event(10);
		_localEvent->publish<ProjectileExplosionComplete&>(event);
	}
}

void Animator::onAnimateStart() {

	if (_currentAnimation->canInterrupt == false) {
		_currentFrame = 0;
	}

	pushNextAnimate("idle", false);
	/*AttackStepEvent event("normal attack");*/
	switch (_currentAnimation->state)
	{
	case AnimateState::Attacking:
		// if (attackingCheck) { attackingCheck(); }
		/*_componentEventManager->publish<AttackStepEvent&>(event);*/
		break;
	case AnimateState::Dying:
		pushNextAnimate("died", true);
		break;
	case AnimateState::Died:
		pushNextAnimate("died", true);
		break;
	default:
		break;
	}
	return;
}

void Animator::startSkillEvent(Event& event) {
	auto& skillEvent = static_cast<SkillStepEvent&>(event);
	if (skillEvent.step == SkillStep::startSkill) {
		setAnimation("useSkill");
	}
}

void Animator::endSkillEvent() {
	SkillStepEvent event(SkillStep::endSkill);
	_localEvent->publish<SkillStepEvent>(event);
}

void Animator::startAttackEvent(Event& event) {
	auto& animateEvent = static_cast<AttackStepEvent&>(event);
	// Handle the attack event
	if (animateEvent.attackStep == AttackStep::startAttack) {
		// std::cout << "attack event: " << int(animateEvent.attackStep) << std::endl;

		if (_isNextAnimateBufferFull == false) {
			// set new buffer only when buffer is empty
			setAnimation("attack_1");
		}
	}
}

void Animator::endAttackEvent() {
	AttackStepEvent event(AttackStep::endAttack);
	_localEvent->publish<AttackStepEvent>(event);
}

void Animator::endAttackAndRest() {
	AttackStepEvent event(AttackStep::endAttackAndRest);
	_localEvent->publish<AttackStepEvent>(event);
}

void Animator::startDodgeEvent(Event& event) {
	auto& dodgeStep = static_cast<DodgeStepEvent&>(event);
	if (dodgeStep.dodgeStep == DodgeStep::startDodge) {
		setAnimation("dodge");
	}
}

void Animator::endDodgeEvent() {
	DodgeStepEvent event(DodgeStep::endDodge);
	_localEvent->publish<DodgeStepEvent>(event);
}

void Animator::update(Sprite* sprite) {
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
				_currentAnimation = popNextAnimate();
				onAnimateStart();
			}
			_currentFrame = (_currentFrame + 1) % totalFrames; // 循环播放
			sprite->updateAnimateVertex(_currentFrame, _currentAnimation->tileY, framesPerRow);
			_lastFrameTime = currentTime;

			if (_currentAnimation->state == AnimateState::Attacking) {
				if (_currentFrame == _currentAnimation->atcDetail->attackFrame) {
					AttackDamageEvent event("normal attack", _currentAnimation->atcDetail->damage);
					_localEvent->publish<AttackDamageEvent&>(event);
					// std::cout << "current/ attack frame: (" << _currentFrame << "/ " << _currentAnimation->atcDetail->attackFrame << ")" << std::endl;
				}
			}
			else if (_currentAnimation->state == AnimateState::usingSkill) {
				if (_currentFrame == _currentAnimation->atcDetail->attackFrame) {
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

void Animator::pushNextAnimate(const std::string& name, bool isFull)
{
	if (getAnimation(name) == nullptr) {
		return;
	}

	_isNextAnimateBufferFull = isFull;
	_nextAnimate = getAnimation(name);
}

std::shared_ptr<TAnimation> Animator::popNextAnimate(void)
{
	std::shared_ptr<TAnimation> temp = _nextAnimate;
	_nextAnimate = getAnimation("idle");

	_isNextAnimateBufferFull = false;

	return temp;
}
