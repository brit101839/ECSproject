#include "Map.h"
#include "textureManager.h"
#include "nlohmann/json.hpp"
#include "Game.h"

int lvl1[16][30] = {
	{0,0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

Map::Map()
{
	/*GLuint textureBufferID = TextureManager::loadMapImage("D:/dependencies/resource/map2/spritesheet.png", 1, 0);
	_drit = new Sprite(textureBufferID, 48, 48);*/
	/*TextureManager textureManager;
	GLuint textureBufferID = textureManager.TileSpriteManager(1, "D:/dependencies/resource/map2/spritesheet.png");
	std::cout << textureBufferID << std::endl;
	_drit = new Sprite(textureBufferID, 48, 48);
	textureBufferID = TextureManager::loadMapImage("D:/dependencies/resource/map2/spritesheet.png", 0, 0);
	_grass = new Sprite(textureBufferID, 48, 48);
	std::cout << textureBufferID << std::endl;
	textureBufferID = TextureManager::loadMapImage("D:/dependencies/resource/map2/spritesheet.png", 2, 0);
	_water = new Sprite(textureBufferID, 48, 48);

	LoadMap(lvl1);*/

	loadTileMap(readTileMap_json::loadMapFromJson("D:/dependencies/resource/map/map.json"));
}

Map::~Map()
{
}

void Map::LoadMap(int arr[16][30])
{
	for (int row = 0; row < 16; row++) {
		for (int column = 0; column < 29; column++) {
			_map[row][column] = arr[row][column];
		}
	}
}

void Map::loadTileMap(TileMapData tileMap)
{
	_MapWidth = tileMap.width;
	_MapHeight = tileMap.height;
	_tileSize = tileMap.tileSize;

	for (auto layer = tileMap.layers.rbegin(); layer != tileMap.layers.rend(); ++layer) {
		for (const auto& tile : layer->tiles) {
			Game::addTile(tile.id, Vector2D(tile.x * 48, Window_h_Size - tile.y * 48), layer->collider);
		}
	}

	std::cout << "Map Size: " << tileMap.width << "x" << tileMap.height << "\n";
	std::cout << "Tile Size: " << tileMap.tileSize << "\n";
	for (auto layer = tileMap.layers.rbegin(); layer != tileMap.layers.rend(); ++layer) {
		std::cout << "Layer: " << layer->name << ", Collider: " << (layer->collider ? "true" : "false") << "\n";
		for (const auto& tile : layer->tiles) {
			std::cout << "Tile ID: " << tile.id << ", Position: (" << tile.x << ", " << tile.y << ")\n";
		}
	}
}

void Map::DrawMap()
{
	int type = 0;
	for (int row = 0; row < 16; row++) {
		for (int column = 0; column < 30; column++) {
			type = _map[row][column];

			switch (type) {
			case 0:
				_grass->render(Vector2D(column * 48, Window_h_Size - row * 48), 0.0f);
				break;
			case 1:
				_water->render(Vector2D(column * 48, Window_h_Size - row * 48), 0.0f);
				break;
			case 2:
				_drit->render(Vector2D(column * 48, Window_h_Size - row * 48), 0.0f);
				break;
			default:
				break;
			}

		}
	}
}
