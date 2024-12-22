//
// SpriteComponent.h

#ifndef __SpriteComponent__
#define __SpriteComponent__

#include "ECS.h"
#include "../textureManager.h"
#include "TransformComponent.h"
#include "../AnimateSet.h"
#include "../shader/Shader.h"

class SpriteComponent : public Component
{
private:

	TransformComponent* _transform;
	GLuint _texture;
	Sprite* _sprite;
	AnimationSet _animationSet;
	const Animation* _activeAnimation;
	std::string _currentAnimation;
	int _textureWidth = 0, _textureHeight = 0;
	int _id;

	bool _map = false;
	bool _animated = false;

public:

	SpriteComponent() = default;

	SpriteComponent(const char* path, bool isAnimated)
		:_animated(isAnimated), _activeAnimation(nullptr)
	{
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

	void addAnimation(const std::string& name, const Animation& anim) {
		_animationSet.addAnimation(name, anim);
	}

	void setAnimate(std::string animName)
	{
		_currentAnimation = animName;
		_activeAnimation = &_animationSet.getAnimation(animName);
		if (_activeAnimation == nullptr) {
			_activeAnimation = &_animationSet.getAnimation("idle");
		}
	}

	// void settexture() {};

	void update(GLFWwindow* window) override
	{
		if (_animated) {
			static float lastFrameTime = 0.0f;
			static float frameInterval = 1.0f / _activeAnimation->speed; // 每帧持续时间
			static int currentFrame = 0;
			int totalFrames = _activeAnimation->frames; // 动画总帧数
			int framesPerRow = _activeAnimation->frames; // 每行的帧数

			float currentTime = glfwGetTime();
			if (currentTime - lastFrameTime > frameInterval) {
				currentFrame = (currentFrame + 1) % totalFrames; // 循环播放
				_sprite->updateAnimateVertex(currentFrame, _activeAnimation->tileY, framesPerRow);
				lastFrameTime = currentTime;
			}
			// _sprite->setFlip(_flip);
			_sprite->setFlip(_activeAnimation->flip);
		}
	}

	void draw(Shader& shader, Vector2D cameraPos) override
	{
		// if (_animated) _sprite->animateRender(_transform->position, 0.0f);
		_sprite->render(_transform->position, 0.0f, shader, cameraPos);
	}
};


#endif // !__SpriteComponent