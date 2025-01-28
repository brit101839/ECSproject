#pragma once
#include "ECS/ECS.h"
#include <vector>
class UIManager
{
private:

	std::vector<Entity*> _uiEntities;
	Manager& _manager;

public:

	explicit UIManager(Manager& m)
		:_manager(m){ }

	void init();

	Entity& buildUI(Vector2D pos, GLfloat w, GLfloat h, std::string path);

	void render(Shader& shader, const Vector2D& cameraPos) {
		for (auto* UIentity : _uiEntities) {
			if (UIentity->isActive()) {
				UIentity->draw(shader, cameraPos);
			}
		}
	}
};

