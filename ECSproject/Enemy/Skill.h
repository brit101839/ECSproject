#pragma once
#include <iostream>

// 策略基類
class Skill {
public:
	virtual void execute() = 0;
	virtual ~Skill() = default;
};

// 近戰攻擊
class MeleeAttack : public Skill {
public:
	void execute() override {
		std::cout << "Enemy uses melee attack!" << std::endl;
	}
};

// 遠程攻擊
class RangedAttack : public Skill {
public:
	void execute() override {
		std::cout << "Enemy uses ranged attack!" << std::endl;
	}
};

// 魔法攻擊
class MagicAttack : public Skill {
public:
	void execute() override {
		std::cout << "Enemy casts a magic spell!" << std::endl;
	}
};
