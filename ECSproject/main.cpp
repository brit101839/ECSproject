#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>

#include "Game.h"

Game* game = nullptr;

int main(int argc, char** argv)
{
    game = new Game();

    const double updateRate = 60;
    const double TIME_STEP = 1.0 / updateRate;

    double currentTime = glfwGetTime();
    double lag = 0.0;

    /* Loop until the user closes the window */
    while (game->getRunning())
    {
        double newTime = glfwGetTime();
        double frameTime = newTime - currentTime;
        currentTime = newTime;

        lag += frameTime;

        while (lag >= TIME_STEP) {
            game->update(TIME_STEP);
            lag -= TIME_STEP;
        }

        /* Render here */
        game->render();

        game->handleEvents();
    }

    glfwTerminate();
    return 0;
}