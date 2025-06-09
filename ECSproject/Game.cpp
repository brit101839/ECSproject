#include "Game.h"

#include "ECS/ECS.h"
#include "ECS/Components.h"
#include "ECS/Collision.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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
    _window = glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, title, NULL, NULL);

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

    // glfwSetMouseButtonCallback(_window, mouseButtonCallbackDispatcher);

    _isRunning = true;
    return true;
}

void Game::setupGL()
{
    // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glViewport(0.0f, 0.0f, SRC_WIDTH, SRC_HEIGHT);

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

//void Game::mouseButtonCallbackDispatcher(GLFWwindow* window, int button, int action, int mods)
//{
//    Game* gameInstance = static_cast<Game*>(glfwGetWindowUserPointer(window));
//    if (gameInstance) {
//        gameInstance->keyCallback(window, button, action);
//    }
//}

bool Game::initEntityGroup()
{
    Entity* playerEntity = &manager.addEntity();
    player = new Player(playerEntity, _globalEventManager, _spawnSystem);
    Entity* backPackEntity = &manager.addEntity();
    backpack = new BackPack(backPackEntity);
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

    _spriteShader.init("shader/sprite.vs", "shader/sprite.fs");
    _textShader.init("shader/text.vs", "shader/text.fs");

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SRC_WIDTH), 0.0f, static_cast<float>(SRC_HEIGHT));
    _textShader.use();
    glUniformMatrix4fv(glGetUniformLocation(_textShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    _textRender = new TextRender(_textShader);

    _textRender->initFont("C:\\dependencies\\resource\\text\\minecraftia\\Minecraftia-Regular.ttf");
    _textRender->initTextRendering();

    Box<float> interBox{ {-10000.0f, -10000.0f} , {20000.0f, 20000.0f} };
    _colliderManager = new CollisionManager(interBox, _globalEventManager);
	_projectileManager = std::make_shared<ProjectileManager>();
    _spawnSystem = std::make_shared<SpawnSystem>(manager, *_projectileManager, _globalEventManager);

    initEntityGroup();
    
    std::cout << "{ {" << interBox.left << "," << interBox.getBottom() << " }, {" << interBox.getRight() << "," << interBox.top << " } }" << std::endl;
    _renderManager = new RenderQuadtreeManager(interBox);

    _map = new Map(*this, "C:/dependencies/resource/map/map_town/map.json");

    player->componentSetting(_colliderManager);

    Camera->addcomponent<CameraComponent>(&player->getEntity().getComponent<TransformComponent>());
    Camera->addGroup(groupCamera);

    _enemyManager = new EnemyManager(manager, player->getEntity().getComponent<TransformComponent>(), _colliderManager, _globalEventManager, _spawnSystem);
    _enemyManager->addEnemy("Necromancer", Vector2D(4500.0f, 600.f));
    _enemyManager->addEnemy("Night Borne", Vector2D(580.0f, -1300.f));
    _enemyManager->addEnemy("Cacodaemon", Vector2D(2270.0f, -2700.f));
    _enemyManager->addEnemy("Minotaur", Vector2D(4530.0f, -950.f));
    _enemyManager->addEnemy("BringOfDeath", Vector2D(1120.f, -4600.f));
    // _enemyManager->addEnemy("Ratflok Axe", Vector2D(1500.0f, -1500.f));
    
    _UIManager = new UIManager(manager, *player);
    _UIManager->init(_textRender);
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
    return _spriteShader;
}

void Game::handleEvents()
{
    /* Poll for and process events */
    glfwPollEvents();

    static bool leftMousePressed = false;

    int mouseState = glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_LEFT);
    if (mouseState == GLFW_PRESS && !leftMousePressed)
    {
        leftMousePressed = true;
        keyCallback(_window, GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS);
    }
    else if (mouseState == GLFW_RELEASE)
    {
        leftMousePressed = false;
    }

    player->getEntity().getComponent<KeyboardController>().onKeyboard(_window);
}

void Game::keyCallback(GLFWwindow* window, int button, int action)
{
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT)
    {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        // y = _height - y;
        std::cout << "Left mouse button pressed!"<< x << " " << y << std::endl;

        player->getEntity().getComponent<KeyboardController>().onLeftMouse();
    }
    else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        backpack->disable();
    }
}

void Game::addTile(int id, GLfloat tileSize, Vector2D position, bool collider)
{
    const char* sheetPath = "C:/dependencies/resource/map/map_town/spritesheet.png";
    auto& tile(manager.addEntity());
    tile.addcomponent<TileComponent>(position, tileSize, tileSize, id, sheetPath);
    

    if (collider) {
        std::string tag = "tile id: ";
        tile.addcomponent<ColliderComponent>(_globalEventManager, tag.append(std::to_string(id)));
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
    for (auto& r : _renderManager->query(cameraBound)) { r->entity->draw(_spriteShader, cameraPos); }
    _enemyManager->renderEnemies(_spriteShader, cameraPos);
    for (auto& p : manager.getGroup(groupPlayer)) { p->draw(_spriteShader, cameraPos); }
    // for (auto& e : manager.getGroup(groupEnemies)) { e->draw(_shader, cameraPos); }
    // _spawnSystem.get()->render(_spriteShader, cameraPos);
    _projectileManager->render(_spriteShader, cameraPos);
    _UIManager->render(_spriteShader, cameraPos);
    backpack->render(_spriteShader, cameraPos);

    manager.refresh();
    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(_window);
}

void Game::update(double deltaTime)
{
    // manager.refresh();
    manager.update(_window, deltaTime);

    _UIManager->update();

    quadtree::Box<float> cameraBound = Camera->getComponent<CameraComponent>().getBox();
    _colliderManager->checkCollisions(&player->getEntity(), cameraBound, _window);
    _projectileManager->removeOutOfBounds(cameraBound);
    // _colliderManager->removeOutOfBounds(cameraBound);

  //  std::cout << "player velocity: " << player->getEntity().getComponent<TransformComponent>().getVelocity().x << ", "
		//<< player->getEntity().getComponent<TransformComponent>().getVelocity().y << std::endl;
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