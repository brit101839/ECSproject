#pragma once

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "../ECS/Animation.h"
#include "../ECS/ECS.h"
#include "Enemy.h"

using json = nlohmann::json;

AnimateState parseState(const std::string& stateStr) {
    if (stateStr == "Attacking") return AnimateState::Attacking;
    if (stateStr == "Dying") return AnimateState::Dying;
    if (stateStr == "Died") return AnimateState::Died;
    std::cerr << "no selecting animation state" << std::endl;
    return AnimateState::Idle;
}

Animation parseAnimation(const json& animJson) {
    return Animation(
        animJson.value("row", 0),  // �w�] row = 0
        animJson.value("frames", 1),  // �w�] frames = 1
        animJson.value("speed", 10),  // �w�] speed = 10
        animJson.value("flip", false), // �w�]��½��
        animJson.value("canInterrupt", true), // �w�]�i���_
        parseState(animJson.value("state", "")) // �ഫ���A�A�w�] None
    );
}

Animation parseAttackAnimation(const json& animJson) {
   
    return Animation(
        animJson.value("row", 0),
        animJson.value("frames", 1),
        animJson.value("speed", 10),
        animJson.value("flip", false),
        std::make_shared<AttackDetail>(
            animJson.value("attackFrame", 0),
            animJson.value("damage", 0))
    );
}

class EnemyFactory {
public:
    static Entity* createEnemyFromJson(Manager& manager, const std::string& enemyType, const Vector2D& position) {
        std::ifstream file("C:/Users/brit/source/repos/ECSproject/ECSproject/Enemy/enemy_config.json");
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

                enemyEntity->addcomponent<LocalEventComponent>();
                auto& trans = enemyEntity->addcomponent<TransformComponent>(position, enemyData["speed"], enemyData["width"], enemyData["height"]);
                auto& stats = enemyEntity->addcomponent<StatsComponent>(enemyData["hp"], enemyData["atk"], enemyData["def"]);
                auto& spriteData = enemyData["sprite"];
                std::string path = spriteData["path"];
                auto& sprite = enemyEntity->addcomponent<SpriteComponent>(path.c_str(), spriteData["animated"], spriteData["cutWidth"], spriteData["cutHeight"]);
                for (auto& [name, anim] : enemyData["animations"].items()) {
                    if (anim.value("state", "") == "Attacking") {
                        sprite.addAnimation(name, parseAttackAnimation(anim));
                    }
                    else {
                        sprite.addAnimation(name, parseAnimation(anim));
                    }
                }
                sprite.setAnimate("idle"); // �w�]�� idle �ʵe

                return enemyEntity;
            }
        }

        std::cerr << "Enemy type " << enemyType << " not found!" << std::endl;
        return nullptr;
    }
};
