﻿//
// SpriteComponent.h

#ifndef __SpriteComponent__
#define __SpriteComponent__

#include "ECS.h"
#include "../textureManager.h"
#include "TransformComponent.h"
#include "../AnimateSet.h"
#include "../shader/Shader.h"

class SpriteComponent : public Component
{
private:

	TransformComponent* _transform;
	GLuint _texture;
	Sprite* _sprite;
	AnimationSet _animationSet;
	
	std::string _currentAnimation;
	GLfloat _cutWidth, _cutHeight;
	int _textureWidth = 0, _textureHeight = 0;
	int _id;

	bool _map = false;
	bool _animated = false;
	bool _OnAttack = false;
	float beginAttackTime;

public:

	SpriteComponent() = default;

	SpriteComponent(const char* path, bool isAnimated, GLfloat cutWidth, GLfloat cutHeight)
		:_animated(isAnimated), _cutWidth(cutWidth), _cutHeight(cutHeight)
	{
		TextureManager& textureManager = TextureManager::getInstance();
		_texture = textureManager.textureManager(path, _textureWidth, _textureHeight);
		if (_texture == -1) {
			throw std::runtime_error("Failed to load texture.");
		}
	}

	SpriteComponent(int map_id, const char* path)
		:_id(map_id), _map(true)
	{
		TextureManager& textureManager = TextureManager::getInstance();
		_texture = textureManager.textureManager(path, _textureWidth, _textureHeight);
		if (_texture == -1) {
			throw std::runtime_error("Failed to load texture.");
		}
	}

	void init() override
	{
		_transform = &entity->getComponent<TransformComponent>();
		if (_animated) _sprite = new Sprite(_texture, _transform->width, _transform->height, _textureWidth, _textureHeight, _cutWidth, _cutHeight);
		else if (_map) _sprite = new Sprite(_texture, _transform->width, _transform->height, _textureWidth, _textureHeight, 16.0f, 16.0f, _id);
		else _sprite = new Sprite(_texture, _transform->width, _transform->height);
	}

	void addAnimation(const std::string& name, const Animation& anim) {
		_animationSet.addAnimation(name, anim);
	}

	void setAnimate(std::string animName)
	{
		_animationSet.setAnimation(animName);
	}

	void OnAttack() {
		beginAttackTime = glfwGetTime();
		_OnAttack = true;
	}

	// void settexture() {};

	void update(GLFWwindow* window) override
	{
		if (_animated) {
			_animationSet.update(_sprite);
			_transform->canMove = _animationSet.getFrameInterrupt();
		}
		if (_OnAttack) {
			float time = glfwGetTime(); // 獲取時間
			float sqr = (sin(time * 10) > 0.5) ? 1.0f : 0.0f;
			_sprite->setOverlapColor(glm::vec3(1.0f - (1 - sqr)/4, sqr, sqr)); // 閃爍紅色
			if (time - beginAttackTime > 0.3) {
				_OnAttack = false;
				_sprite->setOverlapColor(glm::vec3(1.0f, 1.0f, 1.0f));
			}
		}
	}

	void draw(Shader& shader, Vector2D cameraPos) override
	{
		// if (_animated) _sprite->animateRender(_transform->position, 0.0f);
		_sprite->render(_transform->position, 0.0f, shader, cameraPos);
	}

	bool isanimated() { return _animated; }
	bool getFlip() { return _sprite->getFlip(); }
	GLfloat getCutWidth() { return _cutWidth; }
};


#endif // !__SpriteComponent