//
// Map.h

#ifndef __Map__
#define __Map__
#include "Sprite.h"
#include "readTileMap/readTileMap_json.h"
#include "ECS/ECS.h"

#define Window_w_Size 1351
#define Window_h_Size 760

class Map
{
private:

	int _MapWidth, _MapHeight;
	GLfloat _tileSize;

	int _map[16][30];

	Sprite* _drit;
	Sprite* _grass;
	Sprite* _water;

public:

	Map();
	~Map();

	void LoadMap(int arr[16][30]);
	void loadTileMap(TileMapData tileMap);

	void DrawMap();

};

#endif // !__Map__