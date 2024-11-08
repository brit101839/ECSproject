//
// PositionComponent.h

#ifndef __PositionComponent__
#define __PositionComponent__

#include "ECS.h"

class PositionComponent : public Component
{
private:

	Vector2D _position;

public:

	PositionComponent()
	{
		_position = Vector2D();
		// _position.setVector2D(0.0f, 0.0f);
	}

	PositionComponent(Vector2D position)
	{
		_position = position;
		// _position.setVector2D(position);
	}

	Vector2D getPosition() { return _position; };

	void setPosition(Vector2D newPosition)
	{
		_position = newPosition;
		// _position.setVector2D(newPosition);
	}

	void update() override
	{
		// _position = addVector2(_position, makeVector2(0.1f, 0.1f));
		_position.operator+=(Vector2D(0.1f, 0.1f));
	}
};

#endif // !__PositionComponent__
