#include "UIManager.h"
#include "ECS/Components.h"
#include <string>

void UIManager::init(TextRender* t)
{
	std::string UIPath = "D:/dependencies/resource/UI/Fantasy Minimal Pixel Art GUI by eta-commercial-free/UI/";
	std::string fullPath = UIPath + "HealthBarPanel_160x41.png";
	auto& stateBarPanel = this->buildUI(Vector2D(50.f, 120.f), 160.f * 2, 41.f * 2, fullPath);
	fullPath = UIPath + "ValueRed_120x8.png";
	buildUI(*_bloodBar, Vector2D(102.f, 77.f), 120.f * 2, 8.f * 2, fullPath);
	fullPath = UIPath + "ValueBar_128x16.png";
	auto& bloodBarFrame = this->buildUI(Vector2D(94.f, 85.f), 128.f * 2, 16.f * 2, fullPath);
	fullPath = UIPath + "ValueBlue_120x8.png";
	auto& magicBar = this->buildUI(Vector2D(102.f, 100.f), 120.f * 2, 4.f * 2, fullPath);
	fullPath = UIPath + "ValueBar_128x11.png";
	auto& magicBarFrame = this->buildUI(Vector2D(94.f, 107.f), 128.f * 2, 11.f * 2, fullPath);
	fullPath = UIPath + "BlackBigCircleBoxWithBorder_27x27.png";
	auto& panelCircle = this->buildUI(Vector2D(20.f, 120.f), 41.f * 2, 41.f * 2, fullPath);
	fullPath = UIPath + "HeartIcons_32x32.png";
	auto& heartIcon = this->buildUI(Vector2D(29.f, 111.f), 32.f * 2, 32.f * 2, fullPath);
	fullPath = UIPath + "CoinIcon_16x18.png";
	auto& CoinIcon = this->buildUI(Vector2D(1150.f, 740.f), 16.f * 3, 18.f * 3, fullPath);

	auto& CoinNumber = _manager.addEntity();
	CoinNumber.addcomponent<TransformComponent>(Vector2D(1210.f, 730.f), 16.f * 1, 18.f * 1);
	CoinNumber.addcomponent<TextRenderComponent>(t);
	CoinNumber.addGroup(groupUI);
	CoinNumber.getComponent<TextRenderComponent>().mText = "1235";
	_uiEntities.push_back(&CoinNumber);
}

Entity& UIManager::buildUI(Vector2D pos, GLfloat w, GLfloat h, std::string path)
{
	auto& UIentity = _manager.addEntity();
	UIentity.addcomponent<TransformComponent>(pos, w, h);
	UIentity.addcomponent<SpriteComponent>(path.c_str(), SpriteType::UI);
	UIentity.addGroup(groupUI);
	_uiEntities.push_back(&UIentity);
	return UIentity;
}

void UIManager::buildUI(Entity& entity, Vector2D pos, GLfloat w, GLfloat h, std::string path)
{
	entity.addcomponent<TransformComponent>(pos, w, h);
	entity.addcomponent<SpriteComponent>(path.c_str(), SpriteType::UI);
	entity.addGroup(groupUI);
	_uiEntities.push_back(&entity);
}

void UIManager::update()
{
	float bloodPercent = _player.getEntity().getComponent<StatsComponent>().mhealthPercent;
	_bloodBar->getComponent<SpriteComponent>().setUIScale(Vector2D(bloodPercent, 1.0f));
}
