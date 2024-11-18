//
// SpriteComponent.h

#ifndef __SpriteComponent__
#define __SpriteComponent__

#include "ECS.h"
#include "../textureManager.h"
#include "TransformComponent.h"

class SpriteComponent : public Component
{
private:

	TransformComponent* _transform;
	GLuint _textureBufferID;
	Sprite* _sprite;
	int _textureWidth, _textureHeight;

	bool _animated = false;
	int _frames = 0;
	int _speed = 100;

public:

	SpriteComponent() = default;
	SpriteComponent(const char* path)
	{
		_textureBufferID = TextureManager::loadAndBufferImage(path, _textureWidth, _textureHeight);
		if (_textureBufferID == -1) {
			throw std::runtime_error("Failed to load texture.");
		}
	}

	SpriteComponent(const char* path, int nFrames, int mSpeed)
		:_frames(nFrames), _speed(mSpeed), _animated(true)
	{
		_textureBufferID = TextureManager::loadAndBufferImage(path, _textureWidth, _textureHeight);
		if (_textureBufferID == -1) {
			throw std::runtime_error("Failed to load texture.");
		}
	}

	SpriteComponent(GLuint textureBufferID)
		:_textureBufferID(textureBufferID)
	{

	}

	void init() override
	{
		_transform = &entity->getComponent<TransformComponent>();
		if (_animated) _sprite = new Sprite(_textureBufferID, _transform->width, _transform->height, _textureWidth, _textureHeight, 32.0f, 32.0f);
		else _sprite = new Sprite(_textureBufferID, _transform->width, _transform->height);
	}

	void settexture() {};

	void update(GLFWwindow* window) override
	{
		static float lastFrameTime = 0.0f;
		static float frameInterval = 0.1f; // 每帧持续时间
		static int currentFrame = 0;
		static int totalFrames = 13; // 动画总帧数
		static int framesPerRow = 13; // 每行的帧数

		if (_animated) {

			// _sprite->updateFrame(currentFrame, framesPerRow);

			float currentTime = glfwGetTime();
			if (currentTime - lastFrameTime > frameInterval) {
				currentFrame = (currentFrame + 1) % totalFrames; // 循环播放
				_sprite->updateFrame(currentFrame, framesPerRow);
				lastFrameTime = currentTime;
			}
		}
	}

	void draw() override
	{
		// if (_animated) _sprite->animateRender(_transform->position, 0.0f);
		_sprite->render(_transform->position, 0.0f);
	}

};


#endif // !__SpriteComponent__


#pragma once
