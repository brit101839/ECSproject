#include "Map.h"

int lvl1[16][30] = {
	{0,0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

Map::Map()
{
	GLuint textureBufferID = TextureManager::loadMapImage("D:/dependencies/resource/PUNY_WORLD_v1/tilemap.png", 7, 11);
	_drit = new Sprite(textureBufferID, 48, 48);
	textureBufferID = TextureManager::loadMapImage("D:/dependencies/resource/PUNY_WORLD_v1/tilemap.png", 1, 0);
	_grass = new Sprite(textureBufferID, 48, 48);
	textureBufferID = TextureManager::loadMapImage("D:/dependencies/resource/PUNY_WORLD_v1/tilemap.png", 9, 11);
	_water = new Sprite(textureBufferID, 48, 48);

	LoadMap(lvl1);
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
