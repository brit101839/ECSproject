//
// PositionComponent.h

#ifndef __PositionComponent__
#define __PositionComponent__

#include "ECS.h"

class PositionComponent : public Component
{
private:

	Vector2D _position;
	Vector2D _velocity;

	float _speed;

public:

	PositionComponent()
	{
		_position = Vector2D();
		_velocity = Vector2D();
		_speed = 0;
	}

	PositionComponent(Vector2D position, Vector2D velocity, float speed)
	{
		_position = position;
		_velocity = velocity;
		_speed = speed;
	}

	Vector2D getPosition() { return _position; }

	void setPosition(Vector2D newPosition) { _position = newPosition; }

	void setVelocity(Vector2D newVelocity) { _velocity = newVelocity; }

	void update() override
	{
		Vector2D tempVec = _velocity;
		tempVec.operator*=(Vector2D(_speed, _speed));
		_position.operator+=(tempVec);
	}
};

#endif // !__PositionComponent__
