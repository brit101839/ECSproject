//
// Components.h

#ifndef __Components__
#define __Components__

#include "ECS.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"
#include "KeyboardController.h"
#include "ColliderComponent.h"
#include "TileComponent.h"
#include "CameraComponent.h"
#include "AIComponent.h"

enum GroupLabels : std::size_t
{
	groupMap,
	groupCollider,
	groupPlayer,
	groupEnemies,
	groupColliders,
	groupCamera
};

#endif // !__Components__