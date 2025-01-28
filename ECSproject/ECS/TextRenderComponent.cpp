#include "TextRenderComponent.h"
#include <string>

void TextRenderComponent::init()
{
	std::string numberPath = "D:/dependencies/resource/number/";
	TextureManager& textureManager = TextureManager::getInstance();
	_numberTextures.push_back(textureManager.textureManager((numberPath + "0.png").c_str(), _textureWidth, _textureHeight));
	_numberTextures.push_back(textureManager.textureManager((numberPath + "1.png").c_str(), _textureWidth, _textureHeight));
	_numberTextures.push_back(textureManager.textureManager((numberPath + "2.png").c_str(), _textureWidth, _textureHeight));
	_numberTextures.push_back(textureManager.textureManager((numberPath + "3.png").c_str(), _textureWidth, _textureHeight));
	_numberTextures.push_back(textureManager.textureManager((numberPath + "4.png").c_str(), _textureWidth, _textureHeight));
	_numberTextures.push_back(textureManager.textureManager((numberPath + "5.png").c_str(), _textureWidth, _textureHeight));
	_numberTextures.push_back(textureManager.textureManager((numberPath + "6.png").c_str(), _textureWidth, _textureHeight));
	_numberTextures.push_back(textureManager.textureManager((numberPath + "7.png").c_str(), _textureWidth, _textureHeight));
	_numberTextures.push_back(textureManager.textureManager((numberPath + "8.png").c_str(), _textureWidth, _textureHeight));
	_numberTextures.push_back(textureManager.textureManager((numberPath + "9.png").c_str(), _textureWidth, _textureHeight));
	_texture = _numberTextures.at(0);
	_transform = &entity->getComponent<TransformComponent>();
	_sprite = new Sprite(_texture, _transform->width, _transform->height, Origin::TopLeft);
}

void TextRenderComponent::draw(Shader& shader, Vector2D cameraPos)
{
	if (_renderNumber == 0) {
		_sprite->renderNumber(_transform->position, 0.0f, shader, _numberTextures.at(0));
	}
	int number = _renderNumber;
	int loopCount = 0;
	while (number != 0) {
		int nowNumber = number % 10;
		_sprite->renderNumber(_transform->position + Vector2D(-loopCount * _transform->width, 0.f), 0.0f, shader, _numberTextures.at(nowNumber));
		number /= 10;
		loopCount++;
	}
}
