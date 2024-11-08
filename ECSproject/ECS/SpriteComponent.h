//
// SpriteComponent.h

#ifndef __SpriteComponent__
#define __SpriteComponent__

#include "ECS.h"
#include "../textureManager.h"
#include "PositionComponent.h"

class SpriteComponent : public Component
{
private:

	PositionComponent* _position;
	GLfloat _width, _height;
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
		//init;
	}

	void init() override
	{
		_position = &entity->getComponent<PositionComponent>();

		_width = 50.0f;
		_height = 50.0f;
		_sprite = new Sprite(_textureBufferID, _width, _height);
	}

	void settexture() {};

	void update() override
	{

	}

	void draw() override
	{
		_sprite->render(_position->getPosition().getVector2D(), 0.0f);
		//_sprite->render(makeVector2(0.0f, 0.0f), 0.0f);
	}

};


#endif // !__SpriteComponent__


#pragma once
