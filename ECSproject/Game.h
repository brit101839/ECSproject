//
// Game.h

#ifndef __Game__
#define __Game__

//#include "glad/glad.h"
//#include <GLFW/glfw3.h>
#include <iostream>

#include "textureManager.h"
#include "GameObject.h"
#include "Map.h"
#include "shader/Shader.h"

class ColliderComponent;

enum GroupLabels : std::size_t
{
	groupMap,
	groupPlayer,
	groupEnemies,
	groupColliders,
	groupCamera
};

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

	// Sprite* _testsprite;
	// GameObject* _gameObject;
	Map* _map;

public:

	static bool _isRunning;

	Game();
	~Game();

	bool getRunning();

	Shader& getShader();
	
	void handleEvents();
	void keyCallback(GLFWwindow* window, int button, int action);

	static void addTile(int id, GLfloat tileSize, Vector2D position, bool collider);

	void render();
	void update();

	static std::vector<ColliderComponent*> colliders;

	unsigned int VBO, VAO, EBO;
	unsigned int texture;
};

#endif // !__Game__
