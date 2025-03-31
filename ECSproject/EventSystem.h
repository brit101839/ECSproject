#pragma once

#include "ECS/ECS.h"
#include "BoundingBox.h"
#include <unordered_map>
#include <string>
#include <functional>

class Event {
public:
    virtual ~Event() = default;
};

class AttackCheckingEvent : public Event {
public:
    std::string attacker;
    BoundingBox boundingBox;
    int damage;

    AttackCheckingEvent(std::string attacker, BoundingBox boundingBox, int damage)
        : attacker(attacker), boundingBox(boundingBox), damage(damage) {
    }
};

class AttackDamageEvent : public Event {
public:
    std::string skillName;
    int damage;

    AttackDamageEvent(std::string skillName, int damage)
        : skillName(skillName), damage(damage) {

    }
};

enum class SkillStep
{
    startSkill, SkillTrigger, endSkill
};

class SkillStepEvent :public Event {
public:
    SkillStep step;

    SkillStepEvent(SkillStep step)
        : step(step) {
    }
};

enum class AttackStep
{
    startAttack, Checking, endAttack
};

class AttackStepEvent : public Event {
public:
    AttackStep attackStep;

    AttackStepEvent(AttackStep attackStep)
        : attackStep(attackStep) {
    }
};

enum class DodgeStep
{
    startDodge, startInvincibility, endInvincibility, endDodge
};

class DodgeStepEvent : public Event {
public:
    DodgeStep dodgeStep;

    DodgeStepEvent(DodgeStep dodgeStep) 
        :dodgeStep(dodgeStep) {}
};

class EventSystem {
public:
    using Callback = std::function<void(Event&)>;

    template <typename EventType>
    void subscribe(Callback callback) {
        auto& subscribers = _subscribers[typeid(EventType).hash_code()];
        subscribers.push_back(callback);
    }

    template <typename EventType>
    void publish(EventType& event) {
        auto& subscribers = _subscribers[typeid(EventType).hash_code()];
        for (auto& callback : subscribers) {
            callback(event);
        }
    }

private:
    std::unordered_map<size_t, std::vector<Callback>> _subscribers;
};
