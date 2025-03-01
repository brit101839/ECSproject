#pragma once
#include <iostream>

// ��������
class Skill {
public:
	virtual void execute() = 0;
	virtual ~Skill() = default;
};

// ��ԧ���
class MeleeAttack : public Skill {
public:
	void execute() override {
		std::cout << "Enemy uses melee attack!" << std::endl;
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
