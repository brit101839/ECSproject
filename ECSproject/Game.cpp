#include "Game.h"

#include "ECS/ECS.h"
#include "ECS/Components.h"
#include "ECS/Collision.h"

Manager manager;

std::vector<ColliderComponent*> Game::colliders;

auto& player(manager.addEntity());
auto& wall(manager.addEntity());

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
        gameInstance->keyCallback(window, button, action);
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

    player.addcomponent<TransformComponent>(Vector2D(500.0f, 500.0f), Vector2D(0.0f, 0.0f), 0.3f, 100.0f, 100.0f);
    // player.addcomponent<SpriteComponent>("D:/dependencies/resource/heart.png");
    player.addcomponent<SpriteComponent>("D:/dependencies/resource/Dungeon/Adventurer Sprite Sheet v1.5.png", true);
    player.addcomponent<KeyboardController>();
    player.addcomponent<ColliderComponent>("player");
    player.addGroup(groupPlayer);
    
    // std::cout << player.getComponent<TransformComponent>().position.x << std::endl;

    wall.addcomponent<TransformComponent>(Vector2D(400.0f, 400.0f), Vector2D(0.0f, 0.0f), 0.3f, 100.0f, 100.0f);
    wall.addcomponent<SpriteComponent>("D:/dependencies/resource/heart.png", false);
    wall.addcomponent<ColliderComponent>("wall");
    wall.addGroup(groupMap);
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

void Game::keyCallback(GLFWwindow* window, int button, int action)
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

void Game::addTile(int id, Vector2D position, bool collider)
{
    auto& tile(manager.addEntity());
    tile.addcomponent<TileComponent>(position, 48, 48, id);
    tile.addGroup(groupMap);
}

auto& tiles(manager.getGroup(groupMap));
auto& players(manager.getGroup(groupPlayer));
auto& enemies(manager.getGroup(groupEnemies));

void Game::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // manager.draw();

    for (auto& t : tiles) { t->draw(); }
    for (auto& p : players) { p->draw(); }
    for (auto& e : enemies) { e->draw(); }

    /* Swap front and back buffers */
    glfwSwapBuffers(_window);
}

void Game::update()
{
   // _gameObject->update();
    
    player.update(_window);
    wall.update(_window);

    /*for (auto cc : colliders) {
        Collision::AABB(player.getComponent<ColliderComponent>(), *cc);
    }*/

    if (Collision::AABB(player.getComponent<ColliderComponent>().boundingBox,
        wall.getComponent<ColliderComponent>().boundingBox)) {
        Direction dir = Collision::collisionDirect(player.getComponent<ColliderComponent>().boundingBox,
            wall.getComponent<ColliderComponent>().boundingBox);
        if (dir == Up || dir == Down) {
            player.getComponent<TransformComponent>().velocity.y *= -2;
            // std::cout << "up or down" << std::endl;
        }
        else if (dir == Left || dir == Right) {
            player.getComponent<TransformComponent>().velocity.x *= -2;
        }
        player.getComponent<TransformComponent>().update(_window);
    }

    
}
