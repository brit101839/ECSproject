#pragma once

#include "ECS.h"
#include "LocalEventComponent.h"
#include "../Skill/Skill.h"

class SkillCompnent : public Component {
private:
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
		setSkill(SkillFactory::CreateSkill(_skillname, &_localEvent->getEventSystem(), _spawnSys));
	}

	void setSkill(std::unique_ptr<Skill> newSkill) {
		_skill = std::move(newSkill);
	}
		
	void UseSkill() {
		if (_skill) {
			_skill.get()->execute();
		}
	}

};
