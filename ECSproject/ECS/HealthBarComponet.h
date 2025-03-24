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

	HealthBarComponent(Vector2D size = Vector2D(15.f, 3.f), Vector2D offset = Vector2D(0.f,100.f), bool nUI = false)
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

		if(!_UI) _posOffset.x = -_trans->width / 3;
	}

	void update(GLFWwindow* window) override {
		mDroppingBlood = ((mDroppingBlood - 0.0005f) > 0) ? mDroppingBlood - 0.0005f : 0.f;
	}

	void draw(Shader& shader, Vector2D cameraPos) override {
		if (_trans) {
			float healthPercentage = _stats->mhealthPercent;
			Vector2D position;
			if (_UI) position = cameraPos + _posOffset;
			else position = _trans->position + _posOffset; // ��q����ܦb����W��
			
			// ��V�I��
			_sprite->renderRectangle(position, _barSize, shader, cameraPos, glm::vec3(0.4f, 0.4f, 0.4f));

			// ��V�e��
			_sprite->renderRectangle(position, Vector2D(_barSize.x * healthPercentage, _barSize.y), shader, cameraPos, glm::vec3(0.8f, 0.2f, 0.2f));
			position += Vector2D(_barSize.x * _barSize.x * healthPercentage, 0.0f);
			_sprite->renderRectangle(position, Vector2D(_barSize.x * mDroppingBlood, _barSize.y), shader, cameraPos, glm::vec3(0.8f, 0.8f, 0.8f));
			// std::cout << healthPercentage << std::endl;
		}
	}
};