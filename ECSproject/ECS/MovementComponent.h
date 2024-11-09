//
// MovementComponent.h

#ifndef __MovementComponent__
#define __MovementComponent__

#include "ECS.h"

class MovementComponent : public Component
{
private:

	Vector2D _position;
	Vector2D _velocity;

	float _speed;

public:

	MovementComponent()
	{
		_position = Vector2D();
		_velocity = Vector2D();
		_speed = 0;
	}

	MovementComponent(Vector2D position, Vector2D velocity, float speed)
	{
		_position = position;
		_velocity = velocity;
		_speed = speed;
	}

	Vector2D getPosition() { return _position; }

	void setPosition(Vector2D newPosition) { _position = newPosition; }

	void setVelocity(Vector2D newVelocity) { _velocity = newVelocity; }
	void setVelocity_x(GLfloat new_x) { _velocity.x = new_x; }
	void setVelocity_y(GLfloat new_y) { _velocity.y = new_y; }

	void update(GLFWwindow* window) override
	{
		Vector2D tempVec = _velocity;
		tempVec.operator*=(Vector2D(_speed, _speed));
		_position.operator+=(tempVec);
	}
};

#endif // !__MovementComponent__
