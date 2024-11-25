//
// SpriteComponent.h

#ifndef __SpriteComponent__
#define __SpriteComponent__

#include "ECS.h"
#include "../textureManager.h"
#include "TransformComponent.h"
#include "Animation.h"
#include "../shader/Shader.h"
#include <map>

class SpriteComponent : public Component
{
private:

	TransformComponent* _transform;
	GLuint _texture;
	Sprite* _sprite;
	int _textureWidth = 0, _textureHeight = 0;
	int _id;

	bool _map = false;
	bool _animated = false;
	int _frames = 0;
	int _speed = 100;
	bool _flip = false;

public:

	int tileY = 0;

	std::map<std::string, Animation> animations;

	SpriteComponent() = default;

	SpriteComponent(const char* path, bool isAnimated)
		:_animated(isAnimated)
	{
		if (isAnimated) {
			Animation idle = Animation(0, 13, 10, false);
			Animation walk_L = Animation(1, 8, 10, true);
			Animation walk_R = Animation(1, 8, 10, false);
			Animation walk_Up = Animation(11, 8, 10, false);
			// Animation walk_Down = Animation();

			animations.emplace("idle", idle);
			animations.emplace("walkL", walk_L);
			animations.emplace("walkR", walk_R);
			animations.emplace("walkUp", walk_Up);

			setAnimate("idle");
		}

		TextureManager& textureManager = TextureManager::getInstance();
		_texture = textureManager.textureManager(path, _textureWidth, _textureHeight);
		if (_texture == -1) {
			throw std::runtime_error("Failed to load texture.");
		}
	}

	SpriteComponent(int map_id, const char* path)
		:_id(map_id), _map(true)
	{
		TextureManager& textureManager = TextureManager::getInstance();
		_texture = textureManager.textureManager(path, _textureWidth, _textureHeight);
		if (_texture == -1) {
			throw std::runtime_error("Failed to load texture.");
		}
	}

	void init() override
	{
		_transform = &entity->getComponent<TransformComponent>();
		if (_animated) _sprite = new Sprite(_texture, _transform->width, _transform->height, _textureWidth, _textureHeight, 32.0f, 32.0f);
		else if (_map) _sprite = new Sprite(_texture, _transform->width, _transform->height, _textureWidth, _textureHeight, 16.0f, 16.0f, _id);
		else _sprite = new Sprite(_texture, _transform->width, _transform->height);
	}

	// void settexture() {};

	void update(GLFWwindow* window) override
	{
		static float lastFrameTime = 0.0f;
		static float frameInterval = 1.0f/_speed; // 每帧持续时间
		static int currentFrame = 0;
		int totalFrames = _frames; // 动画总帧数
		int framesPerRow = _frames; // 每行的帧数

		if (_animated) {

			// _sprite->updateFrame(currentFrame, framesPerRow);

			float currentTime = glfwGetTime();
			if (currentTime - lastFrameTime > frameInterval) {
				currentFrame = (currentFrame + 1) % totalFrames; // 循环播放
				_sprite->updateAnimateVertex(currentFrame, tileY, framesPerRow);
				lastFrameTime = currentTime;
			}
			_sprite->setFlip(_flip);
		}
	}

	void draw(Shader& shader, Vector2D cameraPos) override
	{
		// if (_animated) _sprite->animateRender(_transform->position, 0.0f);
		_sprite->render(_transform->position, 0.0f, shader, cameraPos);
	}

	void setAnimate(std::string animName)
	{
		tileY = animations[animName].tileY;
		_frames = animations[animName].frames;
		_speed = animations[animName].speed;
		_flip = animations[animName].flip;
	}

};


#endif // !__SpriteComponent