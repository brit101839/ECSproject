//
// CameraComponent.h

#pragma once

#include "ECS.h"
#include "Components.h"
#include <algorithm>

class CameraComponent :public Component
{
private:

	float _zoom = 1.0f;
	TransformComponent* _target;
	bool _followTarget = true;
	float mapWidth = 2320.0f, mapHeight = 1280.0f;

public:

	Vector2D CameraPos;

	CameraComponent(TransformComponent* target)
	{
		_target = target;
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
