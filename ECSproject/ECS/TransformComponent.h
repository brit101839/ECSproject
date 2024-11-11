//
// TransformComponent.h

#ifndef __TransformComponent__
#define __TransformComponent__

#include "ECS.h"

class TransformComponent : public Component
{
private:

	

public:

	Vector2D position;
	Vector2D velocity;
	GLfloat width, height;

	float speed;

	TransformComponent()
	{
		position = Vector2D();
		velocity = Vector2D();
		speed = 0;
	}

	TransformComponent(Vector2D position, Vector2D velocity, float speed, GLfloat width, GLfloat height)
		:position(position), velocity(velocity), speed(speed), width(width), height(height)
	{

	}

	//Vector2D getPosition() { return position; }
	//void setPosition(Vector2D newPosition) { position = newPosition; }

	//void setVelocity(Vector2D newVelocity) { velocity = newVelocity; }
	//void setVelocity_x(GLfloat new_x) { velocity.x = new_x; }
	//void setVelocity_y(GLfloat new_y) { velocity.y = new_y; }

	void update(GLFWwindow* window) override
	{
		Vector2D tempVec = velocity * speed;
		position+=(tempVec);
	}
};

#endif // !__TransformComponent__
