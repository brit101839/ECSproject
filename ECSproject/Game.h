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

class Game
{
private:

	bool _isRunning;

	GLFWwindow* _window;

	bool initFlow(const char* title, bool fullscreen);
	void setupGL();

	void handleInitFailure();
	void cleanup();

	Sprite* _testsprite;
	GameObject* _gameObject;
	Map* _map;
	TextureManager* _textureManager;

public:

	Game();
	~Game();

	bool getRunning();

	void handleEvents();
	void render();
	void update();

};

#endif // !__Game__
