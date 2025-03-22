#pragma once

#include"ECS/ECS.h"

class BackPack {
private:
	Entity* _entity;
	bool _enable = false;
	SpriteComponent* _sprite;

public:
	BackPack(Entity* entity) :_entity(entity) {
		entity->addcomponent<TransformComponent>(Vector2D(200.0f, 700.0f), 500.0f, 500.0f);
		entity->addcomponent<SpriteComponent>("D:\\dependencies\\resource\\UI\\Fantasy Minimal Pixel Art GUI by eta-commercial-free\\UI\\RectangleBox_96x96.png", SpriteType::UI);
		_sprite = &_entity->getComponent<SpriteComponent>();
	}

	void disable() {
		_enable = false;
	}

	void update() {
		
	}

	void render(Shader& shader, Vector2D cameraPos) {
		if (_enable) {
			_sprite->draw(shader, cameraPos);
		}
	}
};