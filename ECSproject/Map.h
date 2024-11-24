//
// Map.h

#ifndef __Map__
#define __Map__
#include "Sprite.h"
#include "readTileMap/readTileMap_json.h"
#include "ECS/ECS.h"

class Map
{
private:

	int _MapWidth, _MapHeight;
	GLfloat _tileSize;

public:

	Map();
	~Map();

	void loadTileMap(TileMapData tileMap);

	void DrawMap();
};

#endif // !__Map__