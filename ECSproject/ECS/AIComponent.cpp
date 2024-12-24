#include "AIComponent.h"

//void AIComponent::update(GLFWwindow* window)
//{
//	auto playerT = _playerTrans.position;
//	auto dist = _transform->position.distanceTo(playerT);
//	bool canMove = _transform->canMove;
//	if (dist < 100.f) {
//		_state = EnemyState::fighting;
//		_transform->velocity = Vector2D(0.f, 0.f);
//		_sprite->setAnimate("attack_1");
//	}
//	else if (!canMove) { return; }
//	else if (dist < 400.f) {
//		_state = EnemyState::tracking;
//		_transform->velocity = (playerT - _transform->position).normalize();
//		if (_transform->velocity.x >= 0) { _sprite->setAnimate("walkR"); }
//		else { _sprite->setAnimate("walkL"); }
//	}
//	else if (dist > 400.f) {
//		dist = _transform->position.distanceTo(_defaltPos);
//		if (dist > 50.f) {
//			_state = EnemyState::backing;
//			_transform->velocity = (_defaltPos - _transform->position).normalize();
//			if (_transform->velocity.x >= 0) { _sprite->setAnimate("walkR"); }
//			else { _sprite->setAnimate("walkL"); }
//		}
//		else {
//			_state = EnemyState::patrol;
//			_sprite->setAnimate("idle");
//		}
//	}
//}
