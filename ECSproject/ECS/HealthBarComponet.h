#pragma once

#include "ECS.h"
#include "Components.h"
#include "../textureManager.h"

class HealthBarComponent : public Component {
private:

	TransformComponent* _trans;
	StatsComponent* _stats;
	Vector2D _barSize;
	Sprite* _sprite;

public:

	HealthBarComponent(Vector2D size = Vector2D(15.f, 3.f))
		:_barSize(size) 
	{
	}

	void init() override {
		_trans = &entity->getComponent<TransformComponent>();
		_stats = &entity->getComponent<StatsComponent>();

		TextureManager& textureManager = TextureManager::getInstance();
		_sprite = new Sprite(textureManager.genWhiteTexture(), _barSize.x, _barSize.y);
	}

	void update(GLFWwindow* window) override {

	}

	void draw(Shader& shader, Vector2D cameraPos) override {
		if (_trans) {
			float healthPercentage = _stats->mhealthPercent;
			Vector2D position = _trans->position + Vector2D(0, 100.0f); // 血量條顯示在角色上方

			// 渲染背景
			// shader.setVec3("overlayColor", glm::vec3(0.2f, 0.2f, 0.2f)); // 灰色背景
			_sprite->renderRectangle(position, _barSize, shader, cameraPos, glm::vec3(0.4f, 0.4f, 0.4f));

			// 渲染前景
			// glm::vec2 frontBarSize = glm::vec2(_barSize.x * healthPercentage, _barSize.y);
			// shader.setVec3("overlayColor", glm::vec3(0.0f, 1.0f, 0.0f)); // 綠色前景
			_sprite->renderRectangle(position - Vector2D(_barSize.x * _barSize.x/2 * (1.0f - healthPercentage), 0.f), Vector2D(_barSize.x * healthPercentage, _barSize.y), shader, cameraPos, glm::vec3(0.0f, 1.0f, 0.0f));
			// std::cout << healthPercentage << std::endl;
		}
	}
};