#include "Game.h"

#include "ECS/ECS.h"
#include "ECS/Components.h"
#include "ECS/Collision.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Quadtree/Quadtree.h"
#include "Map.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

Manager manager;

std::vector<ColliderComponent*> Game::colliders;

//auto& player(manager.addEntity());
//auto& wall(manager.addEntity());
//auto& Camera(manager.addEntity());

//auto& tiles(manager.getGroup(groupMap));
//auto& colliderTile(manager.getGroup(groupCollider));
//auto& players(manager.getGroup(groupPlayer));
//auto& enemies(manager.getGroup(groupEnemies));

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

bool Game::initEntityGroup()
{
    player = &manager.addEntity();
    wall = &manager.addEntity();
    Camera = &manager.addEntity();

    return false;
}

Game::Game()
    :_enemyManager(nullptr)
{
    // _isRunning = false;

    if (!initFlow("OpenGL practice", false)) {
        handleInitFailure();
    }

    setupGL();

    _shader.init("shader/shader.vert", "shader/shader.frag");
    _enemyManager = new EnemyManager(manager);

    Box<float> interBox{ {-5000.0f, -5000.0f} , {10000.0f, 10000.0f} };
    std::cout << "{ {" << interBox.left << "," << interBox.getBottom() << " }, {" << interBox.getRight() << "," << interBox.top << " } }" << std::endl;
    _renderManager = new RenderQuadtreeManager(interBox);

    _map = new Map(*this, "D:/dependencies/resource/map_town/map_town/map.json");

    initEntityGroup();

    player->addcomponent<TransformComponent>(Vector2D(400.0f, 100.0f), Vector2D(0.0f, 0.0f), 1.0f, 100.0f, 100.0f);
    // player.addcomponent<SpriteComponent>("D:/dependencies/resource/heart.png");
    auto& playerSprite = player->addcomponent<SpriteComponent>("D:/dependencies/resource/Dungeon/Adventurer Sprite Sheet v1.5.png", true);
    playerSprite.addAnimation("idle", Animation(0, 13, 10, false));
    playerSprite.addAnimation("walkL", Animation(1, 8, 10, true));
    playerSprite.addAnimation("walkR", Animation(1, 8, 10, false));
    playerSprite.addAnimation("walkUp", Animation(11, 8, 10, false));
    playerSprite.setAnimate("idle");
    player->addcomponent<KeyboardController>();

    auto trans = player->getComponent<TransformComponent>();
    BoundingBox bound{trans.position, 40.0f, 40.0f};
    player->addcomponent<ColliderComponent>("player", bound, Vector2D(0.f, -40.f));

    player->addGroup(groupPlayer);

    Camera->addcomponent<CameraComponent>(&player->getComponent<TransformComponent>());
    Camera->addGroup(groupCamera);
    
    // std::cout << player.getComponent<TransformComponent>().position.x << std::endl;

    wall->addcomponent<TransformComponent>(Vector2D(800.0f, 300.0f), Vector2D(0.0f, 0.0f), 0.3f, 100.0f, 100.0f);
    wall->addcomponent<SpriteComponent>("D:/dependencies/resource/heart.png", false);
    wall->addcomponent<ColliderComponent>("wall");
    wall->addGroup(groupMap);

    // _enemyManager->addEnemy(Vector2D(400.0f, 100.0f), "D:/dependencies/resource/Dungeon/Minotaur - Sprite Sheet.png");
    _enemyManager->addEnemy(Vector2D(400.0f, 100.0f), "D:/dependencies/resource/redShot.png");
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
    const char* sheetPath = "D:/dependencies/resource/map_town/map_town/spritesheet.png";
    auto& tile(manager.addEntity());
    tile.addcomponent<TileComponent>(position, tileSize, tileSize, id, sheetPath);
    

    if (collider) {
        std::string tag = "tile id: ";
        tile.addcomponent<ColliderComponent>(tag.append(std::to_string(id)));
        tile.addGroup(groupCollider);

        // quadTree.add(tile);
    }
    else {
        tile.addGroup(groupMap);
        _renderManager->addTransfrom(&tile.getComponent<TransformComponent>());
    }
}

void Game::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // manager.draw();
    Vector2D cameraPos = Camera->getComponent<CameraComponent>().CameraPos;
    quadtree::Box<float> cameraBound = Camera->getComponent<CameraComponent>().getBox();
    

    // for (auto& t : tiles) { t->draw(_shader, cameraPos); }
    for (auto& r : _renderManager->query(cameraBound)) { r->entity->draw(_shader, cameraPos); }
    for (auto& p : manager.getGroup(groupPlayer)) { p->draw(_shader, cameraPos); }
    // for (auto& e : manager.getGroup(groupEnemies)) { e->draw(_shader, cameraPos); }
    _enemyManager->renderEnemies(_shader, cameraPos);

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

    for (auto cc : colliders) {
        if (cc->tag != "player" && Collision::AABB(player->getComponent<ColliderComponent>().boundingBox, cc->boundingBox)) {
            std::cout << "collision: " << cc->tag << std::endl;
            std::cout << "position:" << player->getComponent<TransformComponent>().position.x << ", " << player->getComponent<TransformComponent>().position.y << std::endl;
            Vector2D mtv = Collision::calculateMTV(player->getComponent<ColliderComponent>().boundingBox, cc->boundingBox);
            player->getComponent<TransformComponent>().position += mtv;
            player->getComponent<TransformComponent>().update(_window);
        }
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