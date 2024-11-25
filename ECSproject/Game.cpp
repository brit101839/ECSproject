#include "Game.h"

#include "ECS/ECS.h"
#include "ECS/Components.h"
#include "ECS/Collision.h"
#include <glm/gtc/matrix_transform.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

Manager manager;

std::vector<ColliderComponent*> Game::colliders;

auto& player(manager.addEntity());
auto& wall(manager.addEntity());
auto& Camera(manager.addEntity());

auto& tiles(manager.getGroup(groupMap));
auto& players(manager.getGroup(groupPlayer));
auto& enemies(manager.getGroup(groupEnemies));

bool Game::_isRunning = false;

bool Game::initFlow(const char* title, bool fullscreen)
{
    /* Initialize the library */
    if (!glfwInit())
        return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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
    glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);

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

    // glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // glMatrixMode(GL_PROJECTION);
    // glOrtho(0, Window_w_Size, 0, Window_h_Size, -1, 1);
    // glMatrixMode(GL_MODELVIEW);
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
{
    // _isRunning = false;

    if (!initFlow("OpenGL practice", false)) {
        handleInitFailure();
    }

    setupGL();

    _shader.init("shader/shader.vert", "shader/shader.frag");

    _map = new Map();

    player.addcomponent<TransformComponent>(Vector2D((float)Window_w_Size/2, (float)Window_h_Size/2), Vector2D(0.0f, 0.0f), 1.0f, 100.0f, 100.0f);
    // player.addcomponent<SpriteComponent>("D:/dependencies/resource/heart.png");
    player.addcomponent<SpriteComponent>("D:/dependencies/resource/Dungeon/Adventurer Sprite Sheet v1.5.png", true);
    player.addcomponent<KeyboardController>();
    player.addcomponent<ColliderComponent>("player");
    player.addGroup(groupPlayer);

    Camera.addcomponent<CameraComponent>(&player.getComponent<TransformComponent>());
    Camera.addGroup(groupCamera);
    
    // std::cout << player.getComponent<TransformComponent>().position.x << std::endl;

    wall.addcomponent<TransformComponent>(Vector2D(800.0f, 300.0f), Vector2D(0.0f, 0.0f), 0.3f, 100.0f, 100.0f);
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
    if (!_isRunning) return _isRunning;
    _isRunning = !glfwWindowShouldClose(_window);
    return _isRunning;
}

Shader& Game::getShader()
{
    return _shader;
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

void Game::addTile(int id, GLfloat tileSize, Vector2D position, bool collider)
{
    auto& tile(manager.addEntity());
    tile.addcomponent<TileComponent>(position, tileSize, tileSize, id);
    tile.addGroup(groupMap);
}

void Game::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    //// manager.draw();
    Vector2D cameraPos = Camera.getComponent<CameraComponent>().CameraPos;

    for (auto& t : tiles) { t->draw(_shader, cameraPos); }
    for (auto& p : players) { p->draw(_shader, cameraPos); }
    for (auto& e : enemies) { e->draw(_shader, cameraPos); }

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(_window);
}

void Game::update()
{
    //player.update(_window);
    //wall.update(_window);
    manager.refresh();
    manager.update(_window);

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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);

    //// 重新计算正交投影矩阵
    //glm::mat4 projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height, -1.0f, 1.0f);

    //// 更新到着色器中（需在回调中获取当前使用的 Shader 对象）
    //shader.use();
    //shader.setMat4("projection", projection);
}