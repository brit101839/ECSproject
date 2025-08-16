#pragma once

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <filesystem>
#include "../ECS/Animation.h"
#include "../ECS/ECS.h"
#include "Enemy.h"

using json = nlohmann::json;

AnimateState parseState(const std::string& stateStr) {
    if (stateStr == "Attacking") return AnimateState::Attacking;
	if (stateStr == "FinalAttacking") return AnimateState::FinalAttack;
    if (stateStr == "useSkill") return AnimateState::usingSkill;
    if (stateStr == "Dying") return AnimateState::Dying;
    if (stateStr == "Died") return AnimateState::Died;
    std::cerr << "no selecting animation state" << std::endl;
    return AnimateState::Idle;
}

AIPreferBehavior parseAIBehavior(const std::string& behavior) {
    if (behavior == "NormalAttackOnly") return AIPreferBehavior::NormalAttackOnly;
    if (behavior == "SkillOnly") return AIPreferBehavior::SkillOnly;
}

Animation parseAnimation(const json& animJson) {
    return Animation(
        animJson.value("row", 0),  // 預設 row = 0
        animJson.value("frames", 1),  // 預設 frames = 1
        animJson.value("speed", 10),  // 預設 speed = 10
        animJson.value("flip", false), // 預設不翻轉
        animJson.value("canInterrupt", true), // 預設可中斷
        parseState(animJson.value("state", "")) // 轉換狀態，預設 None
    );
}

Animation parseAttackAnimation(const json& animJson) {
	AnimateState state = AnimateState::Attacking;

	if (animJson.value("state", "") == "Attacking") state = AnimateState::Attacking;
	else if (animJson.value("state", "") == "FinalAttacking") state = AnimateState::FinalAttack;
	else if (animJson.value("state", "") == "useSkill") state = AnimateState::usingSkill;

    return Animation(
        animJson.value("row", 0),
        animJson.value("frames", 1),
        animJson.value("speed", 10),
        animJson.value("flip", false),
        std::make_shared<AttackDetail>(
            animJson.value("attackFrame", 0),
            animJson.value("damage", 0)),
        state
    );
}

class EnemyFactory {
public:
    static Entity* createEnemyFromJson(Manager& manager, const std::string& enemyType, const Vector2D& position, const TransformComponent& playerTrans, std::shared_ptr<SpawnSystem> spawnSys, EventSystem gEventSys) {
        std::ifstream file("../ECSproject/Enemy/enemy_config.json");
        if (!file.is_open()) {
            std::cerr << "Failed to open enemy_config.json" << std::endl;
            return nullptr;
        }

        json config;
        file >> config;

        if (config.is_null() || config.empty()) {
            std::cerr << "Error: JSON file is empty or invalid" << std::endl;
            return nullptr;
        }

        for (const auto& enemyData : config["enemies"]) {
            if (enemyData["name"] == enemyType) {

                Entity* enemyEntity = &manager.addEntity();

                enemyEntity->addcomponent<LocalEventComponent>(gEventSys);
                auto& trans = enemyEntity->addcomponent<TransformComponent>(position, enemyData["speed"], enemyData["width"], enemyData["height"]);
                enemyEntity->addcomponent<AIComponent>(playerTrans, parseAIBehavior(enemyData["behavior"]));
                auto& stats = enemyEntity->addcomponent<StatsComponent>(enemyData["hp"], enemyData["atk"], enemyData["def"]);
                auto& spriteData = enemyData["sprite"];
                std::string path = spriteData["path"];
                auto& sprite = enemyEntity->addcomponent<SpriteComponent>(path.c_str(), spriteData["animated"], 
                    spriteData["cutWidth"], spriteData["cutHeight"], spriteData.value("offsetX", 0.f));

                for (auto& [name, anim] : enemyData["animations"].items()) {
                    if (anim.value("state", "") == "Attacking" || anim.value("state", "") == "FinalAttacking" || anim.value("state", "") == "useSkill") {
                        sprite.addAnimation(name, parseAttackAnimation(anim));
                    }
                    else {
                        sprite.addAnimation(name, parseAnimation(anim));
                    }
                }
                sprite.setAnimate("idle"); // 預設為 idle 動畫

                enemyEntity->addcomponent<SkillCompnent>("fire ball", spawnSys);
                // enemyEntity->addcomponent<StatsComponent>(100, 10, 1);
                enemyEntity->addcomponent<HealthBarComponent>(Vector2D(enemyData["width"], 10.f));

                return enemyEntity;
            }
        }

        std::cerr << "Enemy type " << enemyType << " not found!" << std::endl;
        return nullptr;
    }
};
