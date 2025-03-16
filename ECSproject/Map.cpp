#include "Map.h"
#include "textureManager.h"
#include "nlohmann/json.hpp"
#include "Game.h"

Map::Map(Game& gameInstance, std::string mapJASON)
	: _game(gameInstance)
{
	// loadTileMap(readTileMap_json::loadMapFromJson("D:\\dependencies\\resource\\map_town\\map_town\\map.json"));
	loadTileMap(readTileMap_json::loadMapFromJson(mapJASON));
}

Map::~Map()
{
}

void Map::loadTileMap(TileMapData tileMap)
{
	_MapWidth = tileMap.width;
	_MapHeight = tileMap.height;
	_tileSize = tileMap.tileSize;

	for (auto layer = tileMap.layers.rbegin(); layer != tileMap.layers.rend(); ++layer) {
		for (const auto& tile : layer->tiles) {
			_game.addTile(tile.id, 80, Vector2D(tile.x * 80, SRC_HEIGHT - tile.y * 80), layer->collider);
		}
	}

	/*std::cout << "Map Size: " << tileMap.width << "x" << tileMap.height << "\n";
	std::cout << "Tile Size: " << tileMap.tileSize << "\n";
	for (auto layer = tileMap.layers.rbegin(); layer != tileMap.layers.rend(); ++layer) {
		std::cout << "Layer: " << layer->name << ", Collider: " << (layer->collider ? "true" : "false") << "\n";
		for (const auto& tile : layer->tiles) {
			std::cout << "Tile ID: " << tile.id << ", Position: (" << tile.x << ", " << tile.y << ")\n";
		}
	}*/
}

void Map::DrawMap()
{
	
}
