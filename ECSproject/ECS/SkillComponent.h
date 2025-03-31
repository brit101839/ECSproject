#pragma once

#include "ECS.h"
#include "LocalEventComponent.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "../Skill/Skill.h"

class SkillCompnent : public Component {
private:
	bool _usingSkill;

	std::string _skillname;
	std::unique_ptr<Skill> _skill;
	LocalEventComponent* _localEvent;
	std::shared_ptr<SpawnSystem> _spawnSys;

public:
	SkillCompnent(std::string skillName, std::shared_ptr<SpawnSystem> spawnSys)
		: _skillname(skillName), _spawnSys(spawnSys){
	}

	void init() override {
		if (entity->hasComponent<LocalEventComponent>()) {
			_localEvent = &entity->getComponent<LocalEventComponent>();
		}
		else {
			std::cerr << "not init local event component" << std::endl;
		}
		auto& trans = entity->getComponent<TransformComponent>();
		auto& sprite = entity->getComponent<SpriteComponent>();
		setSkill(SkillFactory::CreateSkill(_skillname, &_localEvent->getEventSystem(), _spawnSys, &trans, &sprite));

		_localEvent->subscribe<SkillStepEvent>([this](Event& event) {
			triggerSkill(event); });
	}

	void setSkill(std::unique_ptr<Skill> newSkill) {
		_skill = std::move(newSkill);
	}
		
	void UseSkill() {
		if (_skill) {
			SkillStepEvent event(SkillStep::startSkill);
			_localEvent->publish<SkillStepEvent>(event);
			// _skill.get()->execute();
		}
	}

	void triggerSkill(Event& event) {
		auto& skillEvent = static_cast<SkillStepEvent&>(event);
		if (skillEvent.step == SkillStep::SkillTrigger) {
			if (_skill) {
				_skill.get()->execute();
			}
		}
	}
};
