//
// PositionComponent.h

#ifndef __PositionComponent__
#define __PositionComponent__

#include "ECS.h"

class PositionComponent : public Component
{
private:

	Vector2 _position;

public:

	PositionComponent()
	{
		init();
	}

	Vector2 getPosition() { return _position; };

	void init() override
	{
		_position = makeVector2(50.0f, 50.0f);
	}

	void update() override
	{
		_position = addVector2(_position, makeVector2(0.1f, 0.1f));
	}

	void setPosition(Vector2 newPosition)
	{
		_position = newPosition;
	}
};

#endif // !__PositionComponent__
