#include "MovementComponent.h"

void MovementComponent::init()
// Initialize the movement component with default values
{
	m_pTransformComponent = &entity->getComponent<TransformComponent>();
}

void MovementComponent::update(GLFWwindow* window, double deltaTime)
// Update the position of the entity based on its direction and speed
{
	Vector2D deltaVelocity = m_vDirection * m_fSpeed * deltaTime;
	m_pTransformComponent->position += deltaVelocity;
}

void MovementComponent::setDirection(Vector2D& newDirection)
// Set the direction of movement for the entity
{
	if (newDirection.x == 0 && newDirection.y == 0) {
		m_vDirection = Vector2D(0.f, 0.f);
		return;
	}
	m_vDirection = newDirection.normalize();
}
