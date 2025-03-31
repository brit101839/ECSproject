#pragma once
#include "../EventSystem.h"
#include "SpawnSystem.h"
#include "../ECS/TransformComponent.h"
#include "../ECS/SpriteComponent.h"

class Skill {
private:
	EventSystem* _localEventSys;
	std::shared_ptr<SpawnSystem> _spawnSystem;
	TransformComponent* _trans;
	SpriteComponent* _sprite;

public:
	Skill(EventSystem* localEventSys, std::shared_ptr<SpawnSystem> spawnSys, TransformComponent* trans, SpriteComponent* sprite)
		: _localEventSys(localEventSys), _spawnSystem(spawnSys), _trans(trans), _sprite(sprite){}

	virtual void execute() = 0;
	virtual ~Skill() = default;

	void spawner(std::string name) {
		Vector2D createPos, createVelocity;
		if (_sprite->getFlip()) createPos = _trans->position - Vector2D(50.f, 0.f);
			
		else createPos = _trans->position;
		_spawnSystem.get()->createItem(name, createPos, _sprite->getFlip());
	}
};


// confused fight
class NormalAttack : public Skill {
public:
	/*NormalAttack(EventSystem* localEventSys, std::shared_ptr<SpawnSystem> spawnSys, TransformComponent* trans, SpriteComponent* sprite)
		: Skill(localEventSys, spawnSys, trans, sprite) {}*/
	using Skill::Skill;

	void execute() override {
		std::cout << "Enemy uses normal attack!" << std::endl;
	}
};

class FireBallSkill : public Skill {
public:
	/*FireBallSkill(EventSystem* localEventSys, std::shared_ptr<SpawnSystem> spawnSys, TransformComponent* trans, SpriteComponent* sprite)
		: Skill(localEventSys, spawnSys, trans, sprite) {
	}*/
	using Skill::Skill;

	void execute() override{
		spawner("fire ball");
		std::cout << "Enemy summon fire ball!" << std::endl;
	}
};

class IceBallSkill : public Skill {
	using Skill::Skill;

	void execute() override {
		spawner("ice ball");
		std::cout << "Enemy summon ice ball!" << std::endl;
	}
};

// ���{����
class RangedAttack : public Skill {
public:
	void execute() override {
		std::cout << "Enemy uses ranged attack!" << std::endl;
	}
};

// �]�k����
class MagicAttack : public Skill {
public:
	void execute() override {
		std::cout << "Enemy casts a magic spell!" << std::endl;
	}
};

class SkillFactory {

public:
	static std::unique_ptr<Skill> CreateSkill(const std::string& skillName, EventSystem* localEventSys, std::shared_ptr<SpawnSystem> spawnSys, TransformComponent* trans, SpriteComponent* sprite) {
		if (skillName == "fire ball") {
			return std::make_unique<FireBallSkill>(localEventSys, spawnSys, trans, sprite);
		}
		else if (skillName == "ice ball") {
			return std::make_unique<IceBallSkill>(localEventSys, spawnSys, trans, sprite);
		}
		return std::make_unique<NormalAttack>(localEventSys, spawnSys, trans, sprite);
	}
};