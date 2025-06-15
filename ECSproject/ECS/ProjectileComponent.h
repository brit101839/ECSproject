#pragma once

#include "ECS.h"
#include "LocalEventComponent.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"

class ProjectileComponent : public Component {
private:
	int _damage;
	LocalEventComponent* _localEvent;
	SpriteComponent* _sprite;

	void destroyEntity(Event& event){
		_localEvent->destroyEntity();
	}

public:
	ProjectileComponent(int damage)
		: _damage(damage) {

	}

	~ProjectileComponent() override {}

	void init() override {
		_sprite = &entity->getComponent<SpriteComponent>();

		if (entity->hasComponent<LocalEventComponent>()) {
			_localEvent = &entity->getComponent<LocalEventComponent>();
		}
		else {
			std::cerr << "ProjectileComponent can't access LocalEventComponent!" << std::endl;
		}

		_localEvent->subscribe<ProjectileExplosionComplete>([this](Event& event) {
			destroyEntity(event); });
	}

	void explosion() {
		_sprite->setAnimate("explosion");
		entity->getComponent<TransformComponent>().speed = 0.0f;
	}
};