#pragma once

#include "ECS.h"
#include "Components.h"
#include "../textureManager.h"

class HealthBarComponent : public Component {
private:

	TransformComponent* _trans;
	StatsComponent* _stats;
	Vector2D _barSize;
	Vector2D _pos;
	Sprite* _sprite;

public:

	float mDroppingBlood = 0.f;

	HealthBarComponent(Vector2D size = Vector2D(15.f, 3.f))
		:_barSize(size) 
	{
	}

	void init() override {
		_trans = &entity->getComponent<TransformComponent>();
		_stats = &entity->getComponent<StatsComponent>();

		auto eSprite = &entity->getComponent<SpriteComponent>();
		// if(eSprite->isanimated()){ _barSize.x = _trans->width; }

		TextureManager& textureManager = TextureManager::getInstance();
		_sprite = new Sprite(textureManager.genWhiteTexture(), _barSize.x, _barSize.y, Origin::TopLeft);
	}

	void update(GLFWwindow* window) override {
		mDroppingBlood = ((mDroppingBlood - 0.0005f) > 0) ? mDroppingBlood - 0.0005f : 0.f;
	}

	void draw(Shader& shader, Vector2D cameraPos) override {
		if (_trans) {
			float healthPercentage = _stats->mhealthPercent;
			Vector2D position = _trans->position + Vector2D(-_trans->width / 3, 100.0f); // 血量條顯示在角色上方
			

			// 渲染背景
			_sprite->renderRectangle(position, _barSize, shader, cameraPos, glm::vec3(0.4f, 0.4f, 0.4f));

			// 渲染前景
			_sprite->renderRectangle(position, Vector2D(_barSize.x * healthPercentage, _barSize.y), shader, cameraPos, glm::vec3(0.0f, 1.0f, 0.0f));
			position += Vector2D(_barSize.x * _barSize.x * healthPercentage, 0.0f);
			_sprite->renderRectangle(position, Vector2D(_barSize.x * mDroppingBlood, _barSize.y), shader, cameraPos, glm::vec3(1.0f, 0.0f, 0.0f));
			// std::cout << healthPercentage << std::endl;
		}
	}
};