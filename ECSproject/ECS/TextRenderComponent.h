#pragma once
#include "ECS.h"
#include "Components.h"
#include <vector>

class TextRenderComponent : public Component {
private:

	TransformComponent* _transform;
	GLuint _texture;
	Sprite* _sprite;
	std::vector<GLuint> _numberTextures;
	int _textureWidth = 0, _textureHeight = 0;
	int _renderNumber = 4235;

public:

	TextRenderComponent() = default;

	void init() override;
	
	void update(GLFWwindow* window) override
	{

	}

	void draw(Shader& shader, Vector2D cameraPos) override;
};
