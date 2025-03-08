#pragma once
#include "ECS/ECS.h"
#include "player.h"
#include <vector>
class UIManager
{
private:

	std::vector<Entity*> _uiEntities;
	Entity* _bloodBar;
	Manager& _manager;
	Player& _player;

public:

	explicit UIManager(Manager& m, Player& p)
		:_manager(m), _player(p)
	{
		_bloodBar = &_manager.addEntity();
	}

	void init();

	Entity& buildUI(Vector2D pos, GLfloat w, GLfloat h, std::string path);

	void buildUI(Entity& entity, Vector2D pos, GLfloat w, GLfloat h, std::string path);

	void render(Shader& shader, const Vector2D& cameraPos) {
		for (auto* UIentity : _uiEntities) {
			if (UIentity->isActive()) {
				UIentity->draw(shader, cameraPos);
			}
		}
	}

	void update();
};

