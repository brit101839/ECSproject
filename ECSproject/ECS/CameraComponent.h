//
// CameraComponent.h

#pragma once

#include "ECS.h"
#include "Components.h"
#include <algorithm>
#include "Quadtree/Box.h"

class CameraComponent :public Component
{
private:

	float _zoom = 1.0f;
	TransformComponent* _target;
	bool _followTarget = true;
	float mapWidth = 104.0f * 80.0f, mapHeight = 83.0f * 80.0f;

public:

	Vector2D CameraPos;

	CameraComponent(TransformComponent* target)
	{
		_target = target;
	}

	quadtree::Box<float> getBox() const {
		float width = SRC_WIDTH;
		float height = SRC_HEIGHT;
		// std::cout << "{ {" << CameraPos.x << "," << CameraPos.y << " }, {" << CameraPos.x + width << "," << CameraPos.y + height << " } }" << std::endl;
		return { {CameraPos.x, CameraPos.y},{width, height+100.0f} };
		/*return { {CameraPos.x, CameraPos.y},{width-100.f, height-100.f} };*/
	}

	void update(GLFWwindow* window) override 
	{
		if (_followTarget) {
			Vector2D playerPos = _target->position;
			CameraPos = playerPos - Vector2D(SRC_WIDTH / 2, SRC_HEIGHT / 2);

			CameraPos.x = std::max(0.0f, std::min(CameraPos.x, mapWidth - SRC_WIDTH));
			CameraPos.y = std::max(SRC_HEIGHT - mapHeight, std::min(CameraPos.y, 0.0f));
			// std::cout << "cameraPos: " << CameraPos.y << "," << std::endl;
		}
	}
};
