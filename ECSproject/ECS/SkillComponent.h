#pragma once

#include "ECS.h"

class Skill {
public:
	virtual void execute() = 0;
	virtual ~Skill() = default;
};

class SkillCompnent : public Component {
private:
	std::unique_ptr<Skill> _skill;

public:
	SkillCompnent(std::unique_ptr<Skill> initialSkill)
		:_skill(std::move(initialSkill)) { }

	void setSkill(std::unique_ptr<Skill> newSkill) {
		_skill = std::move(newSkill);
	}
		
	void UseSkill() {
		if (_skill) {
			_skill.get()->execute();
		}
	}

};


// confused fight
class MeleeAttack : public Skill {
public:
	void execute() override {
		std::cout << "Enemy uses melee attack!" << std::endl;
	}
};

// »·µ{§ðÀ»
class RangedAttack : public Skill {
public:
	void execute() override {
		std::cout << "Enemy uses ranged attack!" << std::endl;
	}
};

// Å]ªk§ðÀ»
class MagicAttack : public Skill {
public:
	void execute() override {
		std::cout << "Enemy casts a magic spell!" << std::endl;
	}
};