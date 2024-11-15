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

public:

	SpriteComponent() = default;
	SpriteComponent(const char* path)
	{
		_textureBufferID = TextureManager::loadAndBufferImage(path);
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

		_sprite = new Sprite(_textureBufferID, _transform->width, _transform->height);
	}

	void settexture() {};

	void update(GLFWwindow* window) override
	{

	}

	void draw() override
	{
		_sprite->render(_transform->position, 0.0f);
		//_sprite->render(makeVector2(0.0f, 0.0f), 0.0f);
	}

};


#endif // !__SpriteComponent__


#pragma once
