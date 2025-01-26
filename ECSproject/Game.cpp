#include "Game.h"

#include "ECS/ECS.h"
#include "ECS/Components.h"
#include "ECS/Collision.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Quadtree/Quadtree.h"
#include "Map.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

Manager manager;

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

    return true;
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
    initEntityGroup();

    Box<float> interBox{ {-5000.0f, -5000.0f} , {10000.0f, 10000.0f} };
    std::cout << "{ {" << interBox.left << "," << interBox.getBottom() << " }, {" << interBox.getRight() << "," << interBox.top << " } }" << std::endl;
    _renderManager = new RenderQuadtreeManager(interBox);
    _colliderManager = new CollisionManager(interBox);

    _map = new Map(*this, "D:/dependencies/resource/map_town/map_town/map.json");

    auto trans = player->addcomponent<TransformComponent>(Vector2D(550.0f, -1200.0f), Vector2D(0.0f, 0.0f), 1.0f, 100.0f, 100.0f);
    // player.addcomponent<SpriteComponent>("D:/dependencies/resource/heart.png");
    auto& playerSprite = player->addcomponent<SpriteComponent>("D:/dependencies/resource/Dungeon/Adventurer Sprite Sheet v1.5.png", true, 32.f, 32.f);
    playerSprite.addAnimation("idle", Animation(0, 13, 10, false));
    playerSprite.addAnimation("walkL", Animation(1, 8, 10, true));
    playerSprite.addAnimation("walkR", Animation(1, 8, 10, false));
    playerSprite.addAnimation("walkUp", Animation(11, 8, 10, false));
    playerSprite.addAnimation("attack_1", Animation(2, 8, 10, false, false, AnimateState::Attacking));
    playerSprite.addAnimation("attack_2", Animation(3, 8, 10, false, false, AnimateState::Attacking));
    playerSprite.addAnimation("attack_3", Animation(4, 10, 10, false, false, AnimateState::Attacking));
    playerSprite.setAnimate("idle");
    player->addcomponent<KeyboardController>();

    BoundingBox bound{trans.position, 40.0f, 40.0f};
    player->addcomponent<ColliderComponent>(_colliderManager, "player", bound, Vector2D(0.f, -40.f));
    player->addcomponent<StatsComponent>(100, 10, 1);
    player->addGroup(groupPlayer);

    Camera->addcomponent<CameraComponent>(&player->getComponent<TransformComponent>());
    Camera->addGroup(groupCamera);

    _enemyManager = new EnemyManager(manager, player->getComponent<TransformComponent>(), _colliderManager);
    _enemyManager->addEnemy(Vector2D(550.0f, 100.f), "D:/dependencies/resource/Dungeon/Minotaur - Sprite Sheet.png");

    player->addcomponent<AttackComponent>(_enemyManager);
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
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        // y = _height - y;
        std::cout << "Left mouse button pressed!"<< x << " " << y << std::endl;

        player->getComponent<KeyboardController>().onLeftMouse();
    }
}

void Game::addTile(int id, GLfloat tileSize, Vector2D position, bool collider)
{
    const char* sheetPath = "D:/dependencies/resource/map_town/map_town/spritesheet.png";
    auto& tile(manager.addEntity());
    tile.addcomponent<TileComponent>(position, tileSize, tileSize, id, sheetPath);
    

    if (collider) {
        std::string tag = "tile id: ";
        tile.addcomponent<ColliderComponent>(_colliderManager, tag.append(std::to_string(id)));
        tile.addGroup(groupCollider);
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
    _enemyManager->renderEnemies(_shader, cameraPos);
    for (auto& p : manager.getGroup(groupPlayer)) { p->draw(_shader, cameraPos); }
    // for (auto& e : manager.getGroup(groupEnemies)) { e->draw(_shader, cameraPos); }
    

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(_window);
}

void Game::update()
{
    manager.refresh();
    manager.update(_window);

    quadtree::Box<float> cameraBound = Camera->getComponent<CameraComponent>().getBox();
    _colliderManager->checkCollisions(player, cameraBound, _window);
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