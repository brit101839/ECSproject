#pragma once
#include "../EventSystem.h"
#include "SpawnSystem.h"

class Skill {
private:
	EventSystem* _localEventSys;
	std::shared_ptr<SpawnSystem> _spawnSystem;

public:
	Skill(EventSystem* localEventSys, std::shared_ptr<SpawnSystem> spawnSys)
		: _localEventSys(localEventSys), _spawnSystem(spawnSys){}

	virtual void execute() = 0;
	virtual ~Skill() = default;

	void spawner(std::string name) {
		_spawnSystem.get()->createItem(name);
	}
};


// confused fight
class NormalAttack : public Skill {
public:
	NormalAttack(EventSystem* localEventSys, std::shared_ptr<SpawnSystem> spawnSys)
		: Skill(localEventSys, spawnSys) {}

	void execute() override {
		std::cout << "Enemy uses normal attack!" << std::endl;
	}
};

class FireBallSkill : public Skill {
public:
	FireBallSkill(EventSystem* localEventSys, std::shared_ptr<SpawnSystem> spawnSys)
		: Skill(localEventSys, spawnSys){}

	void execute() override{
		spawner("fire ball");
		std::cout << "Enemy summon fire ball!" << std::endl;
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

class SkillFactory {

public:
	static std::unique_ptr<Skill> CreateSkill(const std::string& skillName, EventSystem* localEventSys, std::shared_ptr<SpawnSystem> spawnSys) {
		if (skillName == "fire ball") {
			return std::make_unique<FireBallSkill>(localEventSys, spawnSys);
		}
		return std::make_unique<NormalAttack>(localEventSys, spawnSys);
	}
};