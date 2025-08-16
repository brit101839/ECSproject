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
	GLfloat width, height;

	bool canMove = true;

	TransformComponent()
	{
		position = Vector2D();
	}

	TransformComponent(Vector2D position, GLfloat width, GLfloat height)
		:position(position), width(width), height(height)
	{ 
	}

	quadtree::Box<float> getBox() const {
		return { {position.x - width / 2, position.y + height / 2},{width, height} };
	}

	quadtree::Box<float> getWideBox() const {
		return { {position.x - width * 100 / 2, position.y + height * 100 / 2},{width * 100, height * 100} };
	}

	void update(GLFWwindow* window, double deltaTime) override
	{
	}
};

#endif // !__TransformComponent__
