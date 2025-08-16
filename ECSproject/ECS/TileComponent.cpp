#include "TileComponent.h"
#include "../textureManager.h"


TileComponent::TileComponent(Vector2D position, GLfloat width, GLfloat height, int id, const char* sheetPath)
	:_position(position), _width(width), _height(height), _id(id)
{
	_path = sheetPath;
	// _path = "D:\\dependencies\\resource\\map_town\\map_town\\spritesheet.png";
}

void TileComponent::init()
{
	entity->addcomponent<TransformComponent>(_position, _width, _height);
	transform = &entity->getComponent<TransformComponent>();

	// TextureManager& textureManager = TextureManager::getTnstance();

	// GLuint tmpBufferID = textureManager.TileSpriteManager(_id, _path);
	/*int textureWidth, textureHeight;
	GLuint tmpBufferID = textureManager.loadAndBufferImage(_path, textureWidth, textureHeight);*/

	entity->addcomponent<SpriteComponent>(_id, _path);
	sprite = &entity->getComponent<SpriteComponent>();
}

