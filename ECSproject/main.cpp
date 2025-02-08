#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>

#include "Game.h"

Game* game = nullptr;

int main(int argc, char** argv)
{
    game = new Game();

    const double FPS = 60;
    const double TIME_STEP = 1.0 / FPS;

    double currentTime = glfwGetTime();
    double accumulator = 0.0;

    /* Loop until the user closes the window */
    while (game->getRunning())
    {
        double newTime = glfwGetTime();
        double frameTime = newTime - currentTime;
        currentTime = newTime;

        accumulator += frameTime;

        while (accumulator >= TIME_STEP) {
            game->update();
            accumulator -= frameTime;
        }

        /* Render here */
        game->render();

        game->handleEvents();
    }

    glfwTerminate();
    return 0;
}