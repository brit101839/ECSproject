#pragma once
#include "ECS.h"
#include "../TextRender.h"

class TextRenderComponent : public Component {
private:

	TextRender* _textRender;
	TransformComponent* _transform;

public:

	std::string mText = "123";

	TextRenderComponent(TextRender* t) : _textRender(t) {

	}

	void init() override {
		_transform = &entity->getComponent<TransformComponent>();
	}
	
	void update(GLFWwindow* window, double deltaTime) override
	{

	}

	void draw(Shader& shader, Vector2D cameraPos) override {
		_textRender->renderText(mText, _transform->position, 1.0f, { 1.0f, 1.0f, 1.0f });
	}
};
