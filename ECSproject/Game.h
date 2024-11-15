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

class ColliderComponent;

class Game
{
private:

	bool _isRunning;

	GLFWwindow* _window;

	bool initFlow(const char* title, bool fullscreen);
	void setupGL();

	void handleInitFailure();
	void cleanup();

	static void mouseButtonCallbackDispatcher(GLFWwindow* window, int button, int action, int mods);

	// Sprite* _testsprite;
	// GameObject* _gameObject;
	Map* _map;

public:

	Game();
	~Game();

	bool getRunning();

	void handleEvents();
	void keyCallback(GLFWwindow* window, int button, int action);

	static void addTile(int id, Vector2D position, bool collider);

	void render();
	void update();

	static std::vector<ColliderComponent*> colliders;

};

#endif // !__Game__
