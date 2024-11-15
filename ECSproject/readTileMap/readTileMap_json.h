//
// readTileMap_json.h

#pragma once

#include <iostream>
#include <vector>

// 地图Tile结构
struct Tile {
    int id;
    int x;
    int y;
};

// 图层结构
struct Layer {
    std::string name;
    std::vector<Tile> tiles;
    bool collider;
};

// 地图结构
struct TileMapData {
    int tileSize;
    int width;
    int height;
    std::vector<Layer> layers;
};

class readTileMap_json
{
public:

    static TileMapData loadMapFromJson(const std::string& filename);
};

