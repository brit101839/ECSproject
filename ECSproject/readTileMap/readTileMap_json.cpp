#include "readTileMap_json.h"
#include <fstream>
#include "nlohmann/json.hpp"

// 使用 nlohmann JSON 命名空间
using json = nlohmann::json;

TileMapData readTileMap_json::loadMapFromJson(const std::string& filename)
{
    TileMapData map;
    std::ifstream file(filename);
    json j;
    file >> j;

    // 解析地图属性
    map.tileSize = j["tileSize"];
    map.width = j["mapWidth"];
    map.height = j["mapHeight"];

    // 解析图层
    for (const auto& layerJson : j["layers"]) {
        Layer layer;
        layer.name = layerJson["name"];
        layer.collider = layerJson["collider"];

        // 解析每个图块
        for (const auto& tileJson : layerJson["tiles"]) {
            Tile tile;
            std::string id = tileJson["id"];
            tile.id = stoi(id);
            tile.x = tileJson["x"];
            tile.y = tileJson["y"];
            layer.tiles.push_back(tile);
        }
        map.layers.push_back(layer);
    }

    return map;
}
