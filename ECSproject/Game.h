//
// Game.h

#ifndef __Game__
#define __Game__

//#include "glad/glad.h"
//#include <GLFW/glfw3.h>
#include <iostream>

#include "textureManager.h"
#include "shader/Shader.h"
#include "RenderQuadtreeManager.h"
#include "CollisionManager.h"
#include "Enemy/EnemyManager.h"
#include "UIManager.h"
#include "EventSystem.h"
#include "Skill/SpawnSystem.h"
#include "player.h"
#include "BackPack.h"
#include "TextRender.h"

class ColliderComponent;
class Map;

//struct GetEntityBox {
//	quadtree::Box<float> operator()(const Entity& entity) const {
//		return entity.getComponent<ColliderComponent>().box;
//	}
//};

class Game
{
private:

	GLFWwindow* _window;

	Shader _spriteShader;
	Shader _textShader;

	TextRender* _textRender;

	bool initFlow(const char* title, bool fullscreen);
	void setupGL();
	bool initEntityGroup();

	void handleInitFailure();
	void cleanup();

	// static void mouseButtonCallbackDispatcher(GLFWwindow* window, int button, int action, int mods);

	// GameObject* _gameObject;
	Map* _map;

	EnemyManager* _enemyManager;

	RenderQuadtreeManager* _renderManager;
	CollisionManager* _colliderManager;
	UIManager* _UIManager;
	EventSystem _globalEventManager;
	std::shared_ptr<SpawnSystem> _spawnSystem;

	Player* player = nullptr;
	BackPack* backpack = nullptr;
	Entity* wall = nullptr;
	Entity* Camera = nullptr;

public:

	static bool _isRunning;

	Game();
	~Game();

	bool getRunning();

	Shader& getShader();
	
	void handleEvents();
	void keyCallback(GLFWwindow* window, int button, int action);

	void addTile(int id, GLfloat tileSize, Vector2D position, bool collider);

	void render();
	void update(double deltaTime);
};

#endif // !__Game__
