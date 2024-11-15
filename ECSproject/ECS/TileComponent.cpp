#include "TileComponent.h"


TileComponent::TileComponent(Vector2D position, GLfloat width, GLfloat height, int id)
	:_position(position), _width(width), _height(height), _id(id)
{
	_path = "D:/dependencies/resource/map3/spritesheet.png";
}

void TileComponent::init()
{
	entity->addcomponent<TransformComponent>(_position, Vector2D(), 0.0f, _width, _height);
	transform = &entity->getComponent<TransformComponent>();

	GLuint tmpBufferID = _textureManager.TileSpriteManager(_id, _path);
	entity->addcomponent<SpriteComponent>(tmpBufferID);
	sprite = &entity->getComponent<SpriteComponent>();
}

