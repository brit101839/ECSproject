//
// TransformComponent.h

#ifndef __TransformComponent__
#define __TransformComponent__

#include "ECS.h"
#include "Quadtree/Box.h"

class TransformComponent : public Component
{
private:

public:

	Vector2D position;
	Vector2D velocity = Vector2D(0.f, 0.f);
	GLfloat width, height;

	float speed;

	bool canMove = true;

	TransformComponent()
	{
		position = Vector2D();
		velocity = Vector2D();
		speed = 0;
	}

	TransformComponent(Vector2D position, GLfloat width, GLfloat height)
		:position(position), velocity(Vector2D(0.f, 0.f)), speed(0), width(width), height(height){ }

	TransformComponent(Vector2D position, float speed, GLfloat width, GLfloat height)
		:position(position), speed(speed), width(width), height(height)
	{

	}

	TransformComponent(Vector2D position, Vector2D velocity, float speed, GLfloat width, GLfloat height)
		:position(position), velocity(velocity), speed(speed), width(width), height(height)
	{

	}

	quadtree::Box<float> getBox() const {
		return { {position.x - width / 2, position.y + height / 2},{width, height} };
	}

	quadtree::Box<float> getWideBox() const {
		return { {position.x - width * 100 / 2, position.y + height * 100 / 2},{width * 100, height * 100} };
	}

	void update(GLFWwindow* window) override
	{
		Vector2D tempVec = velocity * speed;
		position+=(tempVec);
	}
};

#endif // !__TransformComponent__
