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
	Vector2D _posOffset;
	Sprite* _sprite;
	bool _UI;

public:

	float mDroppingBlood = 0.f;

	HealthBarComponent(Vector2D size = Vector2D(300.f, 60.f), Vector2D offset = Vector2D(0.f,100.f), bool nUI = false)
		:_barSize(size), _posOffset(offset), _UI(nUI)
	{
	}

	void init() override {
		_trans = &entity->getComponent<TransformComponent>();
		_stats = &entity->getComponent<StatsComponent>();

		auto eSprite = &entity->getComponent<SpriteComponent>();
		// if(eSprite->isanimated()){ _barSize.x = _trans->width; }

		TextureManager& textureManager = TextureManager::getInstance();
		_sprite = new Sprite(textureManager.genWhiteTexture(), _barSize.x, _barSize.y, Origin::TopLeft);

		if(!_UI) _posOffset.x = -_trans->width / 2;
	}

	void update(GLFWwindow* window, double deltaTime) override {
		mDroppingBlood = ((mDroppingBlood - 0.05f * deltaTime) > 0) ? mDroppingBlood - 0.05f * deltaTime : 0.f;
	}

	void draw(Shader& shader, Vector2D cameraPos) override {
		if (_trans) {
			float healthPercentage = _stats->mhealthPercent;
			// float healthPercentage = float(_stats->getHealth()) / _stats->getMaxHealth();
			Vector2D position;
			if (_UI) position = cameraPos + _posOffset;
			else position = _trans->position + _posOffset; // 血量條顯示在角色上方
			
			// background render
			_sprite->renderRectangle(position, Vector2D(1.0, 1.0), shader, cameraPos, glm::vec3(0.4f, 0.4f, 0.4f));

			// foreground render
			_sprite->renderRectangle(position, Vector2D(healthPercentage, 1.0), shader, cameraPos, glm::vec3(0.8f, 0.2f, 0.2f));
			position += Vector2D(_barSize.x * healthPercentage, 0.0f);
			_sprite->renderRectangle(position, Vector2D(mDroppingBlood, 1.0), shader, cameraPos, glm::vec3(0.8f, 0.8f, 0.8f));
			// std::cout << healthPercentage << std::endl;
		}
	}
};