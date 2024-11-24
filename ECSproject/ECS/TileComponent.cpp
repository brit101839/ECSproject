#include "TileComponent.h"
#include "../textureManager.h"


TileComponent::TileComponent(Vector2D position, GLfloat width, GLfloat height, int id)
	:_position(position), _width(width), _height(height), _id(id)
{
	_path = "D:/dependencies/resource/map3/spritesheet.png";
}

void TileComponent::init()
{
	entity->addcomponent<TransformComponent>(_position, Vector2D(), 0.0f, _width, _height);
	transform = &entity->getComponent<TransformComponent>();

	// TextureManager& textureManager = TextureManager::getTnstance();

	// GLuint tmpBufferID = textureManager.TileSpriteManager(_id, _path);
	/*int textureWidth, textureHeight;
	GLuint tmpBufferID = textureManager.loadAndBufferImage(_path, textureWidth, textureHeight);*/

	entity->addcomponent<SpriteComponent>(_id, _path);
	sprite = &entity->getComponent<SpriteComponent>();
}

