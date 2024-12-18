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
	float mapWidth = 52.0f * 80.0f, mapHeight = 39.0f * 80.0f;

public:

	Vector2D CameraPos;

	CameraComponent(TransformComponent* target)
	{
		_target = target;
	}

	quadtree::Box<float> getBox() const {
		float width = Window_w_Size;
		float height = Window_h_Size;
		// std::cout << "{ {" << CameraPos.x << "," << CameraPos.y << " }, {" << CameraPos.x + width << "," << CameraPos.y + height << " } }" << std::endl;
		return { {CameraPos.x, CameraPos.y},{width, height+100.0f} };
	}

	void update(GLFWwindow* window) override 
	{
		if (_followTarget) {
			Vector2D playerPos = _target->position;
			CameraPos = playerPos - Vector2D(Window_w_Size / 2, Window_h_Size / 2);

			CameraPos.x = std::max(0.0f, std::min(CameraPos.x, mapWidth - Window_w_Size));
			CameraPos.y = std::max(Window_h_Size - mapHeight, std::min(CameraPos.y, 0.0f));
			// std::cout << "cameraPos: " << CameraPos.y << "," << std::endl;
		}
	}
};
