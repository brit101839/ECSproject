//
// Game.h

#ifndef __Game__
#define __Game__

//#include "glad/glad.h"
//#include <GLFW/glfw3.h>
#include <iostream>

#include "Sprite.h"

#define Window_w_Size 1422
#define Window_h_Size 800

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

public:

	Game();
	~Game();

	bool getRunning();

	void handleEvents();
	void render();
	void update();

};

#endif // !__Game__
