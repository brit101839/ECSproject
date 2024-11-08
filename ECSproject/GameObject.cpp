#include "GameObject.h"

GameObject::GameObject(Sprite* sprite, Vector2D position, Vector2D velcocity)
	:_sprite(sprite), _position(position), _velocity(velcocity)
{
}

GameObject::~GameObject()
{
}

void GameObject::render()
{
	//_sprite->render(_position, _rotation);
}

void GameObject::update()
{
	//_position = addVector2(_position, _velocity);
}
