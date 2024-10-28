#include "Game.h"

bool Game::initFlow(const char* title, bool fullscreen)
{
    /* Initialize the library */
    if (!glfwInit())
        return false;

    glfwDefaultWindowHints();

    /* Create a windowed mode window and its OpenGL context */
    _window = glfwCreateWindow(Window_w_Size, Window_h_Size, title, NULL, NULL);

    if (!_window) {
        std::cerr << "Error: Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return false;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    _isRunning = true;
    return true;
}

void Game::setupGL()
{
    // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glViewport(0.0f, 0.0f, Window_w_Size, Window_h_Size);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);
    glOrtho(0, Window_w_Size, 0, Window_h_Size, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}

void Game::handleInitFailure()
{
    std::cerr << "Error: Window initialization failed, terminating program." << std::endl;
    cleanup();
    exit(EXIT_FAILURE);  // 終止程序，或根據需求處理錯誤
}

void Game::cleanup()
{
    if (_window) {
        glfwDestroyWindow(_window);
    }
    glfwTerminate();
}

Game::Game()
    :_isRunning(false)
{
    if (!initFlow("OpenGL practice", false)) {
        handleInitFailure();
    }
    setupGL();

    Sprite* tmpSprite = new Sprite();
    GLint texture = tmpSprite->loadAndBufferImage("C:/Users/brit/source/repos/opengl/opengl/resourse/rocket.png");
    _testsprite = new Sprite(texture, 50, 100);
    delete tmpSprite;
}

Game::~Game()
{
    cleanup();
}

bool Game::getRunning()
{
    _isRunning = !glfwWindowShouldClose(_window);
    return _isRunning;
}

void Game::handleEvents()
{
}

void Game::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    _testsprite->render(100.0f, 100.0f, 0.0f);

    /* Swap front and back buffers */
    glfwSwapBuffers(_window);
}

void Game::update()
{
}
