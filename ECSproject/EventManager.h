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

class AttackEvent : public Event {
public:
    std::string attacker;
    BoundingBox boundingBox;
    int damage;

    AttackEvent(std::string attacker, BoundingBox boundingBox, int damage)
        : attacker(attacker), boundingBox(boundingBox), damage(damage) {
    }
};

class AttackStepEvent : public Event {
public:
    std::string attackStep;

    AttackStepEvent(std::string attackStep)
        : attackStep(attackStep) {
    }
};

class EventManager {
public:
    using Callback = std::function<void(Event&)>;

    template <typename EventType>
    void subscribe(Callback callback) {
        auto& subscribers = _subscribers[typeid(EventType).hash_code()];
        subscribers.push_back(callback);
    }

    template <typename EventType>
    void notify(EventType& event) {
        auto& subscribers = _subscribers[typeid(EventType).hash_code()];
        for (auto& callback : subscribers) {
            callback(event);
        }
    }

private:
    std::unordered_map<size_t, std::vector<Callback>> _subscribers;
};
