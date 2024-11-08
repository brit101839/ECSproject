#include "Game.h"

#include "ECS/ECS.h"
#include "ECS/Components.h"

Manager manager;
auto& player(manager.addEntity());

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

    glfwSetWindowUserPointer(_window, this);

    glfwSetMouseButtonCallback(_window, mouseButtonCallbackDispatcher);

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

void Game::mouseButtonCallbackDispatcher(GLFWwindow* window, int button, int action, int mods)
{
    Game* gameInstance = static_cast<Game*>(glfwGetWindowUserPointer(window));
    if (gameInstance) {
        gameInstance->onMouseButton(window, button, action);
    }
}

Game::Game()
    :_isRunning(false)
{
    if (!initFlow("OpenGL practice", false)) {
        handleInitFailure();
    }
    setupGL();

    _map = new Map();
    player.addcomponent<PositionComponent>(Vector2D(500.0f, 500.0f), Vector2D(1.0f, -1.0f), 0.1);
    player.addcomponent<SpriteComponent>("D:/dependencies/resource/heart.png");
    
    std::cout << player.getComponent<PositionComponent>().getPosition().x << std::endl;
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

void Game::onMouseButton(GLFWwindow* window, int button, int action)
{
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT)
    {
        // _shotArray->addItem(_shotArray->getSprite(), _playerRocket->getPostition());

        double x, y;
        glfwGetCursorPos(window, &x, &y);
        // y = _height - y;
        std::cout << "Left mouse button pressed!"<< x << " " << y << std::endl;
    }
}

void Game::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    _map->DrawMap();
    //_gameObject->render();
    player.draw();
    

    /* Swap front and back buffers */
    glfwSwapBuffers(_window);
}

void Game::update()
{
   //  player.getComponent<PositionComponent>().getPosition().operator+=(Vector2D(0.0f, 1.0f));
    // _gameObject->update();
    player.update();
}
