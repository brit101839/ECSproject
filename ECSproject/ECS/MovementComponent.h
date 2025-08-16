#pragma once

#include "ECS.h"
#include "TransformComponent.h"
#include "../vector2D/Vector2D.h"

class MovementComponent : public Component
{
public:
	MovementComponent(float speed = 0.0f, Vector2D direction = Vector2D()) : m_fSpeed(speed), m_fDefaultSpeed(speed) 
	{
		m_vDirection = direction;
	}

	void init(void) override;

	void update(GLFWwindow* window, double deltaTime) override;

	void stop(void) { m_fSpeed = 0.0f; }

	void setToDefaultSpeed(void) { m_fSpeed = m_fDefaultSpeed; }

	float GetSpeed(void) const { return m_fSpeed; }

	void setDirection(Vector2D& newDirection);

	Vector2D getDirection(void) const { return m_vDirection; }

private:
	float m_fSpeed = 0;
	const float m_fDefaultSpeed = 0;

	Vector2D m_vDirection = Vector2D(0.0f, 0.0f);

	TransformComponent* m_pTransformComponent = nullptr;
};

