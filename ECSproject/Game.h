//
// Game.h

#ifndef __Game__
#define __Game__

//#include "glad/glad.h"
//#include <GLFW/glfw3.h>
#include <iostream>

#include "textureManager.h"
#include "GameObject.h"
#include "shader/Shader.h"
#include "RenderQuadtreeManager.h"

class ColliderComponent;
class Map;

enum GroupLabels : std::size_t
{
	groupMap,
	groupCollider,
	groupPlayer,
	groupEnemies,
	groupColliders,
	groupCamera
};

//struct GetEntityBox {
//	quadtree::Box<float> operator()(const Entity& entity) const {
//		return entity.getComponent<ColliderComponent>().box;
//	}
//};

class Game
{
private:

	GLFWwindow* _window;

	Shader _shader;

	bool initFlow(const char* title, bool fullscreen);
	void setupGL();

	void handleInitFailure();
	void cleanup();

	static void mouseButtonCallbackDispatcher(GLFWwindow* window, int button, int action, int mods);

	// GameObject* _gameObject;
	Map* _map;

	RenderQuadtreeManager* _renderManager;

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
	void update();

	static std::vector<ColliderComponent*> colliders;

	unsigned int VBO, VAO, EBO;
	unsigned int texture;
};

#endif // !__Game__
