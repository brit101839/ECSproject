#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

#include "Game.h"

Game* game = nullptr;

int main(int argc, char** argv)
{
    const int FPS = 60;
    // const int frameDelay = 1000 / FPS;

    // uint32_t frameStart;
    // int frameTime;

    game = new Game();

    double initTime = glfwGetTime();
    double lastTime = glfwGetTime();
    double deltaTime = 0.0f;

    /* Loop until the user closes the window */
    while (game->getRunning())
    {
        deltaTime += (glfwGetTime() - lastTime) * FPS;
        lastTime = glfwGetTime();
        while (deltaTime >= 1.0f) {
            game->update();
            --deltaTime;
        }

        /* Render here */
        game->render();

        game->update();

        game->handleEvents();

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}